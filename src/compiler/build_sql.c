#include "build_sql.h"
#include "str_util.h"

const char* sqlAsset = "(%d, '%s', (SELECT id FROM network WHERE name = 'home')),";
const char* sqlQuality = "(%d, '%s', '%s'),";
const char* sqlTopology = "(%d, %d, '%s'),";

char* make_asset(char* as) {
    size_t mystringlen = strlen(sqlAsset) + strlen(as);
    char* mystring = getstr(mystringlen);
    sprintf(mystring, sqlAsset, assetcount++, as);
    return mystring;
}
