#include "../src/compiler/nm_driver.hpp"

int main(int argv, char** argv) {
	NM::NM_Driver driver;
	driver.parse("../examples/SystemV8.nm");
}
