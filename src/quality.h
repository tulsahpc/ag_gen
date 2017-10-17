//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_QUALITY_H
#define AG_GEN_QUALITY_H

#include <string>

#include "network_state.h"
#include "keyvalue.h"

union EncodedQuality {
    struct {
        int asset_id : 16;
        int attr : 12;
        int op : 4;
        int val : 16;
    } dec;
    size_t enc;
};

class Quality {
	const NetworkState &parent;

    int asset_id;
    std::string name;
    std::string op;
    std::string value;

    friend class Factbase;

public:
    Quality(const NetworkState &ns, int assetId, std::string qualName, std::string op, std::string qualValue);
	Quality(const NetworkState &ns, size_t fact);

    std::string get_name();
    std::string get_op();

    void print() const;

    EncodedQuality encode() const;

    bool operator==(const Quality &rhs) const;
    bool operator<(const Quality &rhs) const;

    static std::vector<Quality> fetch_all(const NetworkState &ns);
};

#endif //AG_GEN_QUALITY_H
