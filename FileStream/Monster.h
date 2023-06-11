#pragma once
#include <fstream>
#include <iostream>

class Monster
{
private:
	std::string mNAME;
	int mLEVEL;
	int mHP;
	int mMP;

public:
	Monster() :mNAME{}, mLEVEL{}, mHP{}, mMP{}
	{

	}
	Monster(std::string name, int level, int hp, int mp) :mNAME{ name }, mLEVEL{ level }, mHP{ hp }, mMP{ mp }
	{

	}
	std::ifstream& ReadBinary(std::ifstream& istream);
	std::ofstream& WriteBinary(std::ofstream& ostream);
};

std::ifstream& Monster::ReadBinary(std::ifstream& istream)
{
    int strLen{};
    istream.read(reinterpret_cast<char*>(&strlen), sizeof(int));
    mNAME.resize(strLen);
    istream.read(&mNAME[0], strLen);
    istream.read(reinterpret_cast<char*>(&mLEVEL), sizeof(int));
    istream.read(reinterpret_cast<char*>(&mHP), sizeof(int));
    istream.read(reinterpret_cast<char*>(&mMP), sizeof(int));
    return istream;

}

std::ofstream& Monster::WriteBinary(std::ofstream& ostream)
{
    int strLen = static_cast<int>(mNAME.size());
    ostream.write(reinterpret_cast<char*>(&strLen), sizeof(int));
    ostream.write(mNAME.c_str(), strLen);
    ostream.write(reinterpret_cast<char*>(&mLEVEL), sizeof(int));
    ostream.write(reinterpret_cast<char*>(&mHP), sizeof(int));
    ostream.write(reinterpret_cast<char*>(&mMP), sizeof(int));
    return ostream;

}

std::ifstream& operator>>(std::ifstream& istream, Monster& monster)
{
    return monster.ReadBinary(istream);
}
std::ofstream& operator<<(std::ofstream& ostream, Monster& monster)
{
    return monster.WriteBinary(ostream);
}