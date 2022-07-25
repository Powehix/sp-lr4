#include <iostream>
#include <windows.h>

using namespace std;

void SearchFile(LPCTSTR path);
void FileAttributes(DWORD attributes, HANDLE hFile);
void AddStructure(LPCTSTR path);
void DeleteStructure(LPCTSTR path, LPCTSTR temp);
void ViewFile(LPCTSTR path);

struct House 
{
	char street[100];
	unsigned int house;
	unsigned int apartments;
};

int main()
{
	/* 9 вариант:
	Реализовать программно поиск файла house.dat на диске с:\. 
	Если файл найден, вывести все его временные атрибуты, если не найден, 
	создать файл в C:\My Documents . 
	Реализовать следующие функции работы с файловыми данными: 
	добавление структуры, удаление, просмотр содержания файла.
	Структура должна хранить след. данные: улица, номер дома, количество квартир. */

	int menu;
	LPCTSTR path = "C:\\Users\\User\\Documents\\house.dat";
	LPCTSTR temp = "C:\\Users\\User\\Documents\\temp.dat";

	setlocale(LC_ALL, "Russian");

	cout << "1. Search/create file" << endl;
	cout << "2. Add structure" << endl;
	cout << "3. Delete structure" << endl;
	cout << "4. View file" << endl;
	cout << "0. Exit" << endl;
	cout << "Сhoose what you want to do:" << endl;

	cin >> menu;
	//пользовательское меню
	switch (menu)
	{
	case 0:
		return 0;
	case 1:
		//поиск файла и вывод его атрибутов
		SearchFile(path);
		break;
	case 2: 
		//добавление структуры
		AddStructure(path);
		break;
	case 3:
		//удаление структуры
		DeleteStructure(path, temp);
		break;
	case 4:
		//просмотр содержимого файла
		ViewFile(path);
		break;
	default: 
		cout << "Incorrect value! Please restart the application. ";
	}

	system("PAUSE");
	return 0;
}

void SearchFile(LPCTSTR path)
{
	system("cls");

	WIN32_FIND_DATA fd;
	/* Если искать только в корневом каталоге диска при условии, что файл создается в папке Documents,
	то файл никогда не найдется, и, соответственно, атрибуты не выведутся, поэтому поиск производится
	в той же папке, где и создается файл. */
	HANDLE hFile, hFindFile = FindFirstFile(path, &fd);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		cout << "The file was not found, so it was created. Restart the application!" << endl;
		hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		CloseHandle(hFile);
	}
	else
	{
		//открывает уже существующий файл для получения hFile
		hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		cout << "File found successfully!" << endl << endl;
		cout << "File name: " << fd.cFileName << endl;
		cout << "Attributes: ";
		FileAttributes(fd.dwFileAttributes, hFile);
	}
	FindClose(hFindFile);
}

void FileAttributes(DWORD attributes, HANDLE hFile)
{
	//перечислены все возможные атрибуты файла
	if (attributes & FILE_ATTRIBUTE_ARCHIVE)
		cout << "| Archive";
	if (attributes & FILE_ATTRIBUTE_COMPRESSED)
		cout << "| Compressed";
	if (attributes & FILE_ATTRIBUTE_DEVICE)
		cout << "| Device";
	if (attributes & FILE_ATTRIBUTE_DIRECTORY)
		cout << "| Directory";
	if (attributes & FILE_ATTRIBUTE_ENCRYPTED)
		cout << "| Encrypted";
	if (attributes & FILE_ATTRIBUTE_HIDDEN)
		cout << "| Hidden";
	if (attributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
		cout << "| Integrity stream";
	if (attributes & FILE_ATTRIBUTE_NORMAL)
		cout << "| Normal";
	if (attributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
		cout << "| Not content indexed";
	if (attributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
		cout << "| No scrub data";
	if (attributes & FILE_ATTRIBUTE_OFFLINE)
		cout << "| Offline";
	if (attributes & FILE_ATTRIBUTE_READONLY)
		cout << "| Readonly";
	if (attributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
		cout << "| Recall on data access";
	if (attributes & FILE_ATTRIBUTE_RECALL_ON_OPEN)
		cout << "| Recall on open";
	if (attributes & FILE_ATTRIBUTE_REPARSE_POINT)
		cout << "| Reparse point";
	if (attributes & FILE_ATTRIBUTE_SYSTEM)
		cout << "| System";
	if (attributes & FILE_ATTRIBUTE_TEMPORARY)
		cout << "| Temporary";
	if (attributes & FILE_ATTRIBUTE_VIRTUAL)
		cout << "| Virtual";

	//временные атрибуты файла
	FILETIME createFT, accessFT, writeFT;
	SYSTEMTIME createST, accessST, writeST;

	GetFileTime(hFile, &createFT, &accessFT, &writeFT);
	FileTimeToSystemTime(&createFT, &createST);
	FileTimeToSystemTime(&accessFT, &accessST);
	FileTimeToSystemTime(&writeFT, &writeST);

	cout << "\n\nCreation time: " << createST.wDay << "." << createST.wMonth << "." << createST.wYear << ",  " << createST.wHour + 3 << ":" << createST.wMinute << ":" << createST.wSecond << endl;
	cout << "Last access time: " << accessST.wDay << "." << accessST.wMonth << "." << accessST.wYear << ",  " << accessST.wHour + 3 << ":" << accessST.wMinute << ":" << accessST.wSecond << endl;
	cout << "Last write time: " << writeST.wDay << "." << writeST.wMonth << "." << writeST.wYear << ",  " << writeST.wHour + 3 << ":" << writeST.wMinute << ":" << writeST.wSecond << endl;

	cout << endl;
}

void AddStructure(LPCTSTR path)
{
	system("cls");

	HANDLE hFile;
	DWORD dWord;
	struct House add;

	//CreateFile открывает уже существующий файл
	hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile, 0, NULL, FILE_END);

	cout << "Enter street: ";
	cin.ignore();
	cin.getline(add.street, 99);
	cout << "Enter house: ";
	cin >> add.house;
	cout << "Enter apartments: ";
	cin >> add.apartments;
	cout << "\nStructure was added successfully!" << endl << endl;

	WriteFile(hFile, &add, sizeof(add), &dWord, NULL);
	CloseHandle(hFile);
}

void DeleteStructure(LPCTSTR path, LPCTSTR temp)
{
	system("cls");

	HANDLE hFile;
	DWORD dWord, size, filesize;
	int num;
	bool ans = false;
	int i = 0;

	//открытие файла и считывание его содержимого в массив структур
	hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	filesize = GetFileSize(hFile, &size);
	size = filesize / (sizeof(House));
	House * data = new House[size];
	cout << "Total number of structures: " << size << endl;
	ReadFile(hFile, &data[i], sizeof(House), &dWord, NULL);
	while (dWord != 0)
	{
		i++;
		ReadFile(hFile, &data[i], sizeof(House), &dWord, NULL);
	}
	CloseHandle(hFile);
	DeleteFile(path);

	//открытие временного файла и заполнение его всеми структурами кроме той, что указывает пользователь для удаления
	hFile = CreateFile(temp, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile, 0, NULL, FILE_END);
	cout << "Enter the sequence number of structure (starting from 0): " << endl;
	cin >> num;
	for (int i = 0; i < size; i++)
	{
		if (i == num)
		{
			ans = true;
		}
		else
		{
			WriteFile(hFile, &data[i], sizeof(House), &dWord, NULL);
		}
	}
	delete[]data;
	CloseHandle(hFile);

	//если пользователь ввел корректный порядкой номер структуры, то временный файл переименовывается в исходный, в противном случае удаляется
	if (ans == true)
	{
		MoveFile(temp, path);
		cout << "Structure was deleted successfully!" << endl << endl;
	}
	else 
	{
		DeleteFile(temp);
		cout << "Invalid structure number!" << endl;
	}
}

void ViewFile(LPCTSTR path)
{
	system("cls");

	HANDLE hFile;
	DWORD dWord;
	struct House view;

	hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile, &view, sizeof(House), &dWord, NULL);
	cout << "Structures: " << endl << endl;
	while (dWord != 0)
	{
		cout << "Street: " << view.street << endl;
		cout << "House: " << view.house << endl;
		cout << "Apartments: " << view.apartments << endl << endl;
		ReadFile(hFile, &view, sizeof(view), &dWord, NULL);
	}
	CloseHandle(hFile);
}