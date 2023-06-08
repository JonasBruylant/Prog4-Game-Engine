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

#include "Scene.h"
#include "ImageObjectComponent.h"
#include "ImageRenderComponent.h"
#include "CollisionComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::JsonLevelReader::JsonLevelReader()
{
}

void dae::JsonLevelReader::ReadAndLoadLevel(std::string file)
{
	auto filePath = m_BasePath + file;
	if (std::ifstream is{ filePath }) //Will check if specified Input path exists, otherwise do nothing/throw exception
	{
		rapidjson::IStreamWrapper isw{ is };

		rapidjson::Document jsonDoc;			 
		jsonDoc.ParseStream(isw);	


		const rapidjson::Value& levelName = jsonDoc["LevelName"];
		const rapidjson::Value& blockIDs = jsonDoc["Level"];
		const rapidjson::Value& platformWidth = jsonDoc["PlatformWidth"];
		const rapidjson::Value& ladderHeight = jsonDoc["LadderHeight"];

		int actualPlatformWidth = platformWidth.GetInt();
		int actualLadderHeight = ladderHeight.GetInt();



		if (!blockIDs.IsArray())
			return;

		int rowCounter = 1;
		auto& sceneLevel = SceneManager::GetInstance().CreateScene(levelName.GetString());
		auto& resourceManager = ResourceManager::GetInstance();
		auto level = std::make_shared<GameObject>();
		for (auto& blockId : blockIDs.GetArray())
		{
			if (!blockId.IsArray())
				continue;

			for (int i = 0; i < static_cast<int>(blockId.GetArray().Size()); ++i)
			{
				//std::cout << currentId.GetInt() << ' ';
				//Make object based on ID here.
				if (blockId[i] == 0)
					continue;

				auto go = std::make_shared<GameObject>();
				go->Initialize();
				sceneLevel.Add(go);


				auto imageObjComp = go->AddComponent<ImageObjectComponent>();
				go->AddComponent<ImageRenderComponent>();
				auto collisionComponent = go->AddComponent<CollisionComponent>();
				auto texture = resourceManager.LoadTexture("Platform.png");
				
				if (blockId[i] == 1)
				{
					texture = resourceManager.LoadTexture("Platform.png");
					collisionComponent->SetTag("LevelPlatform");
					imageObjComp->SetTexture(texture);
				}
				else if (blockId[i] == 2)
				{
					texture = resourceManager.LoadTexture("Slab.png");
					imageObjComp->SetTexture(texture);

					collisionComponent->SetTag("LevelPlatform");
				}
				else if (blockId[i] == 3)
				{
					texture = resourceManager.LoadTexture("Ladder.png");
					imageObjComp->SetTexture(texture);

					collisionComponent->SetTag("Ladder");
				}
				go->GetTransform()->SetLocalPosition({ actualPlatformWidth * i, actualLadderHeight * rowCounter, 0});

				level->AddChild(go);
			}
			++rowCounter;
			//std::cout << std::endl;
		}



	}											 
}
