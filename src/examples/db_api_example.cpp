//
// Created by Kyle Cook on 6/27/17.
//

#include <iostream>
#include <vector>

#include "util/db.h"

using namespace std;

shared_ptr<DB> db;
int main(int argc, char **argv) {
    db = make_shared<DB>("postgres://captredbeard@localhost/ag_gen");
    try {
        auto rows = db->exec("SELECT * FROM factbase;");
        for (auto &row : rows) {
            cout << "Row: ";
            for (auto &field : row) {
                cout << field << " ";
            }
            cout << endl;
        }
    } catch (DBException &e) {
        cout << e.what() << endl;
    }
}
