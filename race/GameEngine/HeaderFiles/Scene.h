#pragma once
#include <vector>
#include "../../GameEngine/Components/HeaderFiles/GameObject.h"

class Scene
{
public:
	Scene();
	~Scene();
	std::map<std::string, GameObject*> _worldObjects;
	void addGameObject(std::string, GameObject*);
	void deleteGameObject(std::string);
	GameObject* getGameObject(std::string);
private:
	

};