//
// Created by Kyle Cook on 5/26/16.
//

#ifndef C_UTIL_H
#define C_UTIL_H

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

// Debug print helper
#define DEBUG_PRINT(fmt, args...) \
    do { if (DEBUG_TEST) fprintf(stderr, "DEBUG: %s:%d %s(): " fmt, __FILE__, \
    __LINE__, __FUNCTION__, ##args); } while (0)

#endif //C_UTIL_H
