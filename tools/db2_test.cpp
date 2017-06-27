//
// Created by Kyle Cook on 6/27/17.
//

#include <iostream>
#include <vector>
#include <string>

#include "../src/util_db.h"

using namespace std;

int main(int argc, char **argv) {
	DB db("postgresql://captredbeard@localhost/ag_gen");

	try {
		auto rows = db.exec("SELECT * FROM factbase;");
		for (auto &row : rows) {

		}
	} catch (DBException& e) {
		cout << e.what() << endl;
	}
}
