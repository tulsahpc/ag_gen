#ifndef FACT_HPP
#define FACT_HPP

#include <iostream>
#include "asset.hpp"

class Asset;

class Fact {
public:
    virtual void print() = 0;
};

class Topology : public Fact {
    Asset& from;
    Asset& to;
    std::string status;
public:
    Topology(Asset& fromAsset, Asset& toAsset, std::string const& topStatus);
    void print();
};

class Quality : public Fact {
public:
	int asset_id;
    std::string name;
    std::string value;

    Quality(int asset, std::string const& qualName, std::string const& qualValue);

    int fetch(int asset_id);
    void print();
};

#endif // FACT_HPP
