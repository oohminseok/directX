#include <iostream>
#include <regex>
#include <vector>
#include <fstream>
#include "monsters.h"
#include "tinyxml2.h"

using namespace tinyxml2;

bool LoadFromXML(const std::string& filename, std::vector<monsters::monsters>& monsters)
{
	XMLDocument doc;

	if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
	{
		std::cerr << "파일을 열지 못했습니다.";
		return false;
	}

	auto pRoot = doc.FirstChildElement("monsters");

	for (auto pMonster = pRoot->FirstChildElement(); pMonster; pMonster = pMonster->NextSiblingElement())
	{
		monsters::monsters monster;

		monster.SetName(pMonster->Attribute("name"));

		auto pStatus = pMonster->FirstChildElement("status");
		monsters::Status status
		{
			pStatus->IntAttribute("level"),
			pStatus->IntAttribute("hp"),
			pStatus->IntAttribute("mp")
		};
		monster.SetStatus(status);

		auto pItems = pMonster->FirstChildElement("items");
		for (auto pItem = pItems->FirstChildElement(); pItem;
			pItem = pItem->NextSiblingElement())
		{
			monsters::Item item{
				pItem->Attribute("name"),
				pItem->IntAttribute("gold")
			};

			monster.AddDropItem(item);
		}
		monsters.push_back(monster);
	}
	return true;
}



bool SaveToXML(const std::string& filename, std::vector<monsters::monsters>& monster)
{
	XMLDocument doc;

	auto pHeader=doc.NewDeclaration(R"(xml version="1.0" encoding="EUC-KR)");
	doc.LinkEndChild(pHeader);
	
	auto pRoot = doc.NewElement("monsters");
	for (auto monsters : monster)
	{
		auto pMonster = doc.NewElement("monster");
		pMonster->SetAttribute("name", monsters.GetName().c_str());

		auto pStatus = doc.NewElement("status");
		pMonster->LinkEndChild(pStatus);
		auto status = monsters.GetStatus();
		pStatus->SetAttribute("level", status.mLevel);
		pStatus->SetAttribute("level", status.mHP);
		pStatus->SetAttribute("level", status.mMP);
		pRoot->LinkEndChild(pMonster);
		
		auto pItems = doc.NewElement("items");
		for (auto item : monsters.GetItems())
		{
			auto pItem = doc.NewElement("item");
			pItem->SetAttribute("name", item.mName.c_str());
			pItem->SetAttribute("gold",item.mGold);
			pItems->LinkEndChild(pItem);
		}
		pMonster->LinkEndChild(pItems);
	}
	doc.LinkEndChild(pRoot);

	return doc.SaveFile(filename.c_str()) == XML_SUCCESS;

}


int main()
{
	std::vector<monsters::monsters> monsters;
	
	LoadFromXML("Data/mydata.xml", monsters);
	SaveToXML("Data/mydata2.xml", monsters);

}