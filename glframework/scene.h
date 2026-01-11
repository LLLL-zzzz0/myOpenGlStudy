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

	void categorizedStorage(ObjectBase* pObject);

	void categorizedStorageIfNeeded(Camera* pCamera)
	{
		if (!m_structureDirty)
		{
			return;
		}

		categorizedStorage();
		sortTransparentMeshs(pCamera);
		m_structureDirty = false;
	}

	void sortTransparentMeshs(Camera* pCamera)
	{
		std::sort(
			m_vctTransparentMeshs.begin(),
			m_vctTransparentMeshs.end(),
			[pCamera](Mesh* pMeshA, Mesh* pMeshB)
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

	const std::vector<SpotLight*>& getSpotLights() { return m_vctSpotLights; }
	const std::vector<PointLight*>& getPointLights() { return m_vctPointLights; }
	const std::vector<DirectionalLight*>& getDirLights() { return m_vctDirLights; }
	const std::vector<Mesh*>& getOpaqueMeshs() { return m_vctOpaqueMeshs; }
	const std::vector<Mesh*>& getTransparentMeshs() { return m_vctTransparentMeshs; }

	void setAmbientLight(std::shared_ptr<AmbientLight> pAmbientLight) 
	{
		m_pAmbientLight = pAmbientLight;
	}

	AmbientLight* getAmbientLight() const
	{
		return m_pAmbientLight.get();
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

	std::vector<SpotLight*> m_vctSpotLights;
	std::vector<PointLight*> m_vctPointLights;
	std::vector<DirectionalLight*> m_vctDirLights;
	std::vector<Mesh*> m_vctOpaqueMeshs; //不透明mesh
	std::vector<Mesh*> m_vctTransparentMeshs; //透明mesh
	std::shared_ptr<AmbientLight> m_pAmbientLight;

	// ===== structure dirty flag =====
	bool m_structureDirty{ true };
	std::weak_ptr<Scene> m_pSelf;
};