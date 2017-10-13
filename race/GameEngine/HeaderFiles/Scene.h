#pragma once
#include <vector>
#include "../RenderEngine/HeaderFiles/RenderableTypes.h"
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
	RenderableScene* getRenderInformation();
	glm::vec3 Scene::Vector3ToGLMVector(Vector3 vec);
	glm::vec3 Scene::FloatToGLMVector(GLfloat num);
private:
	void setUpSceneOne();

};