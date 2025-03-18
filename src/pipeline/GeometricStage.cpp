#include "pipeline/GeometricStage.hpp"

void GeometricStage::VertexShader(IShader* shader)
{
    v2f_data = new std::vector<Shader_Struct_v2f*>(a2v_data->size());

    for(int i = 0; i < a2v_data->size(); i ++)
    {
        (*v2f_data)[i] = shader->VertexShader((*a2v_data)[i]);
    }
}

void GeometricStage::Clipping()
{
    clip_flag = new std::vector<bool>(v2f_data->size(), false);

    for(int i = 0; i < v2f_data->size(); i ++)
    {
        if((*v2f_data)[i]->pos.x < -1 || (*v2f_data)[i]->pos.x > 1 || (*v2f_data)[i]->pos.y < -1 || (*v2f_data)[i]->pos.y > 1 || (*v2f_data)[i]->pos.z < -1 || (*v2f_data)[i]->pos.z > 1)
        {
            (*clip_flag)[i] = true;
        }
    }
}

void GeometricStage::ScreenMapping()
{
    for(int i = 0; i < v2f_data->size(); i ++)
    {
        (*v2f_data)[i]->pos.x = ((*v2f_data)[i]->pos.x + 1) * 0.5 * Buffer::GetInstance()->GetWidth();
        (*v2f_data)[i]->pos.y = ((*v2f_data)[i]->pos.y + 1) * 0.5 * Buffer::GetInstance()->GetHeight();
    }
}

void GeometricStage::NextStage()
{
    RasterizationStage::GetInstance()->v2f_data = v2f_data;
}