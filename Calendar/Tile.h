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
	
	void SetDay(int day);
	int GetDay();

	void SetText(std::string text);
	std::string GetText();

	void SetToday(bool state);
	bool GetToday();

private:
	int x, y;
	int w, h;

	int day;

	std::string text;

	bool today;
};