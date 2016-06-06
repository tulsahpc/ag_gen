//Includes go here
//Normally I would put this in an associated header file, but this project is small
//Enough I won't worry about it
#include <iostream>
#include "netMod.h"
#include "atkPat.h"

//Namespaces go here
using namespace std;

//Function declarations go here
//In C/C++, all functions must be declared before being used
//The actuall code can go elsewhere
int mainMenu();

int main() {
    int menuChoice{mainMenu()}; //Print the main menu and get the user input
    switch ( menuChoice ) { //switch on user choice
        case 1:
            genNetModel();
            break;
        case 2:
            genAtkPat();
            break;
        case 3:
            exit( 1 ); //exit the program
            break;
        case 4:
            break;
    }


    return 0;
}

int mainMenu() {
    cout << "-------------------------------------------------" << endl;
    cout << "Welcome to the Interactive Model Generation Tool!" << endl;
    cout << "Written and Developed by Lucifer!" << endl;
    cout << "-------------------------------------------------" << endl;
    bool madeSelection{false}; //track if the user has entereed a valid selection
    int usrChoice{-1}; //the user's numerical input, initialized to -1
    string usrInput{}; //an empty string, will hold the input before converting to int
    //This is for error handling, and making sure the user put in a valid number
    do { //while the user has not inputted a correct number
        cout << "Main Menu:" << endl;
        cout << "1) Generate Network Model" << endl;
        cout << "2) Generate Exploit Pattern" << endl;
        cout << "3) Exit" << endl;
        cout << "4) Mystery Button" << endl;
        cout << "> ";
        cin >> usrInput;
        /*
         * This try/catch is to prevent the user from putting in a string
         * Without this, we would have to either cin >> to an int and trust the user
         * Or we would have to use a string compare against every correct value
         * e.g. usrChoice == "1" || usrChoice == "2" etc...
         */
        try {
            usrChoice = stoi( usrInput ); //stoi converts a string to an integer
        } catch ( std::invalid_argument ) { //if they put in a string, we catch the exception
            cout << "Enter a number" << endl;
        }
        if ( usrChoice < 5 && usrChoice > 0 ) //on a valid input, toggle the boolean
            madeSelection = true;
        else
            cout << "Invalid Menu Selection" << endl;
    } while ( !madeSelection );
    return usrChoice;
}
