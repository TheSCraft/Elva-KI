#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

void print() {
	string y = "nix";
	string x = ".txt";
	string z = "a";
	string t = "void";
	string m = "not";
	system("CLS");
	cout << "Bitte Dateiname eingeben: ";
	cin >> y;
	z = y + x;
	ofstream MyFile(z);
	system("CLS");
	cout << y << endl;
	cout << endl;
	cout << "Bitte Text eingeben: " << endl;
	cout << setw(23) << setfill('-') << "" << endl;
	cin >> m;
	getline(cin, t);
	t = m + t;
	cout << t;
	MyFile << t;
	MyFile.close();
	//main();
	this_thread::sleep_for(chrono::milliseconds(5000));
}

void read() {
	string myText;
	string yy = "nix";
	string xx = ".txt";
	string zz = ".txt";
	system("CLS");
	cout << "Bitte Dateiname eingeben: ";
	cin >> yy;
	zz = yy + xx;

	ifstream MyReadFile(zz);


	while (getline(MyReadFile, myText)) {
		cout << myText;
	}
	MyReadFile.close();
	this_thread::sleep_for(chrono::milliseconds(5000));

}

int main()
{
	int dau = 0;
	while (dau == 0) {
		int i = 1;
		int q = 0;
		system("CLS");
		cout << "Text files" << endl;
		cout << "Schreiben(1)" << endl;
		cout << "Lesen(2)" << endl;
		cout << "Exit(3)" << endl;
		cin >> q;
		if (q == 1) {
			i = 2;
			print();
		}
		if (q == 2) {
			i = 2;
			read();
		}
		if (q == 3) {
			dau = 1;
		}
		else {
			if (i == 1) {
				cout << "Error bitte nur 1 oder 2";
				this_thread::sleep_for(chrono::milliseconds(5000));
				system("CLS");
				main();
			}
		}
	}
	return 0;
}

