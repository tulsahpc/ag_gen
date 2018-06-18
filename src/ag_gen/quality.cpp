#include <iostream>
#include <vector>

#include "ag_gen.h"

using namespace std;

/**
 * @brief Constructor for Quality
 *
 * @param asset The id of the asset with which the Quality is associated
 * @param qualName The name of the Quality
 * @param o The operation
 * @param qualValue The value of the Quality
 */
Quality::Quality(int asset, string qualName, string o, string qualValue)
    : asset_id(asset), name(move(qualName)), op(o), value(move(qualValue)) {}

/**
 * @return The name of the Quality
 */
string Quality::get_name() const { return name; }

/**
 * @return The operation
 */
string Quality::get_op() const { return op; }

/**
 * @brief Prints the Quality
 */
void Quality::print() const {
    std::cout << to_string(asset_id) << ": " << name << op << value
              << std::endl;
}

/**
 * @brief Encodes the Quality
 *
 * @param kv_facts The Keyvalue of the facts
 *
 * @return The EncodedQuality
 */
EncodedQuality Quality::encode(const Keyvalue &kv_facts) const {
    EncodedQuality qual{};
    qual.dec.asset_id = asset_id;
    qual.dec.attr = kv_facts[name];
    qual.dec.val = kv_facts[value];

    return qual;
}

bool Quality::operator==(const Quality &rhs) const {
    if (this->asset_id != rhs.asset_id)
        return false;
    if (this->name != rhs.name)
        return false;
    if (this->value != rhs.value)
        return false;
    return true;
}
