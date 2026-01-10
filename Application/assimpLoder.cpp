#include "assimpLoder.h"

AssimpLoder::PtrNode AssimpLoder::load(const std::string& path)
{
	//拿取模型路径
	std::size_t index = path.find_last_of("//");
	std::string strRootPath = path.substr(0, index + 1);

	PtrNode pRetNode = Node::Create();
	Assimp::Importer assimpImporter;
	const aiScene* pAiScene = assimpImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);//三角形，生成法线

	if (!pAiScene
		|| pAiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
		|| !pAiScene->mRootNode
		)
	{
		std::cerr << "Error: Model Read Failed!" << std::endl;
		return nullptr;
	}

	processingAiSceneNode(pAiScene, pAiScene->mRootNode, pRetNode, strRootPath);

	return pRetNode;
}

void AssimpLoder::processingAiSceneNode(const aiScene* const pAiScene, aiNode* pAiNode, PtrNode pParentNode, const std::string& strRootPath)
{
	PtrNode newNode = Node::Create();
	newNode->setParent(pParentNode);

	glm::vec3 vec3Position;
	glm::vec3 vec3EulerAngle;
	glm::vec3 vec3Scale;

	glm::mat4 mat4TransForm = transAiMatrixToGlmMat4(pAiNode->mTransformation);
	Tools::decomposeMat4(mat4TransForm, vec3Position, vec3EulerAngle, vec3Scale);

	newNode->setPosition(vec3Position);
	newNode->setAngleX(vec3EulerAngle.x);
	newNode->setAngleY(vec3EulerAngle.y);
	newNode->setAngleZ(vec3EulerAngle.z);
	newNode->setScale(vec3Scale);

	for (int i = 0; i < pAiNode->mNumMeshes; i++)
	{
		int iMeshID = pAiNode->mMeshes[i];
		aiMesh* pAiMesh = pAiScene->mMeshes[iMeshID];
		auto pMyMesh = processAiMesh(pAiScene, pAiMesh, strRootPath);
		pMyMesh->setParent(newNode);
	}

	for (int i = 0; i < pAiNode->mNumChildren; i++)
	{
		processingAiSceneNode(pAiScene, pAiNode->mChildren[i], newNode, strRootPath);
	}
}

AssimpLoder::PtrMesh AssimpLoder::processAiMesh(const aiScene* const pAiScene, aiMesh* pAimesh, const std::string& strRootPath)
{
	
	std::vector<float> vctPositions;
	std::vector<float> vctNormals;
	std::vector<float> vctUvs;
	std::vector<unsigned int> vctIndices;

	for (int i = 0; i < pAimesh->mNumVertices; i++)
	{
		//第i个顶点的位置
		vctPositions.push_back(pAimesh->mVertices[i].x);
		vctPositions.push_back(pAimesh->mVertices[i].y);
		vctPositions.push_back(pAimesh->mVertices[i].z);

		//法线
		vctNormals.push_back(pAimesh->mNormals[i].x);
		vctNormals.push_back(pAimesh->mNormals[i].y);
		vctNormals.push_back(pAimesh->mNormals[i].z);

		//uv
		//多套uv
		if (pAimesh->mTextureCoords[0])
		{
			vctUvs.push_back(pAimesh->mTextureCoords[0][i].x);
			vctUvs.push_back(pAimesh->mTextureCoords[0][i].y);
		}
		else
		{
			vctUvs.push_back(0.0f);
			vctUvs.push_back(0.0f);
		}
	}

	//索引数据
	for (int i = 0; i < pAimesh->mNumFaces; i++)
	{
		aiFace face = pAimesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			vctIndices.push_back(face.mIndices[j]);
		}
	}

	PtrGeomotry pGeometry = std::make_shared<Geometry>(vctPositions, vctNormals, vctUvs, vctIndices);
	PtrMaterial pMaterial = std::make_shared<PhongMaterial>();
	
	pMaterial->setShiness(16.0f);

	if (pAimesh->mMaterialIndex >= 0)
	{
		PtrTexture pTexture;
		aiMaterial* pAiMaterial = pAiScene->mMaterials[pAimesh->mMaterialIndex];

		pTexture = processTexture(pAiMaterial, aiTextureType_DIFFUSE, pAiScene, strRootPath, 0);
		if (!pTexture)
		{
			pMaterial->setDiffuseTexture(Texture::createTexture("assets/textures/defaultTexture.jpg", 0));
		}
		else
		{
			pMaterial->setDiffuseTexture(pTexture);
		}

		pTexture.reset();
		pTexture = processTexture(pAiMaterial, aiTextureType_SPECULAR, pAiScene, strRootPath, 1);
		if (!pTexture)
		{
			pMaterial->setSpecularMaskTexture(Texture::createTexture("assets/textures/sp_mask.png", 1));
		}
		else
		{
			pMaterial->setSpecularMaskTexture(pTexture);
		}
	}
	else
	{
		pMaterial->setDiffuseTexture(Texture::createTexture("assets/textures/defaultTexture.jpg", 0)); 
		pMaterial->setSpecularMaskTexture(Texture::createTexture("assets/textures/sp_mask.png", 1));
	}

	PtrMesh pMesh = Mesh::Create(pGeometry, pMaterial);
	return pMesh;
}

AssimpLoder::PtrTexture AssimpLoder::processTexture(const aiMaterial* pAimaterial, const aiTextureType& textureType, const aiScene* pAiScene, const std::string& strRootPath, unsigned int uiUnit)
{
	PtrTexture pTexture;
	//获取图片读取路径
	aiString aiPath;
	pAimaterial->Get(AI_MATKEY_TEXTURE(textureType, 0), aiPath);
	if (!aiPath.length)
	{
		return nullptr;
	}

	//判断纹理图片是不是内嵌的
	const aiTexture* pAitexture = pAiScene->GetEmbeddedTexture(aiPath.C_Str());
	if (pAitexture)
	{
		//内嵌，内存创建Texture
		unsigned char* pDataIn = reinterpret_cast<unsigned char*>(pAitexture->pcData);
		uint32_t uiWidth = pAitexture->mWidth;
		uint32_t uiHeight = pAitexture->mHeight;
		pTexture = Texture::createTextureFromMemory(aiPath.C_Str(), uiUnit, pDataIn, uiWidth, uiHeight);
	}
	else
	{
		//纹理图片在硬盘
		std::string strFullPath = strRootPath + aiPath.C_Str();
		pTexture = Texture::createTexture(strFullPath, uiUnit);
	}

	return pTexture;
}

glm::mat4 AssimpLoder::transAiMatrixToGlmMat4(aiMatrix4x4 maTrix)
{
	glm::mat4 retMat4(
		maTrix.a1, maTrix.a2, maTrix.a3, maTrix.a4,
		maTrix.b1, maTrix.b2, maTrix.b3, maTrix.b4,
		maTrix.c1, maTrix.c2, maTrix.c3, maTrix.c4,
		maTrix.d1, maTrix.d2, maTrix.d3, maTrix.d4
	);

	return retMat4;
}

