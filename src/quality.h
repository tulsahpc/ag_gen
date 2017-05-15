//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_QUALITY_H
#define AG_GEN_QUALITY_H

#include <string>

union EncodedQuality {
    struct {
        int asset_id : 32;
        int attr : 16;
        int val : 16;
    } dec;
    int enc;
};

class Quality {
    int asset_id;
    std::string name;
    std::string value;

public:
    Quality(int assetId, const std::string &qualName, const std::string &qualValue);
    const std::string get_name(void) const;

    void print(void);
    int encoded(void);
    bool operator==(const Quality &rhs) const;

    static std::vector<const Quality> fetch_all(void);
    static std::vector<std::string> fetch_all_attributes(void);
    static std::vector<std::string> fetch_all_values(void);
};

#endif //AG_GEN_QUALITY_H
