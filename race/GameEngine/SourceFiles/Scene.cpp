#include "../HeaderFiles/Scene.h"
const float MATH_PI = 3.14159;
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
			rc.position = Vector3ToGLMVector(it->second->_transform.getPosition());
			rc.rotation = Vector3ToGLMVector(it->second->_transform.getRotation());
			rc.viewAngle = cc->getAngle();
			rs->camera = rc;
		} else if (it->first == "Light") 
		{
			if (it->second->hasComponent<LightComponent*>()) {
				RenderableLight rl;
				LightComponent *lc = it->second->getComponent<LightComponent*>();
				rl.type = RenderableLightType::AMBIENT;
				rl.intensity = lc->_intensity;
				rl.color = Vector3ToGLMVector(lc->_color);
				rl.angle = lc->_angle;
				rl.range = lc->_range;
				rl.position = Vector3ToGLMVector(it->second->_transform.getPosition());
				rl.rotation = Vector3ToGLMVector(it->second->_transform.getRotation());
				rl.scale = FloatToGLMVector(it->second->_transform.getScale());
				rs->lights.push_back(rl);
			}
		}
		else 
		{
			if (it->second->hasComponent<RenderComponent*>()) {
				RenderableObject ro;
				RenderComponent *rc = it->second->getComponent<RenderComponent *>();
				ro.albedoName = rc->getAlbedoName();
				ro.normalName = rc->getNormalName();
				ro.smoothness = rc->getSmoothness();
				ro.modelName = rc->getModelName();
				ro.position = Vector3ToGLMVector(it->second->_transform.getPosition());
				ro.rotation = Vector3ToGLMVector(it->second->_transform.getRotation());
				ro.scale = FloatToGLMVector(it->second->_transform.getScale());
				rs->objects.push_back(ro);
			}
		}
	}

	return rs;
};

glm::vec3 Scene::Vector3ToGLMVector(Vector3 vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
};

glm::vec3 Scene::FloatToGLMVector(GLfloat num) 
{
	return glm::vec3(num, num, num);
};

void Scene::setUpSceneOne() {
	GameObject *go = new GameObject(new Transform(new Vector3(0, 0, 10), new Vector3(0, 0, 0), 1.0f));
	go->addComponent(new CameraComponent(new Vector3(1,1,1), 0.1f, 1000.0f, 1.05f));
	addGameObject("Camera", go);

	GameObject *go2 = new GameObject(new Transform(new Vector3(0, 2, 2), new Vector3(0, 1.25 * MATH_PI, 0), 1.0f));
	go2->addComponent(new RenderComponent("null", "rainbow", "", 0));
	addGameObject("Cube", go2);

	GameObject *go3 = new GameObject();
	go3->addComponent(new LightComponent(0.5f, new Vector3(1, 1, 1), 0.0f, 0.0f));
	addGameObject("Light", go3);
};

