all: network_state_scanner exploit_scanner


network_state_scanner: network_state_scanner.cpp
	g++ -c network_state_scanner.cpp

exploit_scanner: exploit_scanner.cpp
	g++ -c exploit_scanner.cpp

exploit_scanner.cpp: exploit_parser.tab.cpp
	flex exploit_scanner.ll

exploit_parser.tab.cpp:
	bison -d exploit_parser.ypp

network_state_scanner.cpp: network_state_parser.tab.cpp
	flex network_state_scanner.ll

network_state_parser.tab.cpp:
	bison -d network_state_parser.ypp

clean:
	@rm -f exploit_scanner.cpp exploit_parser.tab.cpp exploit_parser.tab.hpp network_state_scanner.cpp network_state_parser.tab.cpp network_state_parser.tab.hpp
	@rm -f nsscanner
