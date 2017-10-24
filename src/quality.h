//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_QUALITY_H
#define AG_GEN_QUALITY_H

#include "network_state.h"

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
	NetworkState parent;

    int asset_id;
    std::string name;
    std::string op;
    std::string value;

public:
	
    Quality(const NetworkState &ns, int asset, std::string qualName,
			std::string op, std::string qualValue);

	//Quality(const NetworkState &ns, size_t fact);

    std::string get_name();
    std::string get_op();

    void print() const;

    EncodedQuality encode() const;
	void decode(size_t fact);

    bool operator==(const Quality &rhs) const;
    bool operator<(const Quality &rhs) const;

    static std::vector<Quality> fetch_all(const NetworkState &ns);
};

#endif //AG_GEN_QUALITY_H
