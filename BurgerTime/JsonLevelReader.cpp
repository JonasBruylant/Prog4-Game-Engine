#include "JsonLevelReader.h"

#pragma warning(push)
#pragma warning(disable : 5232)
#pragma warning(disable : 26451)
#pragma warning(disable : 26495)
#pragma warning(disable : 33010)
#include "rapidjson.h"
#include "document.h"
#include "stream.h"
#include "filereadstream.h"
#include "IStreamWrapper.h"
#pragma warning(pop)


#include <iostream>
#include <fstream>
#include <cassert>

#include "Scene.h"
#include "ImageObjectComponent.h"
#include "ImageRenderComponent.h"
#include "CollisionComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::JsonLevelReader::JsonLevelReader()
{
}

dae::Scene& dae::JsonLevelReader::ReadAndLoadLevel(const std::string& file)
{
	auto filePath = m_BasePath + file;
	std::ifstream is{ filePath };

	assert(!is.fail() && "File not found.");

	rapidjson::IStreamWrapper isw{ is };

	rapidjson::Document jsonDoc;			 
	jsonDoc.ParseStream(isw);	


	const rapidjson::Value& levelName = jsonDoc["LevelName"];
	const rapidjson::Value& blockIDs = jsonDoc["Level"];
	const rapidjson::Value& platformWidth = jsonDoc["PlatformWidth"];
	const rapidjson::Value& ladderHeight = jsonDoc["LadderHeight"];
	const rapidjson::Value& platformHeight = jsonDoc["PlatformHeight"];

	int actualPlatformWidth = platformWidth.GetInt();
	int actualPlatformHeight = platformHeight.GetInt();
	int actualLadderHeight = ladderHeight.GetInt();


	assert(blockIDs.IsArray() && "Level has no content.");

	auto& sceneLevel = SceneManager::GetInstance().CreateScene(levelName.GetString());
	auto& resourceManager = ResourceManager::GetInstance();
	auto level = std::make_shared<GameObject>();
	int totalHeightOffset{ 50 };
	int initialWidthOffset{ 20 };
	for (int row = 0; row < static_cast<int>(blockIDs.GetArray().Size()); ++row)
	{
		if (!blockIDs[row].IsArray())
			continue;

		int heightOffset{ (row % 2 == 0) ? actualPlatformHeight : actualLadderHeight };
		for (int col = 0; col < static_cast<int>(blockIDs[row].GetArray().Size()); ++col)
		{
			//std::cout << currentId.GetInt() << ' ';
			//Make object based on ID here.
			if (blockIDs[row][col] == 0)
				continue;

			auto go = std::make_shared<GameObject>();
			go->Initialize();
			sceneLevel.Add(go);


			auto imageObjComp = go->AddComponent<ImageObjectComponent>();
			go->AddComponent<ImageRenderComponent>();
			auto collisionComponent = go->AddComponent<CollisionComponent>();
			collisionComponent->SetMeasurements(static_cast<float>(actualPlatformWidth), static_cast<float>(heightOffset));
			auto texture = resourceManager.LoadTexture("Platform.png");
			
			if (blockIDs[row][col] == 1)
			{
				texture = resourceManager.LoadTexture("Platform.png");
				collisionComponent->SetTag("LevelPlatform");
				imageObjComp->SetTexture(texture);

			}
			if (blockIDs[row][col] == 2)
			{
				texture = resourceManager.LoadTexture("Slab.png");
				imageObjComp->SetTexture(texture);

				collisionComponent->SetTag("LevelPlatform");

			}
			go->GetTransform()->SetLocalPosition({ initialWidthOffset + (actualPlatformWidth * col), totalHeightOffset , 0 });
			if (blockIDs[row][col] == 3)
			{
				texture = resourceManager.LoadTexture("Ladder.png");
				collisionComponent->SetDebugColor({145,245,18});
				imageObjComp->SetTexture(texture);
				collisionComponent->SetMeasurements(static_cast<float>(actualPlatformWidth), static_cast<float>(heightOffset + 10));
				collisionComponent->SetTag("Ladder");
				go->GetTransform()->SetLocalPosition({ initialWidthOffset + (actualPlatformWidth * col), totalHeightOffset - 10 , 0});
			}

			level->AddChild(go);
		}
		totalHeightOffset += heightOffset;
		//std::cout << std::endl;
	}

	return sceneLevel;
}
