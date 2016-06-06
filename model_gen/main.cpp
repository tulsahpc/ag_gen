#include <iostream>
#include "netMod.h"
#include "atkPat.h"

using namespace std;

int mainMenu();

int main() {
	int menuChoice{mainMenu()};
	switch (menuChoice) {
		case 1:
			genNetModel();
			break;
		case 2:
			genAtkPat();
			break;
		case 3:
			exit(1);
			break;
	}
	return 0;
}

int mainMenu() {
	cout << "-------------------------------------------------" << endl;
	cout << "Welcome to the Interactive Model Generation Tool!" << endl;
	cout << "Written and Developed by Lucifer!" << endl;
	cout << "-------------------------------------------------" << endl;
	bool madeSelection{false};
	int usrChoice{-1};
	string usrInput{};

	do {
		cout << "Main Menu:" << endl;
		cout << "1) Generate Network Model" << endl;
		cout << "2) Generate Exploit Pattern" << endl;
		cout << "3) Exit" << endl;
		cout << "4) Mystery Button" << endl;
		cout << "> ";
		cin >> usrInput;

		try {
			usrChoice = stoi(usrInput);
		} catch ( std::invalid_argument ) {
			cout << "Enter a number" << endl;
		}

		if (usrChoice < 5 && usrChoice > 0) {
			madeSelection = true;
		} else {
			cout << "Invalid Menu Selection" << endl;
		}
	} while (!madeSelection);
	return usrChoice;
}
