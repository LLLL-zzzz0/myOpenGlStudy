#include "object.h"
#include "light/directionalLight.h"
#include "light/pointLight.h"
#include "light/spotLight.h"
#include "mesh.h"
#include <vector>
#include "light/ambientLight.h"
#include "node.h"
#include <queue>

class Scene :public Object<Scene>
{
public:
	using Ptr = std::shared_ptr<Scene>;
	using PtrSpotLight = std::shared_ptr<SpotLight>;
	using PtrPointLight = std::shared_ptr<PointLight>;
	using PtrDirLight = std::shared_ptr<DirectionalLight>;
	using PtrMesh = std::shared_ptr<Mesh>;

	static Ptr Create()
	{
		return Ptr(new Scene());
	}

	void categorizedStorage();

	const std::vector<PtrSpotLight>& getSpotLights() { return m_vctSpotLights; }
	const std::vector<PtrPointLight>& getPointLights() { return m_vctPointLights; }
	const std::vector<PtrDirLight>& getDirLights() { return m_vctDirLights; }
	const std::vector<PtrMesh>& getMeshs() { return m_vctMeshs; }

	void setAmbientLight(std::shared_ptr<AmbientLight> pAmbientLight) 
	{
		m_pAmbientLight = pAmbientLight;
	}

	std::shared_ptr<AmbientLight> getAmbientLight()
	{
		return m_pAmbientLight;
	}

protected:
	Scene()
		:Object(ObjectType::SCENE)
	{
	}
private:
	std::vector<PtrSpotLight> m_vctSpotLights;
	std::vector<PtrPointLight> m_vctPointLights;
	std::vector<PtrDirLight> m_vctDirLights;
	std::vector<PtrMesh> m_vctMeshs;
	std::shared_ptr<AmbientLight> m_pAmbientLight;
};