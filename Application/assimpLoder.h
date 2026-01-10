#pragma once
#include "../glframework/core.h"
#include "../glframework/node.h"
#include "../glframework/mesh.h"
#include "../glframework/material/phongMaterial.h"
#include "../glframework/texture.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../glframework/tools/tools.h"

class AssimpLoder
{
public:
	using PtrNode = std::shared_ptr<Node>;
	using PtrMesh = std::shared_ptr<Mesh>;
	using PtrMaterial = std::shared_ptr<PhongMaterial>;
	using PtrGeomotry = std::shared_ptr<Geometry>;
	using PtrTexture = std::shared_ptr<Texture>;

	static PtrNode load(const std::string& path);
public:
	static void processingAiSceneNode(
		const aiScene* const pAiScene,
		aiNode *pAiNode,
		PtrNode pParentNode,
		const std::string &strRootPath
	);

	static PtrMesh processAiMesh(
		const aiScene* const pAiScene,
		aiMesh* pAimesh,
		const std::string &strRootPath
	);

	static PtrTexture processTexture(
	    const aiMaterial* pAimaterial,
		const aiTextureType& textureType,
		const aiScene* pAiScene,
		const std::string& strRootPath,
		unsigned int uiUnit
	);

	static glm::mat4 transAiMatrixToGlmMat4(aiMatrix4x4 maTrix);
};