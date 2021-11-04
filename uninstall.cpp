#include <Windows.h>

BOOL CALLBACK window_closer(HWND hwnd, LPARAM lParam)
{
	wchar_t name[64];
	GetClassNameW(hwnd, name, 64);
	if (wcscmp(name, L"helltexter") == 0 || wcscmp(name, L"helltexter32") == 0) SendMessage(hwnd, WM_CLOSE, 0, 0);
	return TRUE;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	EnumWindows(window_closer, 0);
}