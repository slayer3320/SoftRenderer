#pragma once

#include "vector"

#include "../component/Transform.hpp"

class GameObject
{
public:
	GameObject();

	template <typename T>
	T* AddComponent();

	template <typename T>
	T* GetComponent();

	void Update();

private:
	std::vector<Component*> components;
};

template<typename T>
inline T* GameObject::AddComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

	T* component = new T();
	component->SetGameObject(this);
	components.push_back(component);
	return component;
}

template<typename T>
T* GameObject::GetComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

	for (Component* component : components)
	{
		T* castedComponent = dynamic_cast<T*>(component);
		if (castedComponent != nullptr)
		{
			return castedComponent;
		}
	}
	return nullptr;
}

