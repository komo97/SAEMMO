#pragma once
#include "Scene.h"
class MainScene :
	public Scene
{
public:
	void Start() final;
	void Update() final;
	void Draw() final;
	void SceneEnd() final;
};

