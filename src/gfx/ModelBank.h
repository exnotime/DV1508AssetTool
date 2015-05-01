#pragma once
#include "Model.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
/*
This class loads and store all models and vertices and indices.
It also generates vertex and index buffers.
*/
namespace gfx
{
	typedef int ModelHandle;
#define g_ModelBank ModelBank::GetInstance()
	class ModelBank{
	public:
		 ~ModelBank();
		 static ModelBank& GetInstance();
		 const Model& FetchModel(const char* filename);
		 const Model& FetchModel(ModelHandle handle);
		 ModelHandle LoadModel(const char* filename);
		 ModelHandle AddModel(Model& TheModel);
		 ModelHandle AddModel(std::vector<VertexPosNormalTexTangent>& vertices, std::vector<unsigned int>& indices, Model& outModel);
		 void UpdateModel(ModelHandle& handle, Model& model);
		 void BuildBuffers();
		 void DeleteModel(ModelHandle& handle);
		 void ApplyBuffers();
		 void Clear();
		 std::vector<VertexPosNormalTexTangent>& GetVertices();
		 VertexBuffer& GetVertexBuffer( VertexType modelType );
		 GLuint& GetIndexBuffer( VertexType modelType );
	private:
		ModelBank();
		void LoadMeshes(Model& model, const aiScene* scene);
		VertexBuffer							m_VertexBuffer;
		GLuint									m_IndexBuffer;
		ModelHandle								m_Numerator;
		std::map<ModelHandle, Model>			m_Models;
		std::vector<VertexPosNormalTexTangent>	m_Vertices;
		std::vector<GLuint>						m_Indices;
	};
}
