#pragma once
#define g_MaterialBank MaterialBank::GetInstance()
#include <map>
#include <vector>
#include "Texture.h"
struct aiScene;
typedef int TextureHandle;
namespace gfx
{
	struct Model;class Material;
	class MaterialBank
	{
	public:
		~MaterialBank();
		static MaterialBank& GetInstance();
		void LoadMaterials(Model& model, std::string filename, const aiScene* scene);
		void BuildMaterialBuffer();
		void ClearMaterials();
		Material* GetMaterial(int matIndex);
		Material* GetMaterial(const std::string& name);
		int GetMaterialID(const std::string& name);
		TextureHandle LoadTexture(const char* filename, TextureType type);
		Texture* GetTexture(TextureHandle handle);
	private:
		MaterialBank();
		int									m_Numerator = 0;
		std::vector<Material*>				m_Materials;
		std::map<std::string, Material*>	m_MatMap;
		std::vector<Texture*>				m_Textures;
	};
}
