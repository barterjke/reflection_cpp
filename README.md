# C++ reflection
Custom reflection system for C++, similar to UnrealHeaderTool, used for readable serialization/deserialization

Example usage:
```c++
EXPOSE_STRUCT // macro
struct A {
    EXPOSE // another macro
    int x;

    EXPOSE
    float y;
    
    EXPOSE
    string s;
};

int main() {
    harvest("../main.cpp"); // name of the file with struct definition
    A *ptr = new A{3, 2.5f, "hello32"};
    auto s = serialize<A>("A", ptr);
    std::cout << s; /* prints out the following:
      int x 3
      float y 2.500000
      string s hello32
    */
    auto a = deserialize<A>(s);
    printf("%d %f %s", a->x, a->y, a->s.c_str());
    // prints out 3 2.500000 hello32
    return 0;
}
```

Currently, supports only 3 types: `float`, `int` and `string`, because it was created for demonstration purposes only,
but it's easily extendable to any other type or container, all you have to do is to add branch condition in `deserialize` function:
```c++
 } else if (input[i] == "string") {
    *((std::string *) (ptr + offset)) = input[i + 2];
    offset += sizeof(std::string);
} else if (input[i] == /*your type*/){
    //your code//
} else {
    std::cout << "ERROR: unknown type " << input[i] << "\n";
    throw std::invalid_argument(input[i]);
}
```
and `serialize` function as well:
```c++
 } else if (type == "string") {
    auto *ptr = (std::string *) (raw_ptr);
    ss << *ptr << "\n";
    raw_ptr += sizeof(std::string);
} else if (type == /*your type*/){
    //your code//
} else {
    std::cout << "ERROR: unknown type " << type << "\n";
    throw std::invalid_argument(type);
}
```
