#include "netMod.h"

int genNetModel()
{
	string netModName{};
	vector<string> assetVector{};
	vector<quality> qualities{};
	vector<topology> topologies{};

	cout << "Please name your network model" << endl;
	cout << "> ";
	cin >> netModName;

	bool readyToPrint{false};
	do {
		int choice = selectAsset(assetVector);
		cout << "Choice" << choice << endl;
		if (choice == assetVector.size()) {
			string newAsset = addAsset();
			newAsset = fixString(newAsset);
			assetVector.push_back(newAsset);
		} else if (choice == assetVector.size() + 1) {
			topologyManager(assetVector, topologies);
		} else if (choice == assetVector.size() + 2) {
			readyToPrint = true;
		} else {
			qualityManager(choice, qualities);
		}
	} while (!readyToPrint);

	cout << "SQL Commands:" << endl;
	cout << "-----------------------------------------------" << endl;
	printSql( netModName, assetVector, qualities, topologies );
	cout << "-----------------------------------------------" << endl;

	return 0;
}

int selectAsset(vector<string> assetVector)
{
	bool validChoice{false};
	string usrChoice{};
	int usrSelection{-1};

	do {
		cout << "Select an asset to modify:" << endl;

		int enhancedIdx{0};
		for ( string asset : assetVector ) {
			cout << "\t" << enhancedIdx + 1 << ") " << asset << endl;
			enhancedIdx++;
		}

		cout << enhancedIdx + 1 << ") Create New" << endl;
		cout << enhancedIdx + 2 << ") Topolgy Manager" << endl;
		cout << enhancedIdx + 3 << ") Print SQL and Exit" << endl;
		cout << "> ";
		cin >> usrChoice;

		try {
		    usrSelection = stoi(usrChoice);
		} catch (std::invalid_argument) {
		    cout << "Enter a number" << endl;
		}

		if (usrSelection <= (assetVector.size() + 3) && usrSelection > 0) {
		    validChoice = true;
		} else {
		    cout << "Invalid Menu Selection" << endl;
		}
	} while (!validChoice);

	// Decrement for use in zero-indexed structure
	return usrSelection--;

}

string addAsset()
{
	string assetName{};
	cout << "Please name your new asset:" << endl;
	cout << "> ";
	cin >> assetName;
	return assetName;
}

int qualityManager(int id, vector<quality> &qualities)
{
	/*
	 * For the purposes of adding/deleting, I will be
	 * creating an vector. Every time I find a valid id
	 * I add it's global index (the one in the vector) to the array.
	 * This way, the user can simply delete with a 1/2/3... instead of
	 * having to deal with weird numbers.
	 */
	vector<int> indexTracker{};
	bool validInput{false};
	int usrChoice{-1};

	// Ensure stdin is cleared.
	cin.clear();

	cout << "Select an existing value to delete it" << endl;
	do {
		// Print out current values --------------------------------------------------------------------
		int enhancedIdx{0};
		for (quality iterator : qualities) {
			if (iterator.id == id) {
				cout << indexTracker.size() + 1 << ") " << iterator.type << " : " << iterator.value << endl;
				indexTracker.push_back(enhancedIdx++);
			}
		}

		cout << indexTracker.size() + 1 << ") Add Quality" << endl;
		cout << indexTracker.size() + 2 << ") Go Back" << endl;

		// Select a user option --------------------------------------------------------------------------
		string usrInput{};

		cout << "> ";
		cin >> usrInput;

		// Hack for getline
		cin.ignore();
		cin.sync();

		try {
			usrChoice = stoi(usrInput);
		} catch (std::invalid_argument) {
			cout << "Enter a number" << endl;
		}

		if (usrChoice <= (indexTracker.size() + 2) && usrChoice > 0)  {
			validInput = true;
		} else {
			cout << "Invalid Menu Selection" << endl;
		}

	} while (!validInput);


	// AFTER MENU------------------------------------------------------------------------------------------------------

	if (usrChoice == indexTracker.size() + 1) {
		string type{};
		string value{};

		cout << "Create a new quality:" << endl;
		cout << "Give your quality a type:" << endl;
		cout << "> ";
		getline( cin, type );

		cout << "Give your quality a value:" << endl;
		cout << "> ";
		getline( cin, value );

		// Format input correctly
		type = fixString(type);
		value = fixString(value);

		quality newQual;
		newQual.id = id;
		newQual.type = type;
		newQual.value = value;

		qualities.push_back(newQual);
	} else if ( usrChoice == indexTracker.size() + 2 ) {
	} else {
		// Zero indexing weirdness again
		int eraseId = indexTracker.at( usrChoice - 1 );
		qualities.erase(qualities.begin() + eraseId);
	}

    return 0;
}

/*
 * fixString formats all input strings as to
 * ensure the data is consistent in Redis.
 */
string fixString(string fixMe)
{
	for (int idx = 0; idx < fixMe.length(); idx++) {
		fixMe[idx] = tolower( fixMe[idx] );
		if ( fixMe[idx] == 32 ) {
			fixMe[idx] = 95;
		}
	}
	return fixMe;
}

int topologyManager(vector<string> assets, vector<topology> &topologies)
{
	bool validChoice{false};
	int usrSelection{-1};
	string usrChoice{};

	cout << "Current Topologies" << endl;
	cout << "Select and existing topology to remove it" << endl;

	do {
		int extIdx{};
		for (extIdx = 0; extIdx < topologies.size(); extIdx++ ) {
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
			usrSelection = stoi(usrChoice);
		} catch (std::invalid_argument) {
			cout << "Enter a number" << endl;
		}

		if (usrSelection <= (topologies.size() + 3) && usrSelection > 0) {
			validChoice = true;
		} else {
			cout << "Invalid Menu Selection" << endl;
		}

	} while (!validChoice);

	if ( usrSelection == topologies.size() + 2 ) {
	} else if ( usrSelection == topologies.size() + 1 ) {
		bool firstAsset{false};
		bool secondAsset{false};

		string asset1Str{};
		int asset1{-1};
		string asset2Str{};
		int asset2{-1};

		cout << "Create a new topology" << endl;

		// ASSET 1 ------------------------------------------------
		do {
			cout << "Select your first asset" << endl;
			for (int idx = 0; idx < assets.size(); idx++) {
				cout << idx + 1 << ") " << assets[idx] << endl;
			}

			cout << "> ";
			cin >> asset1Str;

			try {
				asset1 = stoi(asset1Str);
			} catch (std::invalid_argument) {
				cout << "Enter a number" << endl;
			}

			if (asset1 <= (assets.size()) && asset1 > 0) {
				firstAsset = true;
			} else {
				cout << "Invalid Menu Selection" << endl;
			}
		} while (!firstAsset);

		// ASSET 2 ------------------------------------------------------------
		do {
			cout << "Select your second asset" << endl;
			for (int idx = 0; idx < assets.size(); idx++) {
				cout << idx + 1 << ") " << assets[idx] << endl;
			}

			cout << "> ";
			cin >> asset2Str;

			// getline hackery
			cin.ignore();

			try {
				asset2 = stoi(asset2Str);
			} catch (std::invalid_argument) {
				cout << "Enter a number" << endl;
			}
			if (asset2 <= (assets.size()) && asset2 > 0) {
				secondAsset = true;
			} else {
				cout << "Invalid Menu Selection" << endl;
			}
		} while (!secondAsset);

		cout << "And set their relationship in a comma-seperated list" << endl;
		cout << "> ";

		string relationship{};
		getline(cin, relationship);
		relationship = fixString(relationship);

		topology newTop;
		newTop.asset1 = asset1;
		newTop.asset2 = asset2;
		newTop.relationship = relationship;

		topologies.push_back(newTop);
	} else {
		topologies.erase( topologies.begin() + ( usrSelection - 1 ));
	}

	return 0;
}


void printSql(string networkName, vector<string> assets, vector<quality> qualities, vector<topology> topologies)
{
	// Tons of print statements incoming :-(
	cout << "INSERT INTO network VALUES" << endl;
	cout << "\t(DEFAULT, '" << networkName << "');" << endl;

	// Assets
	cout << "\nINSERT INTO asset VALUES" << endl;
	for (int idx = 0; idx < assets.size(); idx++) {
		cout << "\t(DEFAULT, '" << assets[idx] << "', (SELECT id FROM network WHERE name = '" << networkName << "'))";
		if ( idx != assets.size() - 1 ) {
			cout << "," << endl;
		} else {
			cout << ";" << endl;
		}
	}

	// Qualities
	cout << "\nINSERT INTO quality VALUES" << endl;
	for (int idx = 0; idx < qualities.size(); idx++) {
		quality qual = qualities[idx];
		cout << "\t(" << qual.id + 1 << ", '" << qual.type << "', '" << qual.value << "')";
		if ( idx != qualities.size() - 1 ) {
			cout << "," << endl;
		} else {
			cout << ";" << endl;
		}
	}

	// Topologies
	cout << "\nINSERT INTO topology VALUES" << endl;
	for ( int idx = 0; idx < topologies.size(); idx++ ) {
		topology top = topologies[idx];
		cout << "\t(" << top.asset1 << ", " << top.asset2 << ", '" << top.relationship << "')";
		if ( idx != topologies.size() - 1 ) {
			cout << "," << endl;
		} else {
			cout << ";" << endl;
		}
	}

	return;
}
