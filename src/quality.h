//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_QUALITY_H
#define AG_GEN_QUALITY_H

union EncodedQuality {
    struct {
        int asset_id : 32;
        int attr : 16;
        int val : 16;
    } dec;
    size_t enc;
};

class Quality {
    int asset_id;
    std::string name;
    std::string value;

    friend class Factbase;
public:
    Quality(int assetId, const std::string &qualName, const std::string &qualValue);
	Quality(size_t fact);

    const std::string get_name(void) const;

    void print(void) const;
    const EncodedQuality encode(void) const;
    bool operator==(const Quality &rhs) const;

    static std::vector<const Quality> fetch_all(void);
    static std::vector<std::string> fetch_all_attributes(void);
    static std::vector<std::string> fetch_all_values(void);
};

#endif //AG_GEN_QUALITY_H
