all:
	flex nm_scanner.ll
	/usr/local/opt/bison/bin/bison nm_parser.ypp
	g++ --std=c++11 -o nm main.cpp nm_driver.cpp nm_parser.tab.cpp nm_scanner.cpp

clean:
	@rm -rf nm_scanner.cpp scanner nm_parser.tab.cpp nm_parser.tab.hpp nm
