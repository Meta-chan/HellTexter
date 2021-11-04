#include <Windows.h>
#include <string.h>
#include <stdio.h>

class WindowClass
{
private:
	HINSTANCE _hinstance;
public:
	WindowClass(HINSTANCE hinstance) : _hinstance(hinstance) { }
	~WindowClass() { UnregisterClassW((sizeof(void*) == 8) ? L"helltexter" : L"helltexter32", _hinstance); }
};

class WindowsLibrary
{
private:
	HINSTANCE _hinstance;
public:
	WindowsLibrary(HINSTANCE hinstance) : _hinstance(hinstance) { }
	~WindowsLibrary() { FreeLibrary(_hinstance); }
};

class WindowsHook
{
private:
	HHOOK _hook;
public:
	WindowsHook(HHOOK hook) : _hook(hook) { }
	~WindowsHook() { UnhookWindowsHookEx(_hook); }
};

LRESULT CALLBACK window_handler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK window_closer(HWND hwnd, LPARAM lParam)
{
	wchar_t name[64];
	GetClassNameW(hwnd, name, 64);
	if (wcscmp(name, (sizeof(void*) == 8) ? L"helltexter" : L"helltexter32") == 0) SendMessage(hwnd, WM_CLOSE, 0, 0);
	return TRUE;
}

int _main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	//Close old windows
	EnumWindows(window_closer, 0);
	
	//Start 32-bit process
	if (sizeof(void*) == 8)
	{
		STARTUPINFOW si;
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
		wchar_t name[64];
		wcscpy_s(name, L"helltexter32");
		CreateProcessW(NULL, name, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	}
	
	//Register class
	WNDCLASSEXW window_class = {};
	window_class.cbSize = sizeof(WNDCLASSEXW);
	window_class.lpfnWndProc = window_handler;
	window_class.hInstance = hInstance;
	window_class.lpszClassName = (sizeof(void*) == 8) ? L"helltexter" : L"helltexter32";
	if (RegisterClassExW(&window_class) == 0) return 1;
	WindowClass class_object(hInstance);

	//Create window
	HWND window = CreateWindowExW(0, window_class.lpszClassName, (sizeof(void*) == 8) ? L"HellTexter" : L"HellTexter32",
	0, CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, NULL, NULL, hInstance, NULL);
	if (window == NULL) return 2;
	ShowWindow(window, SW_HIDE);

	//Load hook library
	HINSTANCE hook_library = LoadLibraryW((sizeof(void*) == 8) ? L"hook.dll" : L"hook32.dll");
	if (hook_library == NULL) return 3;
	WindowsLibrary library_object(hook_library);

	//Find hook address
	HOOKPROC hook_address = (HOOKPROC)GetProcAddress(hook_library, (sizeof(void*) == 8) ? "hook" : "_hook@12");
	if (hook_address == NULL) return 4;

	//Set hook
	HHOOK hook = SetWindowsHookExW(WH_GETMESSAGE, hook_address, hook_library, 0);
	if (hook == NULL) return 5;
	WindowsHook hook_object(hook);

	//Enter window loop
	MSG message = { };
	while (GetMessageW(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return _main(hInstance, hPrevInstance, pCmdLine, nCmdShow);
}