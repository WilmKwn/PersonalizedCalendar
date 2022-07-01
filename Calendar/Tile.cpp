#include "Tile.h"

Tile::Tile(std::pair<int, int> pos, std::pair<int, int> size)
{
	x = pos.first;
	y = pos.second;
	w = size.first;
	h = size.second;

	day = 0;

	text = "";

	today = false;
}

Tile::~Tile()
{
}

std::pair<int, int> Tile::GetPos()
{
	return { x,y };
}

std::pair<int, int> Tile::GetSize()
{
	return { w,h };
}

std::string Tile::GetText()
{
	return text;
}

int Tile::GetDay()
{
	return day;
}

void Tile::SetDay(int day)
{
	this->day = day;
}

void Tile::SetText(std::string text)
{
	this->text = text;
}

void Tile::SetToday(bool state)
{
	today = state;
}

bool Tile::GetToday()
{
	return today;
}