#include <vector>
#include <sstream>
#include "Monster.h"

struct Header
{
    int version{ 1 };
    int itemCount{};
};

bool SaveToFile(const char* filename, std::vector<Monster>& monsters)
{
    std::ofstream ostream;
    ostream.exceptions(std::ofstream::badbit|std::ofstream::failbit);

    try
    {
        ostream.open(filename, std::ostream::binary);

        Header header{ 1,static_cast<int>(monsters.size()) };
        ostream.write(reinterpret_cast<char*>(&header), sizeof(Header));
        for (auto item : monsters)
        {
            ostream << item;
        }
        ostream.close();
    }
    catch(std::ofstream::failure e)
    {
        std::cerr << "파일 저장 중에 예외가 발생했습니다."
            << e.what() << std::endl;
        ostream.close();
        return false;
    }
    return true;
}

bool LoadFromFile(const char* filename, std::vector<Monster>& monsters)
{
    std::ifstream istream;
    istream.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try
    {
        istream.open(filename,std::ifstream::binary);

        Header header;
        istream.read(reinterpret_cast<char*>(&header), sizeof(Header));
        for (int i = 0; i < header.itemCount; ++i)
        {
            Monster monster;
            istream >> monster;
            monsters.push_back(monster);
        }
        istream.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "파일 저장 중에 예외가 발생했습니다."
            << e.what() << std::endl;
        istream.close();
        return false;
    }
    return true;
 

}

int main()
{
    std::vector<Monster> monsterList;
    monsterList.push_back(Monster{ "JELLY",1,1,1 });
    monsterList.push_back(Monster{ "WOLF",1,1,1 });
    monsterList.push_back(Monster{ "DEMON",1,1,1 });    

    SaveToFile("Data/SimpleData.dat", monsterList);
    monsterList.clear();
    LoadFromFile("Data/SimpleData.dat", monsterList);


}

