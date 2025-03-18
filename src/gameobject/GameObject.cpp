#include "gameobject/GameObject.hpp"

GameObject::GameObject()
{
	AddComponent<Transform>();
}

void GameObject::Update()
{
	for (Component* component : components)
	{
		component->Update();
	}
}


