#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <fstream>

using namespace std;

struct Route
{
	char startPoint[100];
	char endPoint[100];
	int numberOfRoute;
};

void CreateBIN(string& fname)
{
	Route route;
	int c;

	ofstream fout(fname, ios::binary);

	if (!fout.is_open()) {
		cout << "Помилка відкриття файлу!" << endl;
		return;
	}

	cout << "Введіть кількість маршрутів: "; cin >> c;
	cin.ignore();

	do
	{
		cout << "Введіть номер маршруту: "; cin >> route.numberOfRoute;
		cin.get();
		cin.sync();
		cout << endl;
		cout << "Введіть назву початкового пункту маршруту: "; cin.getline(route.startPoint,99);

		cout << "Введіть назву кінцевого пункту маршруту: "; cin.getline(route.endPoint,99);
		cout << endl;

		fout.write((char*)&route, sizeof(route));

		c--;
	} while (c > 0);

	fout.close();
}

void fWrite(fstream& f, const int i, const Route n)
{
	f.seekp(i * (long)sizeof(n)); // встановили вказівник
	f.write((char*)&n, sizeof(n)); // записали значення
}

Route fRead(fstream& f, const int i)
{
	Route n;
	f.seekg(i * (long)sizeof(n)); // встановили вказівник
	f.read((char*)&n, sizeof(n)); // прочитали значення
	return n;
}

void fChange(fstream& f, const int i, const int j)
{
	Route x = fRead(f, i);
	Route y = fRead(f, j);
	fWrite(f, i, y);
	fWrite(f, j, x);
}

void PrintBIN(string& fname)
{
	ifstream fin(fname, ios::binary);

	if (!fin.is_open()) {
		cout << "Помилка відкриття файлу!" << endl;
		return;
	}


	Route route;

	cout << "========================================================================"<< endl;
	cout << "| Номер маршруту | Початковий пункт маршруту | Кінцевий пункт маршруту |"<< endl;
	cout << "------------------------------------------------------------------------"<< endl;

	while (fin.read((char*)&route, sizeof(route)))
	{
		cout << "| " << setw(15) << left << route.numberOfRoute
			<< "| " << setw(26) << right << route.startPoint
			<< "| " << setw(24) << right << route.endPoint << "|" << endl;
	}

	cout << "========================================================================"<< endl;
	cout << endl;

	fin.close();
}

void Sort(string& fname)
{
	fstream f(fname, ios::binary | ios::in | ios::out);
	Route a, b;

	f.seekg(0, ios::end);
	int size = f.tellg() / (long)sizeof(a);
	f.seekg(0, ios::beg);

	for (int i0 = 1; i0 < size; i0++)
		for (int i1 = 0; i1 < size - i0; i1++)
		{
			a = fRead(f, i1);
			b = fRead(f, i1 + 1);
			if (a.numberOfRoute > b.numberOfRoute)
			{
				fChange(f, i1, i1 + 1);
			}
		}
	f.seekp(0, ios::end);
	f.close();
}

void PrintRoutes(string& fname, const string selectedItem)
{
	ifstream fin(fname, ios::binary);

	if (!fin.is_open()) {
		cout << "Помилка відкриття файлу!" << endl;
		return;
	}

	int f;
	int i = 0;
	cout << "Введіть чи початковий чи кінцевий пункт ( 0 - початковий , 1 - кінцевий ): "; cin >> f;

	cout << "========================================================================" << endl;
	cout << "| Номер маршруту | Початковий пункт маршруту | Кінцевий пункт маршруту |" << endl;
	cout << "------------------------------------------------------------------------" << endl;

	Route route;

	while (fin.read((char*)&route, sizeof(route))) 
	{
		if (f == 0 && route.startPoint == selectedItem)
		{
			cout << "| " << setw(15) << left << route.numberOfRoute
				<< "| " << setw(26) << right << route.startPoint
				<< "| " << setw(24) << right << route.endPoint << "|" << endl;
			i++;
		}
		else if (f == 1 && route.endPoint == selectedItem)
		{
			cout << "| " << setw(15) << left << route.numberOfRoute
				<< "| " << setw(26) << right << route.startPoint
				<< "| " << setw(24) << right << route.endPoint << "|" << endl;
			i++;
		}
	}


	if (i == 0)
	{
		cout << endl;
		cout << "Вибраний вами пункт маршруту відсутній у таблиці маршрутів" << endl;
		cout << endl;
	}
	cout << "========================================================================" << endl;
	fin.close();
}

int main()
{
	SetConsoleCP(1251); // встановлення сторінки win-cp1251 в потік вводу
	SetConsoleOutputCP(1251); // встановлення сторінки win-cp1251 в потік виводу

	string selectedItem;
	string fname;

	cout << "Введіть ім'я файлу: ";
	getline(cin,fname);


	int menuItem;
	do {
		cout << endl << endl << endl;
		cout << "Виберіть дію:" << endl << endl;
		cout << " [1] - введення даних з клавіатури у файл" << endl;
		cout << " [2] - вивід даних на екран" << endl;
		cout << " [3] - фізичне впорядкування даних" << endl;
		cout << " [4] - вивід на екран інформації про маршрути, які починаються або закінчуються у вибраному пункті" << endl;
		cout << " [0] - вихід та завершення роботи програми" << endl << endl;
		cout << "Введіть значення: "; cin >> menuItem;
		cout << endl << endl << endl;
		switch (menuItem)
		{
		case 1:
			CreateBIN(fname);
			break;
		case 2:
			PrintBIN(fname);
			break;
		case 3:
			Sort(fname);
			PrintBIN(fname);
			break;
		case 4:
			cin.get();
			cin.sync();
			cout << "Введіть пункт призначення/відправлення: "; getline(cin, selectedItem);
			cout << endl;
			PrintRoutes(fname, selectedItem);
			break;
		case 0:
			break;
		default:
			cout << "Ви ввели помилкове значення! "
				"Слід ввести число - номер вибраного пункту меню" << endl;
		}
	} while (menuItem != 0);

	return 0;
}