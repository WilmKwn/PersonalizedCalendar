#pragma once

#include "Tile.h"
#include "Graphics.h"

#include <gdiplus.h>

class Calendar
{
public:
	Calendar(Graphics *graphics, std::pair<int, int> dimensions, HWND window);
	~Calendar();

	void TilesInit();

	void Update();
	void SaveToFile(int month);
	void LoadFromFile(int month);
	void DeleteAll();

	void TakeScreenshot();
	void ChangeBackground();

	void GetEncoderClsid(const WCHAR* format, CLSID *clsid);

	static std::pair<int, int> GetFocus() { return focus; }
	static void SetFocus(int x, int y) { focusCoor = { x,y }; }
	static void SetText(std::string msg) {
		if (msg == "###") text = msg = "";
		else if (msg == "<-" && text != "") text.erase(text.end() - 1);
		else if (msg != "<-") text.append(msg); 
	}
	static void SetSave(bool state) { save = state; }
	static void SetDeleteAll(bool state) { deleteAll = state; }
	static void SetTakeScreenshot(bool state) { takeScreenshot = state; }
	static void SetMoveMonth(int num) { moveMonth = num; }

private:
	Graphics *graphics;
	std::pair<int, int> dimensions;
	HWND window;

	D2D1_COLOR_F textColor;
	D2D1_COLOR_F dateColor;
	D2D1_COLOR_F focusColor;
	D2D1_COLOR_F dayOfWeekColor;
	D2D1_COLOR_F lineColor;

	int originalMonth;
	int currentMonth;
	int currentDay;

	int daysPerMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int offsetPerMonth[12] = { 0, 0, 0, 0, 0, 3, 5, 1, 4, 6, 2, 4 };
	std::string months[12] = { "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

	Tile *tiles[5][7];
	Tile *daysOfWeek[7];

	std::pair<int, int> todayIndex;

	static std::pair<int, int> focusCoor;
	static std::string text;

	static std::pair<int, int> focus;
	std::pair<int, int> focusPrev;

	static int moveMonth;

	static bool deleteAll;

	static bool takeScreenshot;

	static bool save;
	std::ofstream writeFile;
	std::ifstream readFile;

	char path[256];
	std::string basePath;
	std::string monthFile;
	std::string screenshotFile;
};