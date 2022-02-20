#include<Windows.h>
#include"resource.h"	//	именно здесь объявление всех ресурсов, окон, иконок, кнопок

//			В WINAPI абсолютно всё является окном !!!
//			В WINAPI типы данных обзываются БОЛЬШИМИ БУКВАМИ !!! (так принято и это хороший тон)


CONST CHAR INVITATION_LOGIN[] = "Введите логин";
CONST CHAR INVITATION_PASSWORD[] = "Введите пароль";

//   CALLBACK - конвенция вызова (что это такое ???)
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//	Объявление процедура окна
//	Любая процедура окна принимает 4 параметра:
//	hwnd - окно
//	uMsg - сообщение
//	wParam - параметр сообщения
//	lParam - параметр сообщения

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT cCmdShow)
{
	/*MessageBox(FindWindow(NULL, "WinAPI - Microsoft Visual Studio"), "Привет! Это моё первое окно сообщения", "Hello World",
		MB_ICONWARNING | MB_YESNOCANCEL | MB_HELP | MB_DEFBUTTON3 | MB_SYSTEMMODAL);*/

	//	1 параметр - родительское окно
	//	2 параметр - текст сообщения
	//	3 параметр - заголовок окна
	//	4 параметр - всё, что касается кнопок (ФЛАГИ)
	//	Модальное окно (MODAL) блокирует доступ к родительскому окну, пока оно открыто
	//	GetDesktopWindow() - делаем рабочий стол родительским окном (первым параметром вместо NULL), модальность не работает
	//	FindWindow(NULL, "WinAPI - Microsoft Visual Studio") - найти окно по заголовку окна
	//	(и сделать его родительским вместо первого NULL) и модальность работает.


	// Для создания диалогового окна первым делом надо создать ресурс
	//	(SLN Explorer - ПКМ по Resource Files - Add - New Item - слева Resource - и Resource File(.rc))

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);	// Вызов нашего окна

	return 0;
}

//	Без процедуры окно не запустится
//	Одна процедура может обрабатывать несколько окон
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			// Инициализация окна (создание значков и т.д.)
		{
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); //	создание иконки
			//	Первый параметр функции LoadIcon() - это .exe-файл для которго нужно загрузить иконку
			//	.exe-файл всегда можно получить с помощью функции GetModuleHandle(), с параметром NULL.
			
			//	Второй параметр - собственно сама иконка, делаем с помощью функции MAKEINTRESOURCE(IDI_ICON1),
			//	с параметром ID ресурса, в нашем случае IDI_ICON1.

			// С2360 initialization of 'hIcon' is skipped by 'case' label - ошибка возникает, если
			//	мы в кейсе что-то объявляем с инициализацией, и при этом у нас нет {}.
			//	Выход - код кейса взять в {}.

			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);	// установка иконка в окно, для этого нужно
			//	отправить окну (в нашем случае hwnd) сообщение.
			//	WM_SETICON - это есть сообщение окну.
			//	WPARAM - у нас 0.
			//	LPARAM - сама иконка hIcon.
			//	Suntax: SendMessage([in] HWND hWnd,	[in] UINT Msg, [in] WPARAM wParam, [in] LPARAM lParam);

			//	Далее идём в Toolbox (слева, когда открыто Resource View), там используем:
			//	Static text,
			//	Toggle Grid (сверху панель инструментов) - можно установить сетку, для позиционирования окон
			//	Edit Control
			
			// Установка фокуса:
			//SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETFOCUS, 0, 0);	//	на уроке не заработал должным образом, можно грохнуть

			SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)INVITATION_LOGIN);
			//	записываем в окно Логин (по HWND с помощью функции GetDlgItem()) текст из INVITATION_LOGIN,
			//	который объявили глобальной константой в самом верху
			SendMessage(GetDlgItem(hwnd, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)INVITATION_PASSWORD);
		}
		break;
		case WM_COMMAND:
			// Обработка команд нажатия кнопок и т.д.
		{
			// WORD = 2 BYTES
			// DWORD = 4 BYTES
			// LOWORD - два младших Байта в двойном слове
			// HIWORD - два старших Байта в двойном слове
			switch (LOWORD(wParam))	//	на какую именно кнопку мы нажали (младшее слово в wParam)
			{
				case IDC_BTN_COPY: //	(ID кнопки копирования в нашем диалоговом окне)
				{
					CONST INT SIZE = 256;
					CHAR buffer[SIZE]{};
					HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
					//	Получаем HWND (уникальный номер, дескриптор) элемента окна по его ResourceID,
					//	чтобы можно было обратиться к этому окну (надо знать HWND hwnd).

					SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)buffer);	// копируем содержимое окна hEdit (под логином) в buffer
					//	WM_GETTEXT (WPARAM это SIZE, (LPARAM)buffer это куда копируем текст).

					SendMessage(GetDlgItem(hwnd, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)buffer);	// копирует текст в окно PASSWORD из buffer.
					//	Можно без введения новой переменной (hEdit), как для логина, сразу вызвать функцию GetDlgItem и получить HWND для IDC_EDIT_PASSWORD.
					//	WM_SETTEXT (WPARAM не используется, ставим 0, (LPARAM)buffer - откуда берём текст для вставки)
				}
				break;
				case IDOK:		MessageBox(hwnd, "Была нажата кнопка ОК", "Info", MB_ICONINFORMATION | MB_OK); break;
				case IDCANCEL:	EndDialog(hwnd, 0);	break;

					//	Фокус, это элемент окна (тоже окно), который принимает команды с клавиатуры и это каким-то образом влияет на окно диалога
				case IDC_EDIT_LOGIN:
				{
					CONST INT SIZE = 256;
					CHAR buffer[SIZE]{};
					SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_GETTEXT, SIZE, (LPARAM)buffer);

					//	Если обращаются к нашему EDIT CONTROL, мы определяем через LOWORD(wParam), он в switch выше.
					//	Что именно пользователь сделал с EDIT CONTROL, находится в HIWORD(wParam), он в switch ниже.
					switch (HIWORD(wParam))
					{
						case EN_SETFOCUS:	// EN_ уведомление, событие, которое произошло с нашим окном.
							if (strcmp(buffer, INVITATION_LOGIN)==0)
								//	сброс текста при установке фокуса будет происходить только в том случае, если
								//	сравнение строк, с помощью функции strcmp() будет равно 0.
								SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)"");
						break;
						case EN_KILLFOCUS:	// когда мы уводим фокус с этого окна, но ничего не ввели,
							//	чтобы текст не сбрасывался, а остался тот, что в приглашении.
							if (strcmp(buffer, "") == 0)
								SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)INVITATION_LOGIN);
						break;
					}
				}
				break;
			}
		}
			break;
		case WM_CLOSE:
			// Закрытие окна
			//break;
			EndDialog(hwnd, 0);	// показываем, какое окно закрываем, со вторым параметром 0
	}
	return FALSE;
}