#pragma once
class Scene {
public:
	virtual void Start();
	virtual void Update();
	virtual void Draw();
	virtual void SceneEnd();
};