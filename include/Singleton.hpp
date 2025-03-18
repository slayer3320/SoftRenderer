#pragma once

template<typename T>
class Singleton {
private:
    static T* instance;

protected:
    Singleton() {}
    virtual ~Singleton() {}

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T* GetInstance() {
        if (instance == nullptr) {
            instance = new T();
        }
        return instance;
    }

    static void DestroyInstance() {
        delete instance;
        instance = nullptr;
    }
};

template<typename T>
T* Singleton<T>::instance = nullptr;