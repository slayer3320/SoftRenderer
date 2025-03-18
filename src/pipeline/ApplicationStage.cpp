#include "pipeline/ApplicationStage.hpp"

void ApplicationStage::PepareData(Mesh* mesh)
{
    if (a2v_data == nullptr) 
    {
        a2v_data = new std::vector<Shader_Struct_a2v*>();
    }

    a2v_data->reserve(mesh->vertices.size());

    for (int i = 0; i < mesh->vertices.size(); i++)
    {
        if((*a2v_data)[i] == nullptr)
        {
            (*a2v_data)[i] = new Shader_Struct_a2v();
        }

        (*a2v_data)[i]->pos = mesh->vertices[i].pos;
        (*a2v_data)[i]->normal = mesh->vertices[i].normal;
        (*a2v_data)[i]->texcoord = mesh->vertices[i].texcoord;
    }
}

void ApplicationStage::NextStage()
{
    GeometricStage::GetInstance()->a2v_data = a2v_data;
}