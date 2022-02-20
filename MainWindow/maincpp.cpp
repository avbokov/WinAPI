#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<windowsx.h>
#include<stdio.h>
#include<CommCtrl.h>
#include"resource.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define IDC_STATIC_RESOLUTION	1000

TOOLINFO g_toolItem = { 0 };
HWND g_hwndTrackingTT;
BOOL g_TrackingMouse = FALSE;
TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };

CONST CHAR g_szClassName[] = "MyWindowClass";	//	имя класса. Объявили как глобальную константу (это не те классы, что в ООП).
CONST CHAR g_szTitle[] = "My First Window";
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//	объявление процедуры окна, но уже LRESULT, а не BOOL.
VOID CreateToolTipForRect(HWND hwnd);
HWND CreateTrackingTooltip(HWND hwnd);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	// 1) Регистрация класса окна: (12 переменных)
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = WndProc;
	wc.style = 0;
	//wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);								//	по умолчанию иконка на панели задач
	//wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);								//	по умолчанию иконка в заголовке основного окна
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_EARTH));			// панель задач (загрузка из ресурса)
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_LIGHTNING));	// заголовок основного окна (загрузка из ресурса)
	wc.hIcon = (HICON)LoadImage(hInstance, "umbrella.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);	// панель задач (загрузка из файла)
	wc.hIconSm = (HICON)LoadImage(hInstance, "school.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);	// заголовок основного окна (загрузка из файла)
	//wc.hCursor = LoadCursor(NULL, IDC_ARROW);									//	по умолчанию курсор окна
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));			//	функция LoadCursor() тоже использует ресурс
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hInstance = hInstance;
	wc.lpszMenuName = NULL;														// меню в этом окне не будет
	wc.lpszClassName = g_szClassName;											//	который объявили как глобальную константу вверху.

	if (!RegisterClassEx(&wc))	//	проверка на регистрацию окна.
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	// 2) Создание окна:
	INT screen_width = GetSystemMetrics(SM_CXSCREEN);	// вычисляем размер экрана
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);	// вычисляем размер экрана
	INT window_width = screen_width - screen_width / 4;
	INT window_height = screen_height - screen_height / 4;
	INT window_start_x = screen_width / 8;
	INT window_start_y = screen_height / 8;

	HWND hwnd = CreateWindowEx	//	CreateWindowEx() - создаёт окно и возвращает HWND этого окна, по которому к нему можно обратиться
	(
		WS_EX_CLIENTEDGE,					// стиль окна (?)
		g_szClassName,						// имя класса окна
		g_szTitle,							// заголовок окна
		WS_OVERLAPPEDWINDOW,				// для главного окна программы всегда задаётся такой стиль
		//CW_USEDEFAULT, CW_USEDEFAULT,		// начальная позиция окна на экране
		//CW_USEDEFAULT, CW_USEDEFAULT,		// исходный размер окна
		window_start_x, window_start_y,		// начальная позиция окна на экране
		window_width, window_height,		// исходный размер окна
		NULL,								// родительское окно отсутсвует
		NULL,								// меню тоже отсутсвует
		hInstance,							// EXE-файл окна
		NULL
	);
	if (hwnd == NULL)	//	проверка на создание окна.
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);		// задаём режим отображения окна (развёрнуто на весь экран, свёрнуто на панель задач и т.д.)
	UpdateWindow(hwnd);				// прорисовка окна

	// 3) Запуск цикла сообщений:
	//	Вот это всё ниже (включая return) пока просто запомнить.
	MSG msg;	
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		//	Преобразует сообщения виртуальных клавиш в символьные сообщения.
		//	Символьные сообщения отправляются в очередь сообщений вызывающего потока,
		//	чтобы быть прочитанными в следующий раз, когда поток вызовет функцию GetMessage или PeekMessage.

		DispatchMessage(&msg);
		//	Отправляет сообщение оконной процедуре. Обычно он используется для отправки сообщения, полученного функцией GetMessage.
	}

	return msg.wParam;
}

//	4 вхождения в такой процедуре (WM_CREATE, WM_COMMAND, WM_CLOSE, WM_DESTROY), в отличии от DIALOG.
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:	//	вместо INITDIALOG, тут будут создаваться разные элементы окна, инициализация проходить и всё такое
	{
		// ToolTip - всплывающая подсказка:
		InitCommonControls();
		//CreateToolTipForRect(hwnd);
		g_hwndTrackingTT = CreateTrackingTooltip(hwnd);
		return TRUE;
	}
	break;
	case WM_MOUSELEAVE:
	{
		SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
		g_TrackingMouse = FALSE;
		return FALSE;
	}
	break;
	case WM_MOUSEMOVE:
	{
		//CreateToolTipForRect(hwnd);
		//SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
		static int oldX, oldY;
		int newX, newY;
		if (!g_TrackingMouse)
		{
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hwnd;

			TrackMouseEvent(&tme);
			SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&g_toolItem);
			g_TrackingMouse = TRUE;
		}
		newX = GET_X_LPARAM(lParam);
		newY = GET_Y_LPARAM(lParam);

		if ((newX != oldX) || (newY != oldY))
		{
			oldX = newX;
			oldY = newY;

			CHAR coords[18]{};
			sprintf(coords, "%dx%d", newX, newY);
			g_toolItem.lpszText = coords;
			SendMessage(g_hwndTrackingTT, TTM_SETTOOLINFO, 0, (LPARAM)&g_toolItem);

			POINT pt = { newX, newY };
			ClientToScreen(hwnd, &pt);
			SendMessage(g_hwndTrackingTT, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x + 20, pt.y - 20));
		}
		SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&g_toolItem);
		return FALSE;
	}
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
			DestroyWindow(hwnd);	//	закрытие окна.
		break;
	case WM_DESTROY:
		PostQuitMessage(0);			//	закрытие окна.
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	//	функция DefWindowProc() вызывает оконную процедуру по умолчанию, чтобы обеспечить обработку по умолчанию любых оконных
	//	сообщений, которые приложение не обрабатывает. Эта функция гарантирует, что каждое сообщение будет обработано.
	//	DefWindowProc вызывается с теми же параметрами, что и оконная процедура.
	}
	return 0;
}

VOID CreateToolTipForRect(HWND hwnd)
{
	HWND hwndTT = CreateWindowEx
	(
		WS_EX_TOPMOST,
		TOOLTIPS_CLASS,
		"My first Tooltip",
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		// WS_POPUP - всплывающее окно
		// TTS_NOPREFIX - игнорировать символ &
		// TTS_ALWAYSTIP - подсказка появляется даже над неактивным окном
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hwnd,
		NULL,
		GetModuleHandle(NULL),	// Получаем HINSTANCE (EXE-файл) программы
		NULL
	);
	if (hwndTT == NULL)
	{
		MessageBox(hwnd, "Tooltip window creation failed", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	TOOLINFO ti;
	ZeroMemory(&ti, sizeof(TOOLINFO));
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hwnd;
	ti.hinst = GetModuleHandle(NULL);
	ti.lpszText = (LPSTR)"This is a tooltip";
	GetClientRect(hwnd, &ti.rect);
	SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
}

HWND CreateTrackingTooltip(HWND hwnd)
{
	HWND hwndTT = CreateWindowEx
	(
		WS_EX_TOPMOST,
		TOOLTIPS_CLASS,
		NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	if (hwndTT == NULL)
	{
		MessageBox(NULL, "Tooltip Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	g_toolItem.cbSize = sizeof(TOOLINFO);
	g_toolItem.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
	g_toolItem.hwnd = hwnd;
	g_toolItem.hinst = GetModuleHandle(NULL);
	g_toolItem.lpszText = (LPSTR)"Tracking tooltip";
	g_toolItem.uId = (UINT_PTR)hwnd;
	GetClientRect(hwnd, &g_toolItem.rect);
	SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&g_toolItem);
	return hwndTT;
}
