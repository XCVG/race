#include "../HeaderFiles/Scene.h"

Scene::Scene()
{
	setUpSceneOne();
}

Scene::~Scene()
{
	_worldObjects.clear();
}

void Scene::addGameObject(std::string id, GameObject *obj)
{
	_worldObjects.emplace(id, obj);
}

void Scene::deleteGameObject(std::string id)
{
	_worldObjects.erase(_worldObjects.find(id));
}

GameObject* Scene::getGameObject(std::string id)
{
	return _worldObjects.find(id)->second;
}

RenderableScene* Scene::getRenderInformation()
{
	RenderableScene* rs = new RenderableScene();

	for (std::map<std::string, GameObject*>::iterator it = _worldObjects.begin(); it != _worldObjects.end(); ++it) {
		if (it->first == "Camera") 
		{
			RenderableCamera rc;
			CameraComponent *cc = it->second->getComponent<CameraComponent*>();
			rc.clearColor = Vector3ToGLMVector(cc->getClearColor());
			rc.nearPlane = cc->getNearPlane();
			rc.farPlane = cc->getFarPlane();
			rc.position = Vector3ToGLMVector(it->second->getPosition());
			rc.rotation = Vector3ToGLMVector(it->second->getRotation());
			rc.viewAngle = cc->getAngle();
			rs->camera = rc;
		} else if (it->first == "Light") 
		{
			if (it->second->hasComponent<LightComponent>()) {
				RenderableLight rl;
				LightComponent *lc = it->second->getComponent<LightComponent*>();
				rl.type = RenderableLightType::AMBIENT;
				rl.intensity = lc->_intensity;
				rl.color = Vector3ToGLMVector(lc->_color);
				rl.angle = lc->_angle;
				rl.range = lc->_range;
				rl.position = Vector3ToGLMVector(it->second->getPosition());
				rl.rotation = Vector3ToGLMVector(it->second->getRotation());
				rl.scale = FloatToGLMVector(it->second->getScale());
				rs->lights.push_back(rl);
			}
		}
		else 
		{
			if (it->second->hasComponent<RenderComponent>()) {
				RenderableObject ro;
				RenderComponent *rc = it->second->getComponent<RenderComponent *>();
				ro.albedoName = rc->getAlbedoName();
				ro.normalName = rc->getNormalName();
				ro.smoothness = rc->getSmoothness();
				ro.modelName = rc->getModelName();
				ro.position = Vector3ToGLMVector(it->second->getPosition());
				ro.rotation = Vector3ToGLMVector(it->second->getRotation());
				ro.scale = FloatToGLMVector(it->second->getScale());
				rs->objects.push_back(ro);
			}
		}
	}

	return rs;
};

glm::vec3 Scene::Vector3ToGLMVector(Vector3 vec)
{
	return glm::vec3(vec._x, vec._y, vec._z);
};

glm::vec3 Scene::FloatToGLMVector(GLfloat num) 
{
	return glm::vec3(num, num, num);
};

void Scene::setUpSceneOne() {
	GameObject *go = new GameObject(Vector3(0, 0, 5), Vector3(0, 0, 0), 1.0f);
	go->addComponent(new CameraComponent(new Vector3(1,1,1), 0.1f, 1000.0f, 1.05f));
	addGameObject("Camera", go);

	go = new GameObject(Vector3(0, 2, 2), Vector3(0, 0, 0), 1.0f);
	go->addComponent(new RenderComponent("Cube", "crate", "", 0));
	addGameObject("Cube", go);

	go = new GameObject();
	go->addComponent(new LightComponent(0.5f, Vector3(1, 1, 1), 0.0f, 0.0f));
	addGameObject("Light", go);
};

