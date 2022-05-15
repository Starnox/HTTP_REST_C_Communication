#include "jsonparser.h"

void test() {
    json test;
    test["foo"] = 23;
    
    for(auto & [key, value] : test.items()) {
        std::cout << key << " " << value << "\n";
    }
}