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
		cout << "������� �������� �����!" << endl;
		return;
	}

	cout << "������ ������� ��������: "; cin >> c;
	cin.ignore();

	do
	{
		cout << "������ ����� ��������: "; cin >> route.numberOfRoute;
		cin.get();
		cin.sync();
		cout << endl;
		cout << "������ ����� ����������� ������ ��������: "; cin.getline(route.startPoint,99);

		cout << "������ ����� �������� ������ ��������: "; cin.getline(route.endPoint,99);
		cout << endl;

		fout.write((char*)&route, sizeof(route));

		c--;
	} while (c > 0);

	fout.close();
}

void fWrite(fstream& f, const int i, const Route n)
{
	f.seekp(i * (long)sizeof(n)); // ���������� ��������
	f.write((char*)&n, sizeof(n)); // �������� ��������
}

Route fRead(fstream& f, const int i)
{
	Route n;
	f.seekg(i * (long)sizeof(n)); // ���������� ��������
	f.read((char*)&n, sizeof(n)); // ��������� ��������
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
		cout << "������� �������� �����!" << endl;
		return;
	}


	Route route;

	cout << "========================================================================"<< endl;
	cout << "| ����� �������� | ���������� ����� �������� | ʳ������ ����� �������� |"<< endl;
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
		cout << "������� �������� �����!" << endl;
		return;
	}

	int f;
	int i = 0;
	cout << "������ �� ���������� �� ������� ����� ( 0 - ���������� , 1 - ������� ): "; cin >> f;

	cout << "========================================================================" << endl;
	cout << "| ����� �������� | ���������� ����� �������� | ʳ������ ����� �������� |" << endl;
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
		cout << "�������� ���� ����� �������� ������� � ������� ��������" << endl;
		cout << endl;
	}
	cout << "========================================================================" << endl;
	fin.close();
}

int main()
{
	SetConsoleCP(1251); // ������������ ������� win-cp1251 � ���� �����
	SetConsoleOutputCP(1251); // ������������ ������� win-cp1251 � ���� ������

	string selectedItem;
	string fname;

	cout << "������ ��'� �����: ";
	getline(cin,fname);


	int menuItem;
	do {
		cout << endl << endl << endl;
		cout << "������� ��:" << endl << endl;
		cout << " [1] - �������� ����� � ��������� � ����" << endl;
		cout << " [2] - ���� ����� �� �����" << endl;
		cout << " [3] - ������� ������������� �����" << endl;
		cout << " [4] - ���� �� ����� ���������� ��� ��������, �� ����������� ��� ����������� � ��������� �����" << endl;
		cout << " [0] - ����� �� ���������� ������ ��������" << endl << endl;
		cout << "������ ��������: "; cin >> menuItem;
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
			cout << "������ ����� �����������/�����������: "; getline(cin, selectedItem);
			cout << endl;
			PrintRoutes(fname, selectedItem);
			break;
		case 0:
			break;
		default:
			cout << "�� ����� ��������� ��������! "
				"��� ������ ����� - ����� ��������� ������ ����" << endl;
		}
	} while (menuItem != 0);

	return 0;
}