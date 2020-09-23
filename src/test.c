#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include "ctest.h"

CTEST(sd, test1) {
}

int main(int argc, const char *argv[]) {
    return ctest_main(argc, argv);
}
