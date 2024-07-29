#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

enum {
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VOILET,
	DAKR_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};

// GDI+ 초기화
class GdiplusInit {
public:
	GdiplusInit() {
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~GdiplusInit() {
		GdiplusShutdown(gdiplusToken);
	}
private:
	ULONG_PTR gdiplusToken;
};

void SetColor(unsigned short text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

// 기본 프린터의 이름을 얻는 함수
wstring GetDefaultPrinterName() {
	DWORD size = 0;
	GetDefaultPrinter(NULL, &size);
	if (size == 0) return L"";

	wchar_t* buffer = new wchar_t[size];
	if (GetDefaultPrinter(buffer, &size)) {
		wstring printerName(buffer);
		delete[] buffer;
		return printerName;
	}
	else {
		delete[] buffer;
		return L"";
	}
}

void PrintImage(const wstring& filename, const wstring& printerName) {
	// 프린터 설정
	HDC hdc = CreateDC(L"WINSPOOL", printerName.c_str(), NULL, NULL);
	if (!hdc) {
		wcerr << L"DC를 만들지 못했습니다 : " << printerName << endl;
		return;
	}

	DOCINFO di;
	ZeroMemory(&di, sizeof(di));
	di.cbSize = sizeof(di);
	di.lpszDocName = L"Image Print";

	if (StartDoc(hdc, &di) > 0) {
		if (StartPage(hdc) > 0) {
			Graphics graphics(hdc);
			Image image(filename.c_str());
			if (image.GetLastStatus() == Ok) {
				graphics.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
			}
			else {
				wcerr << L"이미지를 불러오지 못했습니다 : " << filename << endl;
			}
			EndPage(hdc);
		}
		else {
			wcerr << L"시작에 실패했습니다." << endl;
		}
		EndDoc(hdc);
	}
	else {
		wcerr << L"프린터시작에 실패했습니다." << endl;
	}

	DeleteDC(hdc);
}

int main() {
	GdiplusInit gdiplus;
	SetConsoleTitle(L"디오시스템");
	SetColor(SKYBLUE);
	cout << "______________________________________________" << endl;
	cout << "                 디오시스템                    " << endl;
	SetColor(GREEN);
	cout << "                               Code By. 박 시율" << endl;
	SetColor(SKYBLUE);
	cout << "______________________________________________" << endl;
	cout << endl;
	SetColor(YELLOW);
	cout << "[알림] 잉크막힘을 방지하기위한 인쇄를 시작합니다." << endl;
	cout << "[알림] 이미지파일 로드중..." << endl;
	Sleep(100);

	// 미리 설정한 이미지 파일 경로들
	vector<wstring> filenames = {
		L"C:\\DIO-SYSTEM\\print1.jpg",
		L"C:\\DIO-SYSTEM\\print2.jpg",
		L"C:\\DIO-SYSTEM\\print3.jpg",
		L"C:\\DIO-SYSTEM\\print4.jpg",
		L"C:\\DIO-SYSTEM\\print5.jpg",
		L"C:\\DIO-SYSTEM\\print6.jpg",
		L"C:\\DIO-SYSTEM\\print7.jpg",
		L"C:\\DIO-SYSTEM\\print8.jpg",
		L"C:\\DIO-SYSTEM\\print9.jpg"
	};
	cout << "[알림] 기본프린터 로드중..." << endl;
	Sleep(100);

	// 기본 프린터 이름을 얻음
	wstring defaultPrinter = GetDefaultPrinterName();
	if (defaultPrinter.empty()) {
		wcerr << L"[경고] 기본프린터가 설정되어 있지 않습니다." << endl;
		return -1;
	}
	else {
		wcout << L"[알림] 기본 프린터 : " << defaultPrinter << endl;
	}

	for (const auto& filename : filenames) {
		wcout << L"[알림] 프린팅 : " << filename << endl;
		PrintImage(filename, defaultPrinter);
	}
	SetColor(GREEN);
	cout << "[알림] 인쇄완료 !! 5초뒤 자동으로 꺼집니다.";
	Sleep(3000);

	return 0;
}
