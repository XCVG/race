#include "../HeaderFiles/Scene.h"

Scene::Scene()
{
	GameObject *go = new GameObject();
	go->addComponent(new RenderComponent("Cube", "", "", 0));
	addGameObject("Cube", go);
}

Scene::~Scene()
{
	_worldObjects.clear();
}

void Scene::addGameObject(std::string id, GameObject *obj)
{
	_worldObjects.emplace(std::make_pair(id, obj));
}

void Scene::deleteGameObject(std::string id)
{
	_worldObjects.erase(_worldObjects.find(id));
}

GameObject* Scene::getGameObject(std::string id)
{
	return _worldObjects.find(id)->second;
}

