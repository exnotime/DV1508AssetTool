#include "MaterialBank.h"
#include <fstream>
#include <assimp/scene.h>
#include "Model.h"
#include "Material.h"
#include "utility.h"

gfx::MaterialBank::MaterialBank()
{
}

gfx::MaterialBank::~MaterialBank()
{
	ClearMaterials();
}

gfx::MaterialBank& gfx::MaterialBank::GetInstance()
{
	static MaterialBank m_Bank;
	return m_Bank;
}

void gfx::MaterialBank::LoadMaterials(Model& model, std::string filename, const aiScene* scene){
	model.MaterialOffset = (unsigned int)m_Materials.size();

	for (unsigned int i = 0; i < scene->mNumMaterials; i++){
		Material* modelMat = new Material();
		const aiMaterial* mat = scene->mMaterials[i];
		//get name
		aiString name;
		mat->Get(AI_MATKEY_NAME, name);
		modelMat->SetName(std::string(name.C_Str()));
		//diffuse albedo
		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0){
			aiString path;
			if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
				std::string fullpath = GetDir(filename) + path.data;
				modelMat->SetAlbedoTexture(LoadTexture(fullpath.c_str(), TEXTURE_COLOR));
			}
		}
		else {
			modelMat->SetAlbedoTexture(LoadTexture("asset/whitePixel.png", TEXTURE_COLOR));
		}
		//normal map
		if (mat->GetTextureCount(aiTextureType_HEIGHT) > 0){
			aiString path;
			if (mat->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
				std::string fullpath = GetDir( filename ) + path.data;
				modelMat->SetNormalTexture(LoadTexture(fullpath.c_str(), TEXTURE_COLOR));
			}
		}
		else {
			modelMat->SetNormalTexture(LoadTexture("asset/normal.dds", TEXTURE_COLOR));
		}
		m_Materials.push_back(modelMat);
	}
}

void gfx::MaterialBank::ClearMaterials()
{
	for (int i = 0; i < m_Materials.size(); i++)
	{
		delete m_Materials[i];
	}
	m_Materials.clear();
	m_MatMap.clear();
}


gfx::Material* gfx::MaterialBank::GetMaterial(int matIndex){
	if(matIndex == -1)
		return nullptr;
	return m_Materials[matIndex];
}

gfx::Material* gfx::MaterialBank::GetMaterial(const std::string& name){

	std::map<std::string, Material*>::iterator it = m_MatMap.find(name);
	if(it != m_MatMap.end())
	{
		return m_MatMap[name];
	}
	else
		return nullptr;
}

TextureHandle gfx::MaterialBank::LoadTexture(const char* filename, TextureType type){
	Texture* tex = new Texture();
	tex->Init(filename, type);
	m_Textures.push_back(tex);
	return m_Numerator++;
}

gfx::Texture* gfx::MaterialBank::GetTexture(TextureHandle handle){
	return m_Textures[handle];
}