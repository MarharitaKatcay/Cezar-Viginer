#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"
#include <process.h>
#include <iostream>
#include <commdlg.h>
#include <fstream>
#include <vector>
#include <string>
#include <Shellapi.h>
using namespace std;

HINSTANCE hInst;
HWND hwndCOMMON;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)(DlgProc), 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static TCHAR nameOpenText[256] = "", nameEncryptedText[256] = ""; //для имени файла
	static OPENFILENAME fileOpenText;
	static vector<string> vOpenText; //вектора для строк из файла
	static string key = "";
	ifstream in; //для чтения из файла
	ofstream out; //для записи в файл
	string st; //для временной записи строки из файла

	switch (uMsg)
	{
	case WM_INITDIALOG:	
		//hwndCOMMON = GetDlgItem(hwnd, IDC_TABLO);
		//заполнение структуры file при создании окна
		fileOpenText.lStructSize = sizeof(OPENFILENAME);
		fileOpenText.hInstance = hInst;
		fileOpenText.lpstrFilter = "Text\0*.txt";
		fileOpenText.lpstrFile = nameOpenText;
		fileOpenText.nMaxFile = 256;
		fileOpenText.lpstrInitialDir = ".\\";
		fileOpenText.lpstrDefExt = "txt";

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON_OPEN_CLEAR_FILE: {
			if(strcmp(nameOpenText,""))
				ShellExecute(0, 0, "notepad.exe", nameOpenText, 0, SW_SHOW);
			break;
		}
		case IDC_BUTTON_OPEN_ENCRYPTED_FILE: {
			if (strcmp(nameEncryptedText, ""))
				ShellExecute(0, 0, "notepad.exe", nameEncryptedText, 0, SW_SHOW);
			break;
		}

		case IDC_FILE_OPEN_CLEAR_TEXT: {
			fileOpenText.lpstrTitle = "Открыть файл с открытым текстом"; //изменение заголовка диалогового окна
			fileOpenText.Flags = OFN_HIDEREADONLY; //можно было бы и не заполнять, поскольку OFN_HIDEREADONLY
												   //означает лишь то, что переключатель Read Only не выводится.
			if (!GetOpenFileName(&fileOpenText)) //вывод диалогового окна. Если открылось,
												 //то в массиве name будет полное имя файла
				return 1;

			//меняем имя переменной с путем к файлу с открытым кодом, и открывая его - файл создастся автоматом
			st = nameOpenText;
			st.insert(st.rfind('.'), "_ENCRYPTED");
			strcpy(nameEncryptedText, st.c_str());
			out.open(nameEncryptedText);
			out.close();

			SetDlgItemText(hwnd, ID_PATH_CLEAR_TEXT, nameOpenText);
			SetDlgItemText(hwnd, ID_PATH_ENCRYPTED_TEXT, nameEncryptedText);

			//читаем и записываем строки из файла
			in.open(nameOpenText);
			while (getline(in, st))
				vOpenText.push_back(st);
			in.close();

			break;
		}

		case IDC_ENCRYPT: {
			break;
		}

		case IDC_DECRYPT: {
			break;
		}

		case ID_ABOUT: {
			MessageBoxA(hwnd, "В данной работе использовались алгоритмы .... и ....", "Алгоритмы", MB_OK);
			break;
		}

		case ID_EXIT: {
			if (MessageBoxA(hwnd, "Вы уверены, что хотите выйти?", "Exit dialog", MB_YESNO | MB_ICONQUESTION) == IDYES)
				EndDialog(hwnd, 0);
			break;
		}
		}
		break;
	case WM_CLOSE:
		if (MessageBoxA(hwnd, "Вы уверены, что хотите выйти?", "Exit dialog", MB_YESNO | MB_ICONQUESTION) == IDYES)
			EndDialog(hwnd, 0);
		return FALSE;
	}
	return FALSE;
}
