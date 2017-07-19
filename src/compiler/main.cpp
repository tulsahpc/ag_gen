#include "nm_driver.hpp"

int main(int argc, char** argv) {
    NM::NM_Driver driver;
    driver.parse("../../examples/SystemV8.nm");
}
