#pragma once
#include "Singleton.h"
#include <string>
namespace dae {

	class Scene;

	class JsonLevelReader final : public Singleton<JsonLevelReader>
	{
	public:
		JsonLevelReader();
		void Init(const std::string& basePath) { m_BasePath = basePath; }
		~JsonLevelReader() = default;
		JsonLevelReader(const JsonLevelReader& other) = delete;
		JsonLevelReader(JsonLevelReader&& other) = delete;
		JsonLevelReader& operator=(const JsonLevelReader& other) = delete;
		JsonLevelReader& operator=(JsonLevelReader&& other) = delete;
	
		Scene& ReadAndLoadLevel(const std::string& file);
		
	private:
		std::string m_BasePath{ "" };
	};
}
