#include "object.h"
#include "light/directionalLight.h"
#include "light/pointLight.h"
#include "light/spotLight.h"
#include "mesh.h"
#include <vector>
#include "light/ambientLight.h"
#include "node.h"
#include <queue>
#include "event/eventBus.h"
#include "../Application/camera/camera.h"

class Scene :public Object<Scene>,public EventListener
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

	void initEvent()
	{
		EventBus::instance().subscribe(EventType::HierarchyChanged, m_pSelf.lock());
		EventBus::instance().subscribe(EventType::ObjectDestroyed, m_pSelf.lock());
	}

	void onEvent(const Event& e) override
	{
		switch (e.type)
		{
		case EventType::HierarchyChanged:
		case EventType::ObjectDestroyed:
			m_structureDirty = true;
			break;
		}
	}

	void setSelf(const Ptr& self)
	{
		m_pSelf = self;
	}

	//如果多次调用影响性能
	void categorizedStorage();

	void categorizedStorageIfNeeded(std::shared_ptr<Camera> pCamera)
	{
		if (!m_structureDirty)
		{
			return;
		}

		categorizedStorage();
		sortTransparentMeshs(pCamera);
		m_structureDirty = false;
	}

	void sortTransparentMeshs(std::shared_ptr<Camera> pCamera)
	{
		std::sort(
			m_vctTransparentMeshs.begin(),
			m_vctTransparentMeshs.end(),
			[pCamera](PtrMesh pMeshA, PtrMesh pMeshB)
			{
				auto PositionA = pMeshA->getWorldPosition();
				auto PositionB = pMeshB->getWorldPosition();

				auto viewMatrix = pCamera->getViewMatrix();
				auto cameraPositionA = viewMatrix * PositionA;
				auto cameraPositionB = viewMatrix * PositionB;

				return cameraPositionA.z < cameraPositionB.z;
			}
		);
	}

	const std::vector<PtrSpotLight>& getSpotLights() { return m_vctSpotLights; }
	const std::vector<PtrPointLight>& getPointLights() { return m_vctPointLights; }
	const std::vector<PtrDirLight>& getDirLights() { return m_vctDirLights; }
	const std::vector<PtrMesh>& getOpaqueMeshs() { return m_vctOpaqueMeshs; }
	const std::vector<PtrMesh>& getTransparentMeshs() { return m_vctTransparentMeshs; }

	void setAmbientLight(std::shared_ptr<AmbientLight> pAmbientLight) 
	{
		m_pAmbientLight = pAmbientLight;
	}

	std::shared_ptr<AmbientLight> getAmbientLight()
	{
		return m_pAmbientLight;
	}

	// ===============================
	// Scene Graph 变动时调用
	// ===============================
	void markStructureDirty()
	{
		m_structureDirty = true;
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
	std::vector<PtrMesh> m_vctOpaqueMeshs; //不透明mesh
	std::vector<PtrMesh> m_vctTransparentMeshs; //透明mesh
	std::shared_ptr<AmbientLight> m_pAmbientLight;

	// ===== structure dirty flag =====
	bool m_structureDirty{ true };
	std::weak_ptr<Scene> m_pSelf;
};