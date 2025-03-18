#include "component/MeshRenderer.hpp"
#include <chrono>
MeshRenderer::MeshRenderer()
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Update()
{
	Renderer();
}

void MeshRenderer::InitMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void MeshRenderer::InitShader(IShader* shader)
{
	this->shader = shader;
}

void MeshRenderer::Renderer()
{
	//是否输出
	bool isOutput = false;

	if(isOutput)
	{
		using std::cout,std::endl;
		using std::chrono::duration_cast,std::chrono::system_clock;
		using std::chrono::milliseconds,std::chrono::seconds;
		auto millisec_since_epoch =
		duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		Buffer::GetInstance()->Refresh();
		std::cout<<"BufferRefresh: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		
		shader->UpdateShaderData(GetGameObject()->GetComponent<Transform>());
		std::cout<<"UpdateShaderData: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		
		RenderPipeline::GetInstance()->PepareData(mesh);
		std::cout<<"PepareData: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		RenderPipeline::GetInstance()->VertexShader(shader);
		std::cout<<"VertexShader: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		RenderPipeline::GetInstance()->Clipping();
		std::cout<<"Clipping: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();


		RenderPipeline::GetInstance()->ScreenMapping();
		std::cout<<"ScreenMapping: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();


		RenderPipeline::GetInstance()->TriangleSetup();
		std::cout<<"TriangleSetup: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();


		RenderPipeline::GetInstance()->TriangleTraversal(shader);
		std::cout<<"TriangleTraversal: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();


		RenderPipeline::GetInstance()->FragmentOperation();
		std::cout<<"FragmentOperation: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		RenderPipeline::GetInstance()->RefreshData();
		std::cout<<"RenderPipelineRefreshData: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}
	else
	{
		Buffer::GetInstance()->Refresh();
		shader->UpdateShaderData(GetGameObject()->GetComponent<Transform>());
		RenderPipeline::GetInstance()->Preprocess(mesh);
		RenderPipeline::GetInstance()->PepareData(mesh);
		RenderPipeline::GetInstance()->VertexShader(shader);
		RenderPipeline::GetInstance()->Clipping();
		RenderPipeline::GetInstance()->ScreenMapping();
		RenderPipeline::GetInstance()->TriangleSetup();
		RenderPipeline::GetInstance()->TriangleTraversal(shader);
		RenderPipeline::GetInstance()->FragmentOperation();
		RenderPipeline::GetInstance()->RefreshData();
	}

	// std::cout<<"vertex_count: "<<RenderPipeline::GetInstance()->vertex_count<<std::endl;
	// std::cout<<"face_count: "<<RenderPipeline::GetInstance()->face_count<<std::endl;
	// std::cout<<"fragment_count: "<<RenderPipeline::GetInstance()->fragment_count<<std::endl;

	//Buffer::GetInstance()->WriteToBMP("D:/program/cpp_program/SoftRenderer_v2.0/output/image.bmp");
}
