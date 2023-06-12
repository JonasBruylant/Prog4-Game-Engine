#pragma once
#include "Singleton.h"
#include <string>
#include <vector>


namespace dae {

	class Scene;

	class JsonFileReader final : public Singleton<JsonFileReader>
	{
	public:
		JsonFileReader() = default;
		void Init(const std::string& basePath) { m_BasePath = basePath; }
		~JsonFileReader() = default;
		JsonFileReader(const JsonFileReader& other) = delete;
		JsonFileReader(JsonFileReader&& other) = delete;
		JsonFileReader& operator=(const JsonFileReader& other) = delete;
		JsonFileReader& operator=(JsonFileReader&& other) = delete;
	
		Scene& ReadAndLoadLevel(const std::string& file);
		
		void WriteHighscores(const std::string& path, std::vector<int>& scores);
		std::vector<int> ReadHighscores(const std::string& path);
	private:
		std::string m_BasePath{ "" };
	};
}

