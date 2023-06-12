#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include "monsters.h"

using namespace rapidjson;

bool LoadFromJson(const std::string& filename, std::vector<monsters::monsters>& monsters)
{
	std::ifstream ifs;
	ifs.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	std::stringstream ss;

	try
	{
		ifs.open(filename);
		ss << ifs.rdbuf();
		ifs.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "파일을 읽는 도중 문제가 발생했습니다." << std::endl;
		std::cerr << e.what();
		ifs.close();
		return false;
	}

	Document d;
	d.Parse(ss.str().c_str());
	for (auto& elem : d["monsters"].GetArray())
	{
		monsters::monsters monster;
		monster.SetName(elem["name"].GetString());

		monsters::Status status;
		status.mLevel = elem["status"].GetObject()["level"].GetInt();
		status.mHP = elem["status"].GetObject()["hp"].GetInt();
		status.mMP = elem["status"].GetObject()["mp"].GetInt();
		monster.SetStatus(status);
		for (auto& elem2 : elem["items"].GetArray())
		{
			monsters::Item item;
			item.mName = elem2["name"].GetString();
			item.mGold = elem2["gold"].GetInt();
			monster.AddDropItem(item);
		}
		monsters.push_back(monster);
	}
	return true;
}

bool SaveToJson(const std::string& filename, std::vector<monsters::monsters>& monsters)
{
	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);
	writer.StartObject();
	writer.Key("monsters");
	writer.StartArray();

	for (auto monster : monsters)
	{
		writer.StartObject();
		{
			writer.Key("name"); writer.String(monster.GetName().c_str());
			writer.Key("status");
			writer.StartObject();
			{
				writer.Key("level"); writer.Int(monster.GetStatus().mLevel);
				writer.Key("hp"); writer.Int(monster.GetStatus().mHP);
				writer.Key("mp"); writer.Int(monster.GetStatus().mMP);
			}
			writer.EndObject();
			writer.Key("items");
			writer.StartArray();
			{
				for (auto item : monster.GetItems())
				{
					writer.StartObject();
					writer.Key("name"); writer.String(item.mName.c_str());
					writer.Key("gold"); writer.Int(item.mGold);
					writer.EndObject();
				}
			}
			writer.EndArray();
		}
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();

	std::ofstream ofs;
	ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try 
	{
		ofs.open(filename);
		ofs << sb.GetString();
		ofs.close();
	}
	catch (std::ofstream::failure e)
	{
		std::cerr << "파일 저장중에 예외가 발생했습니다.\n";
		std::cerr << e.what();
		ofs.close();
		return false;
	}
	return true;
}


int main()
{
	std::vector<monsters::monsters>monsters;

	LoadFromJson("Data/mydata.json", monsters);

	SaveToJson("Data/mydata2.json", monsters);
}