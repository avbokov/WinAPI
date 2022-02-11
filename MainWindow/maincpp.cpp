#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include"resource.h"


CONST CHAR g_szClassName[] = "MyWindowClass";
CONST CHAR g_szTitle[] = "My First Window";
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	// 1) Регистрация класса окна: (12 переменных)
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = WndProc;
	wc.style = 0;
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_EARTH));		// панель задач (из ресурса)
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_LIGHTNING));	// основное окно (из ресурса)
	wc.hIcon = (HICON)LoadImage(hInstance, "umbrella.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);	// панель задач (из файла)
	wc.hIconSm = (HICON)LoadImage(hInstance, "school.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);	// основное окно (из файла)
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hInstance = hInstance;
	wc.lpszMenuName = NULL; // меня в этом окне не будет
	wc.lpszClassName = g_szClassName;
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
	}

	// 2) Создание окна:
	INT screen_width = GetSystemMetrics(SM_CXSCREEN);	// вычисляем размер экрана
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);	// вычисляем размер экрана
	INT window_width = screen_width - screen_width / 4;
	INT window_height = screen_height - screen_height / 4;
	INT window_start_x = screen_width / 8;
	INT window_start_y = screen_height / 8;

	HWND hwnd = CreateWindowEx
	(
		WS_EX_CLIENTEDGE,
		g_szClassName,			// имя класса окна
		g_szTitle,			// заголовок окна
		WS_OVERLAPPEDWINDOW,	// для главного окна программы всегда задаётся такой стиль
		window_start_x, window_start_y,	// начальная позиция окна на экране
		window_width, window_height,	// исходный размер окна
		NULL,							// родительское окно отсутсвует
		NULL,							// меня тоже отсутсвует
		hInstance,						// EXE-файл окна
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);		// задаём режим отображения окна (развёрнуто на весь экранб свёрнуто на панель задач)
	UpdateWindow(hwnd);				// прорисовка окна

	// 3) Запсук цикла сообщений:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		break;
	case WM_SIZE:
	case WM_MOVE:
	{
		CONST INT SIZE = 256;
		CHAR buffer[SIZE] = {};
		RECT rect;	// прямоугольник
		GetWindowRect(hwnd, &rect);
		int window_start_x = rect.left;
		int window_start_y = rect.top;
		int window_width = rect.right - rect.left;
		int window_heigth = rect.bottom - rect.top;
		sprintf(buffer, "%s - position: %dx%d, size: %dx%d", g_szTitle, window_start_x, window_start_y, window_width, window_heigth);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)buffer);
	}
	break;
	case WM_CLOSE:
		if (MessageBox(hwnd, "Вы действительно хотите выйти?", "Question", MB_YESNO | MB_ICONQUESTION) == IDYES)
			DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
