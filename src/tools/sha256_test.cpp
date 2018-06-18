#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include <openssl/sha.h>
string sha256(const string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256{};
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    stringstream ss;
    for (unsigned char i : hash) {
        ss << hex << setw(2) << setfill('0') << (int)i;
    }
    ss << " " << dec << ss.str().size();
    return ss.str();
}

int main() {
    string str;
    cout << "hello there: " << sha256("hello there") << endl << endl;
    while (true) {
        cout << "Enter string to be hashed (Enter 'Stop' to end)" << endl;
        str = "";
        getline(cin, str);
        if ((str != "stop") && (str != "Stop")) {
            cout << sha256(str) << endl << endl;
        } else {
            break;
        }
    }

    return 0;
}
