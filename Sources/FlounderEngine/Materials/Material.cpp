#include "Material.hpp"

namespace Flounder
{
	Material::Material(const Colour &colour, Texture *diffuse, const float &metallic, const float &roughness, const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog, Texture *material) :
		Component(),
		m_colour(new Colour(colour)),
		m_textureDiffuse(diffuse),
		m_metallic(metallic),
		m_roughness(roughness),
		m_castsShadows(castsShadows),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog),
		m_textureMaterial(material)
	{
		Link<std::string>(0, "Diffuse Colour", LINK_GET_STR(Colour::GetHex(*GetColour())), LINK_SET(std::string, SetColour(Colour(v))));
		Link<std::string>(1, "Diffuse Texture", LINK_GET_RES(GetTextureDiffuse()), LINK_SET(std::string, TrySetTextureDiffuse(v)));
		Link<float>(2, "Metallic", LINK_GET(GetMetallic()), LINK_SET(float, SetMetallic(v)));
		Link<float>(3, "Roughness", LINK_GET(GetRoughness()), LINK_SET(float, SetRoughness(v)));
		Link<bool>(4, "Cast Shadows", LINK_GET(GetCastsShadows()), LINK_SET(bool, SetCastsShadows(v)));
		Link<bool>(5, "Ignore Lighting", LINK_GET(GetIgnoreLighting()), LINK_SET(bool, SetIgnoreLighting(v)));
		Link<bool>(6, "Ignore Fog", LINK_GET(GetIgnoreFog()), LINK_SET(bool, SetIgnoreFog(v)));
		Link<std::string>(7, "Material Texture", LINK_GET_RES(GetTextureMaterial()), LINK_SET(std::string, TrySetTextureMaterial(v)));
	}

	Material::~Material()
	{
		delete m_colour;
	}

	void Material::Update()
	{
		Component::Update();
	}

	void Material::TrySetTextureDiffuse(const std::string &filename)
	{
		if (filename.empty() || filename == "nullptr")
		{
			return;
		}

		m_textureDiffuse = Texture::Resource(filename);
	}

	void Material::TrySetTextureMaterial(const std::string &filename)
	{
		if (filename.empty() || filename == "nullptr")
		{
			return;
		}

		m_textureMaterial = Texture::Resource(filename);
	}
}