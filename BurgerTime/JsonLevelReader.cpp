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
	const rapidjson::Value& playerHeight = jsonDoc["PlayerHeight"];

	int actualPlatformWidth = platformWidth.GetInt();
	int actualPlatformHeight = platformHeight.GetInt();
	int actualLadderHeight = ladderHeight.GetInt();
	int actualPlayerHeight = playerHeight.GetInt();



	assert(blockIDs.IsArray() && "Level has no content.");

	auto& sceneLevel = SceneManager::GetInstance().CreateScene(levelName.GetString());
	auto& resourceManager = ResourceManager::GetInstance();
	auto level = std::make_shared<GameObject>();
	level->Initialize();
	

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
			auto collisionComponent = go->AddComponent<CollisionComponent>();
			collisionComponent->SetMeasurements(static_cast<float>(actualPlatformWidth), static_cast<float>(heightOffset));
			auto texture = resourceManager.LoadTexture("Platform.png");

			if (blockIDs[row][col] == 1)
			{
				texture = resourceManager.LoadTexture("Platform.png");
				imageObjComp->SetTexture(texture);
				collisionComponent->SetTag("LevelPlatform");

			}
			if (blockIDs[row][col] == 2)
			{
				texture = resourceManager.LoadTexture("Slab.png");
				imageObjComp->SetTexture(texture);

			}
			go->GetTransform()->SetLocalPosition({ initialWidthOffset + (actualPlatformWidth * col), totalHeightOffset , 0 });
			if (blockIDs[row][col] == 3)
			{
				texture = resourceManager.LoadTexture("Ladder.png");
				imageObjComp->SetTexture(texture);

				//Ladder Middle

				collisionComponent->SetMeasurements(static_cast<float>(actualPlatformWidth), static_cast<float>(heightOffset));

				collisionComponent->SetDebugColor({ 145,245,18 });
				collisionComponent->SetMeasurements(static_cast<float>(actualPlatformWidth), static_cast<float>(heightOffset));
				collisionComponent->SetTag("Ladder");
				go->GetTransform()->SetLocalPosition({ initialWidthOffset + (actualPlatformWidth * col), totalHeightOffset, 0 });


				auto ladderTop = std::make_shared<GameObject>();
				ladderTop->Initialize();
				ladderTop->SetParent(go.get(), false);

				collisionComponent = ladderTop->AddComponent<CollisionComponent>();
				collisionComponent->SetMeasurements(static_cast<float>(actualPlatformWidth), static_cast<float>(heightOffset));

				collisionComponent->SetDebugColor({ 132,235,58 });
				collisionComponent->SetMeasurements(static_cast<float>(actualPlatformWidth), static_cast<float>(actualPlatformHeight));
				collisionComponent->SetTag("LadderTop");
				collisionComponent->SetDrawDebug(true);
				ladderTop->GetTransform()->SetLocalPosition({ 0, -actualPlayerHeight , 0 });
				//actualPlayerHeight
			}
			go->AddComponent<ImageRenderComponent>();
		}
		totalHeightOffset += heightOffset;
		//std::cout << std::endl;

	}

	return sceneLevel;
}
