#include<Windows.h>
#include<stdio.h>	// подключаем для работы функции sprintf()
#include"resource.h"

CONST CHAR* words[] = //	массив указателей на CHAR (массив строк)
{
	"This", "is", "my", "first", "Combo", "Box"
};

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//	объявление процедуры окна (прототип)

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);	// вызываем наше окно
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));	//	создание иконки
			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);							//	установка иконки в окно

			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO1);	//	Получаем HWND (уникальный номер, дескриптор) элемента окна по его ResourceID
			for (int i = 0; i < sizeof(words) / sizeof(CHAR*); i++)
				SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)words[i]);
			//	В ComboBox IDC_COMBO1 всвойствах (Properties) настройку Sort ставим False, таким образом
			//	отключаем сортировку строк по алфавиту.
		}
		break;
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDOK:
			{
				CONST INT SIZE = 256;
				CHAR buffer[SIZE]{};
				INT cursel = SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETCURSEL, 0, 0);
				//	сообщение CB_GETCURSEL позволяет понять, какой Item (элемент) выбран, возвращает индекс (начиная с 0).
				//	Если ничего не выбрано, то возвращает CB_ERR.

				if (cursel != CB_ERR)
				{
					CHAR text[SIZE]{};
					SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETLBTEXT, cursel, (LPARAM)text);
					//	сообщение CB_GETLBTEXT - копирует текст по индексу cursel в (LPARAM)text. Работает только с COMBO_BOX !!!
					sprintf(buffer, "Вы выбрали элемент %d - %s", cursel, text);
					//	функция sprintf() - записывает данные в результирующую строку(в нашем случае buffer).
				}
				else
				{
					sprintf(buffer, "Вы ничего не выбрали");
				}
				MessageBox(hwnd, buffer, "Info", MB_OK | MB_ICONINFORMATION);
			}
			break;
			case IDCANCEL: EndDialog(hwnd, 0); break;
			}		
		}
		break;
		case WM_CLOSE:
			EndDialog(hwnd, 0);
	}
	return FALSE;
}
