#include <iostream>

#include "include/Scene.hpp"
#include "include/component/Transform.hpp"
#include "include/component/Mesh.hpp"
#include "include/gameobject/Light.hpp"
#include "include/gameobject/Camera.hpp"
#include "include/pipeline/Buffer.hpp"
#include "include/Color.hpp"
#include "include/component/MeshRenderer.hpp"
#include "include/ui.hpp"
#include <ctime>
int main() 
{
    std::cout<<"Hello SoftRenderer"<<std::endl;

    Scene* scene = Scene::GetInstance();

    GameObject* gameObject = new GameObject();
    gameObject->GetComponent<Transform>()->position = Vector3f(0.0f, 0.0f, 0.0f);
    //gameObject->GetComponent<Transform>()->scale = Vector3f(0.05f, 0.05f, 0.05f);
    Mesh* mesh = gameObject->AddComponent<Mesh>();
    mesh->LoadMesh("D:/program/cpp_program/SoftRenderer_v2.0/obj/african_head/african_head.obj");
    //mesh->LoadMesh("D:/program/cpp_program/SoftRenderer_v2.0/obj/mountainside_4k/mountainside_4k.obj");
    //mesh->LoadMesh("D:/program/cpp_program/SoftRenderer_v2.0/obj/cube.obj");
    //mesh->LoadMesh("D:/program/cpp_program/SoftRenderer_v2.0/obj/diablo3_pose/diablo3_pose.obj");
    //mesh->Print();
    gameObject->AddComponent<MeshRenderer>()->InitMesh(mesh);
    IShader* shader = new BlinnPhongShader();
    //IShader* shader = new DefaultShader();
    gameObject->GetComponent<MeshRenderer>()->InitShader(shader);

    Light* light = new Light();
    light->color = Color(1.0f, 1.0f, 1.0f);
    light->GetComponent<Transform>()->position = Vector3f(0.0f, 0.0f, 2.0f);

    Camera* camera = new Camera();
    camera->GetComponent<Transform>()->position = Vector3f(0.0f, 0.0f, 3.0f);

    float rotateArray[3][3] = 
    {
        {-1, 0, 0},
        {0, 1, 0},
        {0, 0, -1}
    };

    camera->GetComponent<Transform>()->rotationMatrix = Matrix3x3f(rotateArray);

    scene->AddGameObject(gameObject);
    scene->AddLight(light);
    scene->SetCamera(camera);

    // int frame = 0;
    // using std::cout,std::endl;
    // using std::chrono::duration_cast,std::chrono::system_clock;
    // using std::chrono::milliseconds,std::chrono::seconds;
    // auto millisec_since_epoch =
    // duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    // while(true)
    // {
    //     scene->Update();
    //     frame ++;
    //     if(frame == 50)
    //     {
    //         std::cout<<"FPS: "<<50.0f/(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch)*1000<<std::endl;
    //         frame = 0;
    //         millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    //     }
    // }

    //scene->Update();
    ui();

    std::cout << "End" << std::endl;    

    exit(0);

    return 0;
}
