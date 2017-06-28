//Created by Kyle Ruff 6/28/17
//

#ifndef GRAPH_THE_DB_H
#define GRAPH_THE_DB_H

class Graph {
public:
        void checkSelect(PGconn *conn, PGresult *res);
        void graph_db(PGconn *conn);
};

#endif
