#pragma once



class GameObject;

class Component
{
public:
	//当通过基类指针删除派生类对象时，如果基类的析构函数不是虚的，那么只有基类的析构函数会被调用，派生类的析构函数不会被执行。
	virtual ~Component() = default;
	virtual void Update() = 0;

	void SetGameObject(GameObject* gameObject);
	GameObject* GetGameObject();
private:
	GameObject* gameObject;
};