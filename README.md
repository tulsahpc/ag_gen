# ag_gen: Attack Graph Generation System

## Installation

- Build system: clang/gcc, make/cmake
- Dependencies: PostgreSQL, Redis
- Tools: editorconfig, cppcheck

### Ubuntu

	sudo apt update && sudo apt install -y build-essentials \
		postgresql postgresql-server postgresql-contrib libpq5 \
		libpq-dev redis-server redis-tools libhiredis0.13 libhiredis-dev

### Mac OSX

	xcode-select --install # only if Xcode is not already installed
	brew install postgresql redis
