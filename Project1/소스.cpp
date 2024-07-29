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

// GDI+ �ʱ�ȭ
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

// �⺻ �������� �̸��� ��� �Լ�
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
	// ������ ����
	HDC hdc = CreateDC(L"WINSPOOL", printerName.c_str(), NULL, NULL);
	if (!hdc) {
		wcerr << L"DC�� ������ ���߽��ϴ� : " << printerName << endl;
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
				wcerr << L"�̹����� �ҷ����� ���߽��ϴ� : " << filename << endl;
			}
			EndPage(hdc);
		}
		else {
			wcerr << L"���ۿ� �����߽��ϴ�." << endl;
		}
		EndDoc(hdc);
	}
	else {
		wcerr << L"�����ͽ��ۿ� �����߽��ϴ�." << endl;
	}

	DeleteDC(hdc);
}

int main() {
	GdiplusInit gdiplus;
	SetConsoleTitle(L"����ý���");
	SetColor(SKYBLUE);
	cout << "______________________________________________" << endl;
	cout << "                 ����ý���                    " << endl;
	SetColor(GREEN);
	cout << "                               Code By. �� ����" << endl;
	SetColor(SKYBLUE);
	cout << "______________________________________________" << endl;
	cout << endl;
	SetColor(YELLOW);
	cout << "[�˸�] ��ũ������ �����ϱ����� �μ⸦ �����մϴ�." << endl;
	cout << "[�˸�] �̹������� �ε���..." << endl;
	Sleep(100);

	// �̸� ������ �̹��� ���� ��ε�
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
	cout << "[�˸�] �⺻������ �ε���..." << endl;
	Sleep(100);

	// �⺻ ������ �̸��� ����
	wstring defaultPrinter = GetDefaultPrinterName();
	if (defaultPrinter.empty()) {
		wcerr << L"[���] �⺻�����Ͱ� �����Ǿ� ���� �ʽ��ϴ�." << endl;
		return -1;
	}
	else {
		wcout << L"[�˸�] �⺻ ������ : " << defaultPrinter << endl;
	}

	for (const auto& filename : filenames) {
		wcout << L"[�˸�] ������ : " << filename << endl;
		PrintImage(filename, defaultPrinter);
	}
	SetColor(GREEN);
	cout << "[�˸�] �μ�Ϸ� !! 5�ʵ� �ڵ����� �����ϴ�.";
	Sleep(3000);

	return 0;
}
