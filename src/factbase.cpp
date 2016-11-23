#include <vector>
#include "factbase.hpp"

Factbase::Factbase(void) {}

int Factbase::check(int key) {
	for(int i=0; i<db.size(); i++) {
		if(db[i].key == key) {
			return i;
		}
	}
	return false;
}

void Factbase::add(int key, int val) {
	KeyVal new_keyval;
	new_keyval.key = key;
	new_keyval.val = val;

	db.push_back(new_keyval);
}

int Factbase::get(int key) {
	for(auto &kv : db) {
		if(key == kv.key) {
			return kv.val;
		}
	}
	return -1;
}

int Factbase::remove(int key) {
	for(auto kv=db.begin(); kv!=db.end(); kv++) {
		if(key == kv->key) {
			auto val = kv->val;
			db.erase(kv);
			return val;
		}
	}

	return 0;
}
