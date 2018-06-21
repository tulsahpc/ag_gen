//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_QUALITY_H
#define AG_GEN_QUALITY_H

#include "util/keyvalue.h"
#include <string>

union EncodedQuality {
    struct {
        int asset_id : 32;
        int attr : 12;
        int op : 4;
        int val : 16;
    } dec;
    size_t enc;
};

/** Quality class
 * @brief Holds information for the Quality
 */
class Quality {
    int asset_id;
    std::string name;
    std::string op;
    std::string value;

    friend class Factbase;

  public:
    Quality(int assetId, std::string qualName, std::string op,
            std::string qualValue);

    std::string get_name() const;
    std::string get_op() const;

    void print() const;

    EncodedQuality encode(const Keyvalue &kv_facts) const;

    bool operator==(const Quality &rhs) const;
    bool operator<(const Quality &rhs) const;
};

#endif // AG_GEN_QUALITY_H
