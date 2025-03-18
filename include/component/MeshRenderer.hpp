#pragma once

#include <iostream>

#include "Renderer.hpp"
#include "Mesh.hpp"
#include "../Color.hpp"
#include "../shader/Shader.hpp"
#include "../Scene.hpp"

#include "../pipeline/RenderPipeline.hpp"

class MeshRenderer : public Renderer
{
public:
	MeshRenderer();
	~MeshRenderer();
	void Update() override;
	void InitMesh(Mesh* mesh);
	void InitShader(IShader* shader);

	void Renderer();

private:
	Mesh* mesh;
	IShader* shader;


	
};