#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <algorithm>
#include <locale>
#include <fstream>
#include "resource.h"
#include <process.h>
#include <iostream>
#include <Windows.h>
using namespace std;

//читаем и записываем строки из файла
string getWordFromFile(TCHAR nameOpenText[])
{
	ifstream fin;
	fin.open(nameOpenText);
	string word;
	string st;
	while (getline(fin, st))
		word += st;
	fin.close();

	return word;
}

//выводим в файл
void foutDataToFile(TCHAR nameOutputText[], string data)
{
	ofstream fout;
	fout.open(nameOutputText);
	fout << data;
	fout.close();
}


//----------
//для Цезаря

const string letters = " !\"#$%&'()*+,-./0123456789:;<=>?@][\\^_`{|}~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЫЬЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщыьъэюя";
const int lettersLen = letters.length();

string MakeDictionaryWithKeyWord(string keyWord, int key)
{
	int KeyWordLen = keyWord.length();
	string tempDictionary(letters);

	int reallyKeyWordLen = 0;
	for (int i = 0; i < KeyWordLen; i++) //убираем из алфавита символы, которые содержатся в ключе, тем самым считаем настоящую длину ключа!
	{
		size_t found = tempDictionary.find(keyWord[i]);
		if (found != string::npos)
			tempDictionary[found] = '\0';
	}

	string dictionaryWithKeyWord;
	dictionaryWithKeyWord.resize(key + KeyWordLen); //изменяем размер, чтобы вставить ключевое слово на нужное место
	dictionaryWithKeyWord.insert(key, keyWord); //вставляем ключевое слово на позицию начиная с key
	dictionaryWithKeyWord.resize(lettersLen); //изменяем размер на размер обычного алфавита

	int  itertempDict = 0;
	for (int iterKeyDict = KeyWordLen + key; iterKeyDict < lettersLen; itertempDict++) //теперь записываем в готовый словарь все остальные символы, кроме символов ключа (символы ключа начинаются от позиции key)
		if (tempDictionary[itertempDict] != '\0')
			dictionaryWithKeyWord[iterKeyDict++] = tempDictionary[itertempDict];

	for (int iterKeyDict = 0; iterKeyDict < key; itertempDict++) //и дописываем оставшиеся символы до символов с ключем
		if (tempDictionary[itertempDict] != '\0')
			dictionaryWithKeyWord[iterKeyDict++] = tempDictionary[itertempDict];

	return dictionaryWithKeyWord;
}

string getTranslatedWord(string whereTakeLetters, string whereFindLetters, string translateWord)
{
	int translateWordLen = translateWord.length();
	string readyTranslatedWord;
	readyTranslatedWord.resize(translateWordLen);

	for (int i = 0; i < translateWordLen; i++) //выбираем по одному алфавиту символ другого алфавита
		readyTranslatedWord[i] = whereTakeLetters[whereFindLetters.find(translateWord[i])];

	return readyTranslatedWord;
}

string Encrypt(string word, string keyWord, int key)
{
	return getTranslatedWord(MakeDictionaryWithKeyWord(keyWord, key), letters, word);
}

string Decrypt(string CryptWord, string keyWord, int key)
{
	return getTranslatedWord(letters, MakeDictionaryWithKeyWord(keyWord, key), CryptWord);
}

string getClearkeyWord(string keyWord)
{
	size_t found = 0;
	for (int i = 0; keyWord[i] != '\0'; i++)
		do
		{
			string sub = keyWord.substr(i + 1);
			found = sub.find(keyWord[i]);
			if (found != string::npos)
				keyWord.erase(found + i + 1, 1);
		} while (found != string::npos);

	return keyWord;
}


#endif // !FUNCTIONS_H