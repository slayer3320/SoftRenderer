#include "Scene.hpp"

void Scene::Update()
{
	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::AddGameObject(GameObject* gameObject)
{
	_gameObjects.push_back(gameObject);
}

void Scene::AddLight(Light* light)
{
	_lights = light;
}

void Scene::SetCamera(Camera* camera)
{
	_camera = camera;
}

GameObject* Scene::GetGameObject(int index)
{
	return _gameObjects[index];
}

Camera* Scene::GetCamera()
{
	return _camera;
}

Light* Scene::GetLight()
{
	return _lights;
}