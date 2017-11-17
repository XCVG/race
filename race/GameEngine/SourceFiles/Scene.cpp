#include "../HeaderFiles/Scene.h"
#define PI 3.14159265

Scene::Scene()
{
	setUpSceneOne();
}

Scene::~Scene()
{
	_worldObjects.clear();
	//_worldObjects.clear();
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

std::map<std::string, GameObject*> Scene::getWorldObjects() {
	return _worldObjects;
}

RenderableScene* Scene::getRenderInformation()
{
	//_objectsMutex.lock();
	RenderableScene* rs = new RenderableScene();

	for (std::map<std::string, GameObject*>::iterator it = _worldObjects.begin(); it != _worldObjects.end(); ++it) {
		if (it->first == "Camera" && it->second->getComponent<CameraComponent*>() != nullptr) 
		{
			RenderableCamera rc;
			CameraComponent *cc = it->second->getComponent<CameraComponent*>();
			rc.clearColor = Vector3ToGLMVector(cc->getClearColor());
			rc.nearPlane = cc->getNearPlane();
			rc.farPlane = cc->getFarPlane();
			rc.position = Vector3ToGLMVector(it->second->_transform.getPosition());
			rc.rotation = Vector3ToGLMVector(it->second->_transform._orientation.MakeEulerAnglesFromQ());
			rc.viewAngle = cc->getAngle();
			rs->camera = rc;
		}			
		else 
		{
			if (it->second->hasComponent<LightComponent*>()) {
				RenderableLight rl;
				LightComponent *lc = it->second->getComponent<LightComponent*>();
				rl.type = lc->_type;
				rl.intensity = lc->_intensity;
				rl.color = Vector3ToGLMVector(lc->_color);
				rl.angle = lc->_angle;
				rl.range = lc->_range;
				rl.position = Vector3ToGLMVector(it->second->_transform.getPosition());
				rl.rotation = Vector3ToGLMVector(it->second->_transform._rotation);
				rl.scale = FloatToGLMVector(it->second->_transform.getScale());
				rs->lights.push_back(rl);
			}

			if (it->second->hasComponent<RenderComponent*>()) {
				RenderableObject ro;
				RenderComponent *rc = it->second->getComponent<RenderComponent *>();
				ro.albedoName = rc->getAlbedoName();
				ro.normalName = rc->getNormalName();
				ro.smoothness = rc->getSmoothness();
				ro.modelName = rc->getModelName();
				ro.position = Vector3ToGLMVector(it->second->_transform.getPosition());			
				ro.rotation = Vector3ToGLMVector(it->second->_transform._rotation);
				ro.scale = FloatToGLMVector(it->second->_transform.getScale());
				rs->objects.push_back(ro);
			}
		}
	}
	//_objectsMutex.unlock();
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
	GameObject *go = new GameObject(new Transform(new Vector3(0, 2, -5), new Vector3(0, 0, 0), 1.0f));
	go->addComponent(new CameraComponent(new Vector3(0.5,0.5,0.5), 0.1f, 1000.0f, 1.05f));
	addGameObject("Camera", go);

	InputInitializeContent* content = new InputInitializeContent(); 
	content->camera = go;

	go = new GameObject(new Transform(new Vector3(0, 2, 2), new Vector3(0, PI / 4, 0), 1.0f));
	go->addComponent(new RenderComponent("cube", "crate", "", 0));
	addGameObject("Cube", go);

	go = new GameObject(new Transform(new Vector3(5, 2.5, 0), new Vector3(0, 0, 0), 2.0f));
	go->addComponent(new RenderComponent("sphere", "rainbow", "", 1.0f)); 
	addGameObject("Sphere", go);

	go = new GameObject(new Transform(new Vector3(0, 0.5f, 0), new Vector3(0, 0, 0), 1.0f));
	go->addComponent(new RenderComponent("carModel", "test_texture3", "", 0.75f));
	go->addComponent(new RigidBodyComponent(2.5f, 60.0f, 1850.0f, 0.0f, 0.0f, 0.0f));
	GameObject *childF = new GameObject(new Transform(new Vector3(0, 2.5, 10), new Vector3(0, 0, 0), 2.0f), "front");
	GameObject *childR = new GameObject(new Transform(new Vector3(0, 2.5, -10), new Vector3(0, 0, 0), 2.0f), "rear");
	go->addChild(childF);
	go->addChild(childR);
	addGameObject("Player", go);
	addGameObject("Player.ChildF", childF); // DEBUG: Check this is running properly
	addGameObject("Player.ChildR", childR);

	content->player = go;
	std::shared_ptr<Message> msg = std::make_shared<Message>(Message(MESSAGE_TYPE::InputInitializeCallType, false));
	msg->setContent(content);
	MessagingSystem::instance().postMessage(msg);

	go = new GameObject(new Transform(new Vector3(0, 0, 0), new Vector3(0, 0, 0), 3.0f));
	go->addComponent(new RenderComponent("raceTrack", "test_texture2", "", 0));
	addGameObject("Road", go);

	go = new GameObject();
	go->addComponent(new LightComponent(0.25f, new Vector3(1, 1, 1), 0.0f, 0.0f, RenderableLightType::AMBIENT));
	addGameObject("LightAmb", go);

	go = new GameObject(new Transform(new Vector3(0, 10.0f, 0), new Vector3(1.25f, 0, 0), 1.0f));
	//go->addComponent(new RenderComponent("cube", "crate", "", 0)); 
	go->addComponent(new LightComponent(0.5f, new Vector3(1.0f, 0.9f, 0.85f), 1000.0f, 0.0f, RenderableLightType::DIRECTIONAL)); 
	addGameObject("LightDir", go);

	go = new GameObject( new Transform(new Vector3(3.0f, 5.0f, 3.0f), new Vector3(0,0,0), 1.0f));
	go->addComponent(new RenderComponent("cube", "crate", "", 0));
	go->addComponent(new LightComponent(2.0f, new Vector3(0.0,0.0, 0.0), 20.0f, 0.0f, RenderableLightType::POINT));
	addGameObject("Light2", go);

	go = new GameObject(new Transform(new Vector3(-5.0f, 2.0f, 0.0f), new Vector3(0, 1.5f, 0), 1.0f));
	go->addComponent(new RenderComponent("cube", "crate", "", 0));
	go->addComponent(new LightComponent(5.0f, new Vector3(1.0f, 1.0f, 1.0f), 15.0f, 0.6f, RenderableLightType::SPOT));
	addGameObject("Light3", go);
	std::shared_ptr<Message> myMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::SceneDoneLoadType));
	MessagingSystem::instance().postMessage(myMessage);
};

