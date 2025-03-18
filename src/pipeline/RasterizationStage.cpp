#include "pipeline/RasterizationStage.hpp"

void RasterizationStage::TriangleSetup()
{
    //没啥吊用
}

void RasterizationStage::TriangleTraversal()
{
    //三角形遍历,如果一个像素在三角形内部则生成一个片元

}

void RasterizationStage::FragmentShader(IShader* shader)
{
    
}

void RasterizationStage::FragmentOperation()
{
    //深度测试
}