#pragma once

#include "gameobject/GameObject.hpp"
#include "gameobject/Light.hpp"
#include "gameobject/Camera.hpp"
#include "Singleton.hpp"

class Scene : public Singleton<Scene>
{
public:
	void Update();
	void AddGameObject(GameObject* gameObject);
	void AddLight(Light* light);
	void SetCamera(Camera* camera);

	GameObject* GetGameObject(int index);
	Light* GetLight();
	Camera* GetCamera();
	
	

private:
	std::vector<GameObject*> _gameObjects;
	Light* _lights;
	Camera* _camera;
};