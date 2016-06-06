//
// Created by archlord on 6/5/16.
//

#include "netMod.h"

int genNetModel() {

    //Useful declarartions
    string netModName{};
    vector<string> assetVector{}; //A vector of assets
    vector<quality> qualities{}; //All qualities stored in one vector since order doesn't matter
    vector<topology> topologies{};

    //Start by getting the model name
    cout << "Please name your network model" << endl;
    cout << "> ";
    cin >> netModName;
    //Then the main menu


    bool readyToPrint{false}; //More of the same menu code
    do { //do until we're ready to print
        int choice = selectAsset( assetVector ); //choose an object or menu option

        if ( choice == assetVector.size()) { //Create a new asses
            string newAsset = addAsset(); //get the new asset name
            newAsset = fixString( newAsset );
            assetVector.push_back( newAsset ); //Add the asset to the end of the vector
        }
        else if ( choice == assetVector.size() + 1 ) {
            topolgyManager( assetVector, topologies );
        }
        else if ( choice == assetVector.size() + 2 ) { //Print the SQl and exit
            readyToPrint = true;
        }
        else { //Add qualities to an existing object
            qualityMangager( choice, qualities );
        }
    } while ( !readyToPrint );

    cout << "SQL Commands:" << endl;
    cout << "-----------------------------------------------" << endl;
    printSql( netModName, assetVector, qualities, topologies );
    cout << "-----------------------------------------------" << endl;

    return 0;
}

int selectAsset( vector<string> assetVector ) {
    //Here is that same menu code from the main menu
    bool validChoice{false};
    string usrChoice{};
    int usrSelection{-1};

    do {
        cout << "Select an asset to modify:" << endl;

        int enhancedIdx{0};
        for ( string asset : assetVector ) { //For every asset currently in the vector
            cout << "\t" << enhancedIdx + 1 << ") " << asset << endl;
            enhancedIdx++;
        }

        cout << enhancedIdx + 1 << ") Create New" << endl;
        cout << enhancedIdx + 2 << ") Topolgy Manager" << endl;
        cout << enhancedIdx + 3 << ") Print SQL and Exit" << endl;
        cout << "> ";
        cin >> usrChoice;

        try {
            usrSelection = stoi( usrChoice ); //stoi converts a string to an integer
        } catch ( std::invalid_argument ) { //if they put in a string, we catch the exception
            cout << "Enter a number" << endl;
        }
        if ( usrSelection <= ( assetVector.size() + 3 ) && usrSelection > 0 ) //on a valid input, toggle the boolean
            validChoice = true;
        else
            cout << "Invalid Menu Selection" << endl;

    } while ( !validChoice );

    usrSelection--; //Return to zero indexing
    return usrSelection;

}

string addAsset() {
    string assetName{};
    cout << "Please name your new asset:" << endl;
    cout << "> ";
    cin >> assetName;
    return assetName;
}

int qualityMangager( int id, vector<quality> &qualities /*The & sign means to pass by reference*/) {
    /*
     * For the purposes of adding/deleting, I will be
     * Creating an vector. Every time I find a valid id
     * I add it's global index (the one in the vector) to the array.
     * This way, the user can simply delete with a 1/2/3... instead of
     * Having to deal with weird numbers.
     */
    vector<int> indexTracker{};
    bool validInput{false};
    int usrChoice{-1};

    cin.clear(); //Just to be on the safe side...

    cout << "Select an existing value to delete it" << endl;
    do {
        //Print out current values --------------------------------------------------------------------
        int enhancedIdx{0}; //Index for the for loop

        for ( quality iterator : qualities ) { //for every quality in the vector

            if ( iterator.id == id ) { //if the id matches our current asset
                cout << indexTracker.size() + 1 << ") " << iterator.type << " : " << iterator.value << endl;
                indexTracker.push_back( enhancedIdx );
            }

            enhancedIdx++; //increment the index
        }

        cout << indexTracker.size() + 1 << ") Add Quality" << endl;
        cout << indexTracker.size() + 2 << ") Go Back" << endl;

        //Select a user option --------------------------------------------------------------------------
        //This is the same menu code I use everywhere. For a reason.
        string usrInput{};

        cout << "> ";
        cin >> usrInput;
        cin.ignore();
        cin.sync(); //Since we will use getline later, we need to to do this. Trust Me.

        try {
            usrChoice = stoi( usrInput ); //stoi converts a string to an integer
        } catch ( std::invalid_argument ) { //if they put in a string, we catch the exception
            cout << "Enter a number" << endl;
        }
        if ( usrChoice <= ( indexTracker.size() + 2 ) && usrChoice > 0 ) //on a valid input, toggle the boolean
            validInput = true;
        else
            cout << "Invalid Menu Selection" << endl;

    } while ( !validInput );


    //AFTER MENU------------------------------------------------------------------------------------------------------

    if ( usrChoice == indexTracker.size() + 1 ) { //Create a new quality
        string type{}; //store the type
        string value{}; //store the value
        cout << "Create a new quality:" << endl;
        cout << "Give your quality a type:" << endl;
        cout << "> ";
        getline( cin, type );
        cout << "Give your quality a value:" << endl;
        cout << "> ";
        getline( cin, value );
        //Fix the input
        type = fixString( type );
        value = fixString( value );

        //Create a new quality struct
        quality newQual;
        newQual.id = id;
        newQual.type = type;
        newQual.value = value;

        qualities.push_back( newQual ); //Add the quality to the vector
    } else if ( usrChoice == indexTracker.size() + 2 ) {

    }
    else { //Modify an existing quality
        int eraseId = indexTracker.at( usrChoice - 1 ); //the -1 is for zero indexing
        /*
         * The reason I can't just pass this eraseID is that
         * vector.erase requires an iterator. Doing this gets us
         * an iterator.
         */
        qualities.erase( qualities.begin() + eraseId );
    }

    return 0;
}

string fixString( string fixMe ) {
    //lowercase all and remove spaces
    for ( int idx = 0; idx < fixMe.length(); idx++ ) {
        fixMe[idx] = tolower( fixMe[idx] );
        if ( fixMe[idx] == 32 )
            fixMe[idx] = 95;
    }
    return fixMe;

}

int topolgyManager( vector<string> assets, vector<topology> &topologies ) {
    cout << "Current Topologies" << endl;
    cout << "Select and existing topology to remove it" << endl;
    bool validChoice{false};
    int usrSelection{-1};
    string usrChoice{};

    do {
        int extIdx{0};

        for ( extIdx; extIdx < topologies.size(); extIdx++ ) { //Print current topologies
            string asset1 = assets.at( topologies[extIdx].asset1 - 1 );
            string asset2 = assets.at( topologies[extIdx].asset2 - 1 );
            string desrip = topologies[extIdx].relationship;
            cout << extIdx + 1 << ") " << asset1 << " <-> " << asset2 << " : " << desrip << endl;
        }

        cout << extIdx + 1 << ") Create New" << endl;
        cout << extIdx + 2 << ") Go Back" << endl;
        cout << "> ";
        cin >> usrChoice;
        try {
            usrSelection = stoi( usrChoice ); //stoi converts a string to an integer
        } catch ( std::invalid_argument ) { //if they put in a string, we catch the exception
            cout << "Enter a number" << endl;
        }
        if ( usrSelection <= ( topologies.size() + 3 ) && usrSelection > 0 ) //on a valid input, toggle the boolean
            validChoice = true;
        else
            cout << "Invalid Menu Selection" << endl;

    } while ( !validChoice );

    if ( usrSelection == topologies.size() + 2 ) {
    } else if ( usrSelection == topologies.size() + 1 ) {
        //new topology
        cout << "Create a new topology" << endl;
        bool firstAsset{false};
        bool secondAsset{false};
        string asset1Str{};
        int asset1{-1};
        string asset2Str{};
        int asset2{-1};

        //ASSET 1 ------------------------------------------------
        do {
            cout << "Select your first asset" << endl;

            for ( int idx = 0; idx < assets.size(); idx++ ) {
                cout << idx + 1 << ") " << assets[idx] << endl;
            }

            cout << "> ";
            cin >> asset1Str;
            try {
                asset1 = stoi( asset1Str ); //stoi converts a string to an integer
            } catch ( std::invalid_argument ) { //if they put in a string, we catch the exception
                cout << "Enter a number" << endl;
            }
            if ( asset1 <= ( assets.size()) && asset1 > 0 ) //on a valid input, toggle the boolean
                firstAsset = true;
            else
                cout << "Invalid Menu Selection" << endl;
        } while ( !firstAsset );

        //ASSET 2 ------------------------------------------------------------

        do {
            cout << "Select your second asset" << endl;

            for ( int idx = 0; idx < assets.size(); idx++ ) {
                cout << idx + 1 << ") " << assets[idx] << endl;
            }

            cout << "> ";
            cin >> asset2Str;
            cin.ignore(); //Since getline is coming up soon
            try {
                asset2 = stoi( asset2Str ); //stoi converts a string to an integer
            } catch ( std::invalid_argument ) { //if they put in a string, we catch the exception
                cout << "Enter a number" << endl;
            }
            if ( asset2 <= ( assets.size()) && asset2 > 0 ) //on a valid input, toggle the boolean
                secondAsset = true;
            else
                cout << "Invalid Menu Selection" << endl;
        } while ( !secondAsset );

        cout << "And set their relationship in a comma-seperated list" << endl;
        cout << "> ";
        string relationship{};
        getline( cin, relationship );
        relationship = fixString( relationship );

        topology newTop;
        newTop.asset1 = asset1;
        newTop.asset2 = asset2;
        newTop.relationship = relationship;

        topologies.push_back( newTop );


    } else { //remove existing
        topologies.erase( topologies.begin() + ( usrSelection - 1 ));
    }
    return 0;
}


void printSql( string networkName, vector<string> assets, vector<quality> qualities, vector<topology> topologies ) {

//Tons of print statements incoming :-(
    cout << "INSERT INTO network VALUES" << endl;
    cout << "\t(DEFAULT, '" << networkName << "');" << endl;
    //Assets next
    cout << "\nINSERT INTO asset VALUES" << endl;
    for ( int idx = 0; idx < assets.size(); idx++ ) {
        cout << "\t(DEFAULT, '" << assets[idx] << "', (SELECT id FROM network WHERE name = '" << networkName << "'))";
        if ( idx != assets.size() - 1 )
            cout << "," << endl;
        else
            cout << ";" << endl;
    }
    //" << << "
    //Qualities
    cout << "\nINSERT INTO quality VALUES" << endl;
    for ( int idx = 0; idx < qualities.size(); idx++ ) {
        quality qual = qualities[idx];
        cout << "\t(" << qual.id + 1 << ", '" << qual.type << "', '" << qual.value << "')";
        if ( idx != qualities.size() - 1 )
            cout << "," << endl;
        else
            cout << ";" << endl;
    }

    //Topolgies
    cout << "\nINSERT INTO topology VALUES" << endl;
    for ( int idx = 0; idx < topologies.size(); idx++ ) {
        topology top = topologies[idx];
        cout << "\t(" << top.asset1 << ", " << top.asset2 << ", '" << top.relationship << "')";
        if ( idx != topologies.size() - 1 )
            cout << "," << endl;
        else
            cout << ";" << endl;
    }

    return;

}
