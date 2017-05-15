#include "SceneManager.h"

SceneManager* SceneManager::sceneManager = nullptr;

void SceneManager::ChangeScene(Scene * const& _scene)
{
	if (scene != nullptr)
	{
		scene->SceneEnd();
		delete scene;
	}
	scene = _scene;
	scene->Start();
}

SceneManager * SceneManager::GetSceneManager()
{
	return sceneManager == nullptr ? new SceneManager() : sceneManager;
}

Scene * SceneManager::GetCurrentScene()
{
	return scene;
}

void SceneManager::DeleteSceneManager()
{
	delete sceneManager;
}

SceneManager::~SceneManager()
{
	if (scene != nullptr)
	{
		scene->SceneEnd();
		delete scene;
	}
}

SceneManager::SceneManager()
{
	scene = nullptr;
}


