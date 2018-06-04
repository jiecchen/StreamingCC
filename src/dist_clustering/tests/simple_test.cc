// test file for simple.cc

#include "../algorithms/simple.h"

#include "../lest.hpp"

using namespace std;

const lest::test specification[] = {
    CASE("Add(a, b)"){EXPECT(simple::add(1, 1) == 2);
}
}
;

int main() { return lest::run(specification); }
