//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_QUALITY_H
#define AG_GEN_QUALITY_H

#include "util/keyvalue.h"
#include <string>

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
    int asset_id;
    std::string name;
    std::string op;
    std::string value;

    friend class Factbase;

  public:
    Quality(int assetId, std::string qualName, std::string op,
            std::string qualValue);
    // explicit Quality(size_t fact);

    std::string get_name() const;
    std::string get_op() const;

    void print() const;

    EncodedQuality encode(const Keyvalue &kv_facts) const;

    bool operator==(const Quality &rhs) const;
    bool operator<(const Quality &rhs) const;

    static std::vector<Quality> fetch_all();
    static std::vector<std::string> fetch_all_attributes();
    static std::vector<std::string> fetch_all_values();
};

#endif // AG_GEN_QUALITY_H
