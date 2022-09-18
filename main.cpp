#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <functional>

#include "string_util.h"
#include "exposure.h"

using std::string;

EXPOSE_STRUCT
struct A {
    EXPOSE
    int x;

    EXPOSE
    float y;

    EXPOSE
    string s;

    [[nodiscard]] string str() const {
        return "A{ x int " + std::to_string(x) + "; float y " + std::to_string(y) + "; s `" + s + "` }";
    }
};

int main() {
    harvest("../main.cpp");
    A *ptr = new A{3, 2.5f, "hello32"};
    auto s = serialize<A>("A", ptr);
    std::cout << s;
    auto a = deserialize<A>(s);
    printf("%d %f %s", a->x, a->y, a->s.c_str());
    return 0;
}