#include "pch.h"
#include "ResourceSystem.h"
#include "Logger.h"

namespace GameEngine
{
	ResourceSystem* ResourceSystem::Instance()
	{
		static ResourceSystem resourceSystem;
		return &resourceSystem;
	}

	void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
	{
		if (textures.find(name) != textures.end())
		{
			LOG_WARN("Texture already loaded: " + name);
			return;
		}

		sf::Texture* newTexture = new sf::Texture();
		if (!newTexture) {
			LOG_ERROR("Failed to allocate texture: " + name);
			throw std::bad_alloc();
		}

		if (newTexture->loadFromFile(sourcePath))
		{
			newTexture->setSmooth(isSmooth);
			textures.emplace(name, newTexture);
			LOG_INFO("Loaded texture: " + name + " from " + sourcePath);
		}
		else
		{
			delete newTexture;
			LOG_ERROR("Failed to load texture from file: " + sourcePath);
			throw std::runtime_error("Failed to load texture: " + sourcePath);
		}
	}

	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
	{
		auto it = textures.find(name);
		if (it == textures.end()) {
			LOG_ERROR("Texture not found: " + name);
			throw std::runtime_error("Texture not found: " + name);
		}
		return it->second;
	}

	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		auto it = textures.find(name);
		if (it == textures.end()) {
			LOG_ERROR("Texture not found for copy: " + name);
			throw std::runtime_error("Texture not found for copy: " + name);
		}
		return new sf::Texture(*it->second);
	}

	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		auto texturePair = textures.find(name);
		if (texturePair == textures.end()) {
			LOG_WARN("Attempted to delete non-existent texture: " + name);
			return;
		}

		sf::Texture* deletingTexure = texturePair->second;
		textures.erase(texturePair);
		delete deletingTexure;
		LOG_INFO("Deleted texture: " + name);
	}

	void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
	{
		if (textureMaps.find(name) != textureMaps.end())
		{
			LOG_WARN("Texture map already loaded: " + name);
			return;
		}

		sf::Texture textureMap;
		if (!textureMap.loadFromFile(sourcePath)) {
			LOG_ERROR("Failed to load texture map from file: " + sourcePath);
			throw std::runtime_error("Failed to load texture map: " + sourcePath);
		}

		auto textureMapElements = new std::vector<sf::Texture*>();
		auto textureSize = textureMap.getSize();
		int loadedElements = 0;

		for (unsigned int y = 0; y <= textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
		{
			if (loadedElements == totalElements)
			{
				break;
			}

			for (unsigned int x = 0; x <= textureSize.x - elementPixelSize.x; x += elementPixelSize.x)
			{
				if (loadedElements == totalElements)
				{
					break;
				}

				sf::Texture* newTextureMapElement = new sf::Texture();
				if (newTextureMapElement->loadFromFile(sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
				{
					newTextureMapElement->setSmooth(isSmooth);
					textureMapElements->push_back(newTextureMapElement);
				}
				else
				{
					delete newTextureMapElement;
					LOG_ERROR("Failed to load texture map element at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
				}
				loadedElements++;
			}
		}

		textureMaps.emplace(name, *textureMapElements);
		LOG_INFO("Loaded texture map: " + name + " with " + std::to_string(loadedElements) + " elements");
	}

	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end()) {
			LOG_ERROR("Texture map not found: " + name);
			throw std::runtime_error("Texture map not found: " + name);
		}

		if (elementIndex < 0 || elementIndex >= static_cast<int>(textureMap->second.size())) {
			LOG_ERROR("Texture map element index out of bounds: " + std::to_string(elementIndex) + " for map: " + name);
			throw std::out_of_range("Texture map element index out of bounds");
		}

		return textureMap->second[elementIndex];
	}

	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end()) {
			LOG_ERROR("Texture map not found: " + name);
			throw std::runtime_error("Texture map not found: " + name);
		}

		if (elementIndex < 0 || elementIndex >= static_cast<int>(textureMap->second.size())) {
			LOG_ERROR("Texture map element index out of bounds: " + std::to_string(elementIndex) + " for map: " + name);
			throw std::out_of_range("Texture map element index out of bounds");
		}

		return new sf::Texture(*textureMap->second[elementIndex]);
	}

	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end()) {
			LOG_WARN("Texture map not found for count query: " + name);
			return 0;
		}
		return static_cast<int>(textureMap->second.size());
	}

	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end()) {
			LOG_WARN("Attempted to delete non-existent texture map: " + name);
			return;
		}

		auto deletingTextures = textureMap->second;
		for (size_t i = 0; i < deletingTextures.size(); i++)
		{
			delete deletingTextures[i];
		}
		textureMaps.erase(textureMap);
		LOG_INFO("Deleted texture map: " + name);
	}

	void ResourceSystem::Clear()
	{
		LOG_INFO("Clearing all resources");
		DeleteAllTextures();
		DeleteAllTextureMaps();
	}

	void ResourceSystem::DeleteAllTextures()
	{
		std::vector<std::string> keysToDelete;
		for (const auto& texturePair : textures)
		{
			keysToDelete.push_back(texturePair.first);
		}
		for (const auto& key : keysToDelete)
		{
			DeleteSharedTexture(key);
		}
		LOG_INFO("All textures deleted");
	}

	void ResourceSystem::DeleteAllTextureMaps()
	{
		std::vector<std::string> keysToDelete;
		for (const auto& textureMapPair : textureMaps)
		{
			keysToDelete.push_back(textureMapPair.first);
		}
		for (const auto& key : keysToDelete)
		{
			DeleteSharedTextureMap(key);
		}
		LOG_INFO("All texture maps deleted");
	}
}