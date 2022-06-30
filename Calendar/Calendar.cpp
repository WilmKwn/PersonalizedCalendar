#include "Calendar.h"

std::pair<int, int> Calendar::focusCoor = { -1, -1 };
std::pair<int, int> Calendar::focus = { -1, -1 };
std::string Calendar::text = "";
bool Calendar::save = false;
bool Calendar::deleteAll = false;
bool Calendar::takeScreenshot = false;

Calendar::Calendar(Graphics *graphics, std::pair<int, int> dimensions, HWND window)
{
	this->graphics = graphics;
	this->dimensions = dimensions;
	this->window = window;

	textColor = D2D1::ColorF(0.6f, 0.6f, 0.6f);
	lineColor = D2D1::ColorF(0.0f, 0.7f, 0.0f);
	focusColor = D2D1::ColorF(0.2f, 0.2f, 0.2f);
	dateColor = D2D1::ColorF(0.8f, 0.8f, 0.8f);
	dayOfWeekColor = D2D1::ColorF(1.0f, 0.7f, 0.0f);

	GetModuleFileName(NULL, path, sizeof(path));
	basePath = path;
	basePath = basePath.substr(0, basePath.size() - 12);
	
	contentFile = basePath + "content.txt";
	monthFile = basePath + "month.txt";
	screenshotFile = basePath + "picture.png";

	SYSTEMTIME date = { 0 };
	GetLocalTime(&date);
	currentMonth = date.wMonth;
	currentDay = date.wDay;

	for (int i = 0; i < ARRAYSIZE(daysOfWeek); i++) {
		int w = dimensions.first / 7;
		int h = dimensions.second / 20;
		daysOfWeek[i] = new Tile({i*w, 0}, {(i+1)*w, h});
	}
	daysOfWeek[0]->SetText("SUN");
	daysOfWeek[1]->SetText("MON");
	daysOfWeek[2]->SetText("TUE");
	daysOfWeek[3]->SetText("WED");
	daysOfWeek[4]->SetText("THU");
	daysOfWeek[5]->SetText("FRI");
	daysOfWeek[6]->SetText("SAT");

	for (int i = 0; i < ARRAYSIZE(tiles); i++) {
		for (int j = 0; j < ARRAYSIZE(tiles[0]); j++) {
			int space = dimensions.second / 20;
			int w = dimensions.first / 7;
			int h = (dimensions.second - space) / 5;

			tiles[i][j] = new Tile({j*w, (i*h)+space}, {w, h+space});

			if (i != 0 || j > offsetPerMonth[currentMonth - 1]-1) {
				int day = i * 7 + (j + 1) - offsetPerMonth[currentMonth - 1];
				tiles[i][j]->SetDay(day > daysPerMonth[currentMonth - 1] ? 0 : day);
			}
		}
	}
	Load();
}

Calendar::~Calendar()
{
}

void Calendar::Update()
{
	for (int i = 0; i < ARRAYSIZE(daysOfWeek); i++) {
		int w = dimensions.first / 7;
		int h = dimensions.second / 20;
		int x = daysOfWeek[i]->GetPos().first;
		int y = daysOfWeek[i]->GetPos().second;
		graphics->WriteText(daysOfWeek[i]->GetText(), x + (w/3), y + (h/4), w, h, dayOfWeekColor);
	}

	int space = dimensions.second / 20;
	int w = dimensions.first / 7;
	int h = (dimensions.second - space) / 5;

	if (focusCoor.first >= 0 && focusCoor.second >= 0) {
		focus = { focusCoor.first / w, (focusCoor.second - space) / h };
		graphics->DrawRect({ focus.first*w, focus.second*h + space }, { (focus.first + 1)*w, (focus.second + 1)*h + space }, focusColor);

		if (focus.first != focusPrev.first || focus.second != focusPrev.second) text = tiles[focus.second][focus.first]->GetText();
		tiles[focus.second][focus.first]->SetText(text);
	}
	focusPrev = focus;

	for (int i = 0; i < ARRAYSIZE(tiles); i++) {
		for (int j = 0; j < ARRAYSIZE(tiles[0]); j++) {
			graphics->DrawLine({ j*w, i*h + space }, { j*w, (i+1)*h + space }, lineColor);
			graphics->DrawLine({ j*w, i*h + space }, { (j+1)*w, i*h + space }, lineColor);

			int x = tiles[i][j]->GetPos().first;
			int y = tiles[i][j]->GetPos().second;
			int margin = w / 20;
			graphics->WriteText(tiles[i][j]->GetText(), x+margin, y+margin*3, w-margin*1.5, h-margin*3, textColor);

			int dW = w / 1;
			int dH = h / 1;
			int dX = x + w - dW;
			int dY = y;
			graphics->WriteText(tiles[i][j]->GetDay() == 0 ? "" : std::to_string(tiles[i][j]->GetDay()), dX + margin/2, dY + margin/2, dW, dH, dateColor);
		}
	}

	if (save) {
		Save();
		MessageBox(NULL, "SAVED", "", NULL);
		save = false;
	}

	if (deleteAll) {
		DeleteAll();
		MessageBox(NULL, "CLEARED", "", NULL);
		deleteAll = false;
	}
	
	if (takeScreenshot) {
		TakeScreenshot();
		ChangeBackground();
		MessageBox(NULL, "WALLPAPER UPDATED", "", NULL);
		takeScreenshot = false;
	}
}

void Calendar::Save()
{
	writeFile.open(contentFile);
	for (int i = 0; i < ARRAYSIZE(tiles); i++) {
		for (int j = 0; j < ARRAYSIZE(tiles[0]); j++) {
			writeFile << tiles[i][j]->GetDay() << "|" << tiles[i][j]->GetText() << "\n";
		}
	}
	writeFile.close();

	writeFile.open(monthFile);
	writeFile << currentMonth;
	writeFile.close();
}

void Calendar::Load()
{
	readFile.open(monthFile);
	int month;
	readFile >> month;
	readFile.close();
	
	if (month == currentMonth) {
		readFile.open(contentFile);
		std::string line;
		for (int i = 0; i < ARRAYSIZE(tiles); i++) {
			for (int j = 0; j < ARRAYSIZE(tiles[0]); j++) {
				std::getline(readFile, line);
				int index = line.find("|");
				tiles[i][j]->SetDay(std::stoi(line.substr(0, index)));
				tiles[i][j]->SetText(line.substr(index + 1));
			}
		}
		readFile.close();
	}
}

void Calendar::DeleteAll()
{
	for (int i = 0; i < ARRAYSIZE(tiles); i++) {
		for (int j = 0; j < ARRAYSIZE(tiles[0]); j++) {
			tiles[i][j]->SetText("");
		}
	}
}

void Calendar::ChangeBackground()
{
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)screenshotFile.c_str(), SPIF_SENDCHANGE);
}

void Calendar::TakeScreenshot()
{
	int w = dimensions.first;
	int h = dimensions.second;

	HDC screen = GetDC(window);
	HDC hdc = CreateCompatibleDC(screen);
	HBITMAP bitmap = CreateCompatibleBitmap(screen, w, h);

	SelectObject(hdc, bitmap);
	StretchBlt(hdc, 0, 0, w, h, screen, 0, 0, w, h, SRCCOPY);

	Gdiplus::GdiplusStartupInput startupInput;
	ULONG_PTR token;
	GdiplusStartup(&token, &startupInput, NULL);

	CLSID encoderID;
	GetEncoderClsid(L"image/png", &encoderID);

	Gdiplus::Bitmap *bmp = new Gdiplus::Bitmap(bitmap, (HPALETTE)0);
	bmp->Save(std::wstring(screenshotFile.begin(), screenshotFile.end()).c_str(), &encoderID, NULL);

	Gdiplus::GdiplusShutdown(token);

	DeleteObject(bitmap);
	DeleteObject(hdc);
	ReleaseDC(NULL, screen);
}

void Calendar::GetEncoderClsid(const WCHAR* format, CLSID *clsid)
{
	UINT num = 0, size = 0;
	Gdiplus::ImageCodecInfo *image = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);

	image = (Gdiplus::ImageCodecInfo*)(malloc(size));

	Gdiplus::GetImageEncoders(num, size, image);

	for (int i = 0; i < num; i++) {
		if (wcscmp(image[i].MimeType, format) == 0) {
			*clsid = image[i].Clsid;
			free(image);
		}
	}
}