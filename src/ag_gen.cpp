#include <iostream>
#include <fstream>
#include <getopt.h>
#include <memory>
#include <cstdlib>
#include <stdlib.h>
#include <libltdl/lt_system.h>
#include <sstream>

#include "network.hpp"
#include "asset.hpp"
#include "exploit.hpp"
#include "util_db.h"


using namespace std;

string read_config();

#define CONNINFO "postgresql://archlord@localhost/ag_gen_test"

class NetworkState {
    int id;
    int network_id;
    int parent_id;
public:
    NetworkState(Network &);
};

union TestQuality {
    struct {
        int asset : 8;
        int attr : 8;
        int val : 16;
    } dec;
    int enc;
};

enum assets {
    ROUTER, WORKSTATION, PRINTER
};

enum attributes {
    OS, VERSION, ROOT
};

enum values {
    WINDOWS, CISCO, HP, XP, THREE
};

NetworkState::NetworkState(Network &net)
{
    id = net.id;
    network_id = net.id;
    parent_id = 0;
}

static void print_usage() {
    cout << "Usage: ag_gen [OPTION...]" << endl << endl;
    cout << "Flags:" << endl;
    cout << "\t-n\tNetwork model name to generate attack graph on." << endl;
    cout << "\t-p\tPrint information about the network specified by -n." << endl;
    cout << "\t-h\tThis help menu." << endl;
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        print_usage();
        return 0;
    }

    int opt_print = 0;
    string opt_network;

    int opt;
    while((opt = getopt(argc, argv, "hpn:")) != -1) {
        switch(opt) {
            case 'h':
                print_usage();
                return 0;
            case 'n':
                opt_network = optarg;
                break;
            case 'p':
                opt_print = 1;
                break;
            case '?':
                if(optopt == 'c')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                exit(EXIT_FAILURE);
                break;
            case ':':
                fprintf(stderr, "wtf\n");
                exit(EXIT_FAILURE);
                break;
            default:
                fprintf(stderr, "Unknown option -%c. Exiting.\n", optopt);
                exit(EXIT_FAILURE);
                break;
        }
    }

    vector<TestQuality> factbase;
    TestQuality t1, t2, t3, t4, t5;
    t1.dec = {WORKSTATION,OS,WINDOWS};
    t2.dec = {WORKSTATION,VERSION,XP};
    t3.dec = {ROUTER,OS,CISCO};
    t4.dec = {ROUTER,VERSION,THREE};
    t5.dec = {PRINTER,OS,HP};

    printf("t1: %d\n", t1.enc);
    printf("t2: %d\n", t2.enc);
    printf("t3: %d\n", t3.enc);
    printf("t4: %d\n", t4.enc);
    printf("t5: %d\n", t5.enc);

    string conn_info = read_config();

}

string read_config(){

    string conn_info = "postgresql://";
    ifstream config_file ("config");

    if (!config_file.is_open())
        cout << "Unable to open file";
    else {
        char delim = ' ';

        /*
         * db_line: the entire line of the file
         * stringstream db_stream lets us parse the string as a stream
         * vector<string> db_tokens give us a place to store the delmilited strings
         * We split at the space and push both into the vector
         * Then assign the second part of the vector to the value db_name
         */

        string db_line;
        getline( config_file, db_line );
        stringstream db_stream(db_line);
        string db_name;
        vector<string> db_tokens;
        while ( getline(db_stream, db_name, delim)) {
            db_tokens.push_back(db_name);
        }
        db_name = db_tokens.at(1);

        string server_line;
        getline( config_file, server_line );
        stringstream server_stream(server_line);
        string server_name;
        vector<string> server_tokens;
        while ( getline(server_stream, server_name, delim)) {
            server_tokens.push_back(server_name);
        }
        server_name = server_tokens.at(1);
        //cout << "Server name: " << server_name << endl;

        string login_line;
        getline( config_file, login_line );
        stringstream login_stream(login_line);
        string login_name;
        vector<string> login_tokens;
        while ( getline(login_stream, login_name, delim)) {
            login_tokens.push_back(login_name);
        }
        login_name = login_tokens.at(1);

        string pass_line;
        getline( config_file, pass_line );
        stringstream pass_stream(pass_line);
        string pass_name;
        vector<string> pass_tokens;
        while ( getline(pass_stream, pass_name, delim)) {
            pass_tokens.push_back(pass_name);
        }
        pass_name = pass_tokens.at(1);

        conn_info += login_name;
        conn_info += "@";
        conn_info += server_name;
        conn_info += "/";
        conn_info += db_name;

        config_file.close();

    }

    return conn_info;
}
