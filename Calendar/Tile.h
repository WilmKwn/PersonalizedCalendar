#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

class Tile
{
public:
	Tile(std::pair<int,int> pos, std::pair<int,int> size);
	~Tile();

	std::pair<int,int> GetPos();
	std::pair<int,int> GetSize();
	std::string GetText();
	
	int GetDay();
	void SetDay(int day);

	void SetText(std::string text);

private:
	int x, y;
	int w, h;

	int day;

	std::string text;
};