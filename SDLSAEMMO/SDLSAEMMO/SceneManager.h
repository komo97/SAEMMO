#pragma once
#include "Scene.h"
class SceneManager
{
public:
	static SceneManager* GetSceneManager();
	void ChangeScene(Scene* const& _scene);
	Scene* GetCurrentScene();
	void DeleteSceneManager();
private:
	static SceneManager* sceneManager;
	SceneManager();
	Scene* scene;
};




