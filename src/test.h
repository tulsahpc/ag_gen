#ifndef TEST_H
#define TEST_H

#include <stdlib.h>
#include <stdio.h>

#define PASS 1
#define FAIL 0

enum TestTypes {
	INT, CHAR_P
};

#define TEST(FUNC) { \
 	fprintf(stderr, #FUNC ": "); \
 	int res = (FUNC()); \
 	if(res) fprintf(stderr, "Success\n"); }

#define FAILIF(EXPR) if(EXPR) { fprintf(stderr, #EXPR); return FAIL; }
#define FAILIFNEQ(TYPE, NAME, VAL) \
 	if((NAME) != (VAL)) { \
 		switch(TYPE) { \
 		case INT: \
 			fprintf(stderr, "\n\t" #NAME " != " #VAL ": Failure @ %s:%d\n", __FILE__, __LINE__); \
 			break; \
 		case CHAR_P: \
 			fprintf(stderr, "\n\t" #NAME " != " #VAL ": Failure @ %s:%d\n", __FILE__, __LINE__); \
 			break; } \
 		return FAIL; } \
 	return PASS;

#define FAILIFEQ(TYPE, NAME, VAL) \
 	if((NAME) == (VAL)) { \
 		switch(TYPE) { \
 		case INT: \
 			fprintf(stderr, "\n\t" #NAME " == " #VAL ": Failure @ %s:%d\n", __FILE__, __LINE__); \
 			break; \
 		case CHAR_P: \
 			fprintf(stderr, "\n\t" #NAME " == " #VAL ": Failure @ %s:%d\n", __FILE__, __LINE__); \
 			break; } \
 		return FAIL; } \
 	return PASS;

#endif //TEST_H
