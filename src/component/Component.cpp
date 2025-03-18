#include "component/Component.hpp"

void Component::SetGameObject(GameObject* gameObject)
{
    this->gameObject = gameObject;
}

GameObject* Component::GetGameObject()
{
    return gameObject;
}