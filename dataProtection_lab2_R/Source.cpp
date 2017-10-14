#include "functionsAlg.h"
#include <commdlg.h>
#include <Shellapi.h>

HINSTANCE hInst;
HWND hwndCOMMON;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_ALL, "russian");
	hInst = hInstance;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)(DlgProc), 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static TCHAR nameOpenText[256] = "", nameEncryptedText[256] = ""; //для имени файла
	static OPENFILENAME fileOpenText;
	static string word; //слово из файла для шифровки
	static string keyWord = "";
	static string encryptWord, decryptWord;
	static int key = -1;
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

		case IDC_BUTTON_SYNC: {

			if (nameOpenText != "")
				word = getWordFromFile(nameOpenText);

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
			ofstream out;
			out.open(nameEncryptedText);
			out.close();

			SetDlgItemText(hwnd, ID_PATH_CLEAR_TEXT, nameOpenText);
			SetDlgItemText(hwnd, ID_PATH_ENCRYPTED_TEXT, nameEncryptedText);

			word = getWordFromFile(nameOpenText);

			break;
		}

		case IDC_ENCRYPT: {

			if((key == -1) && (keyWord == ""))
				MessageBoxA(hwnd, "Введите ключ и ключевое слово!", "Ошибка", MB_OK);
			else
				if(key == -1)
					MessageBoxA(hwnd, "Введите ключ!", "Ошибка", MB_OK);
				else
					if(keyWord == "")
						MessageBoxA(hwnd, "Введите ключевое слово!", "Ошибка", MB_OK);
					else
					{
						encryptWord = Encrypt(word, keyWord, key);
						foutDataToFile(nameEncryptedText, "Ура, зашифровано!\n" + encryptWord);
					}

			break;
		}

		case IDC_DECRYPT: {

			if(encryptWord == "")
				MessageBoxA(hwnd, "Вы еще не шифровали сообщение!", "Ошибка", MB_OK);
			else
			{
				decryptWord = Decrypt(encryptWord, keyWord, key);
				foutDataToFile(nameEncryptedText, "Ура, расшифровано!\n" + decryptWord);
			}

			break;
		}

		case ID_BUTTON_ACCEPT_INPUT_KEY: {

			TCHAR Tmpkey[64] = "";
			GetDlgItemText(hwnd, IDC_INPUT_KEY, Tmpkey, 4096);
			if (isdigit(Tmpkey[0]))
			{
				key = atoi(Tmpkey);
				int key = 4;
				while (key > lettersLen)
					key -= lettersLen;
			}
			else
				key = -1;

			break;
		}

		case ID_BUTTON_ACCEPT_INPUT_KEYWORD: {

			TCHAR TmpkeyWord[4096] = "";
			GetDlgItemText(hwnd, IDC_INPUT_KEYWORD, TmpkeyWord, 4096);
			keyWord = TmpkeyWord;
			keyWord = getClearkeyWord(keyWord);
			
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


