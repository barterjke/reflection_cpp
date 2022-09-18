#pragma once

#include <cassert>
#include "unordered_map"

#define EXPOSE
#define EXPOSE_STRUCT

std::unordered_map<std::string, std::vector<std::tuple<std::string, std::string>>> type_info_map;

template<typename T>
T *deserialize(std::string &string_input) {
    std::vector<std::string> input = split_space(string_input);
    char *ptr = new char[sizeof(T)];
    char offset = 0;
    for (auto i = 0; i < input.size(); i += 3) {
        if (input[i] == "int") {
            *((int *) (ptr + offset)) = std::stoi(input[i + 2]);
            offset += sizeof(int);
        } else if (input[i] == "float") {
            *((float *) (ptr + offset)) = std::stof(input[i + 2]);
            offset += sizeof(float);
        } else if (input[i] == "string") {
            *((std::string *) (ptr + offset)) = input[i + 2];
            offset += sizeof(std::string);
        } else {
            std::cout << "ERROR: unknown type " << input[i] << "\n";
            throw std::invalid_argument(input[i]);
        }
    }
    return (T *) (ptr);
}

template<typename T>
std::string serialize(const std::string &struct_name, T *t_ptr) {
    auto raw_ptr = (char *) t_ptr;
    assert(type_info_map.contains(struct_name));
    auto type_info = type_info_map[struct_name];
    std::stringstream ss;
    for (auto &[type, name]: type_info) {
        ss << type << " " << name << " ";
        if (type == "int") {
            auto *ptr = (int *) raw_ptr;
            ss << std::to_string(*ptr) << "\n";
            raw_ptr += sizeof(int);
        } else if (type == "float") {
            auto *ptr = (float *) raw_ptr;
            ss << std::to_string(*ptr) << "\n";
            raw_ptr += sizeof(float);
        } else if (type == "string") {
            auto *ptr = (std::string *) (raw_ptr);
            ss << *ptr << "\n";
            raw_ptr += sizeof(std::string);
        } else {
            std::cout << "ERROR: unknown type " << type << "\n";
            throw std::invalid_argument(type);
        }
    }
    return ss.str();
}

//template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
//    auto s = overloaded {
//            [](auto arg) { std::cout << arg << ' '; },
//            [](double arg) { std::cout << std::fixed << arg << ' '; },
//            [](const std::string& arg) { std::cout << 32 << ' '; }
//    };

// possibly I can use a visitor pattern here?

//another way of doing this:
//std::unordered_map<std::string, std::function<char *(std::stringstream ss, char *raw_ptr)>> serialize_map{};
//    serialize_map = {
//            {"int", [](auto ss, auto raw_ptr) {
//                auto *ptr = (int *) raw_ptr;
//                ss << std::to_string(*ptr) << "\n";
//                return raw_ptr + sizeof(int);
//            }},
//            {"float", [](auto ss, auto raw_ptr) {
//                auto *ptr = (float *) raw_ptr;
//                ss << std::to_string(*ptr) << "\n";
//                return raw_ptr + sizeof(float);
//            }}
//    };

void harvest(const char *filename) {
    std::ifstream file(filename);
    assert(file.is_open());
    std::string line;
    std::string current_struct_name;
    std::vector<std::string> after_split;
    while (getline(file, line)) {
        auto after_strip = trim(line);
        if (after_strip == "EXPOSE_STRUCT") {
            getline(file, line);
            after_split = split_space(line);
            current_struct_name = after_split[1];
            type_info_map[current_struct_name] = std::vector<std::tuple<std::string, std::string>>();
        } else if (after_strip == "EXPOSE") {
            getline(file, line);
            after_split = split_space(line);
            auto type = after_split[0];
            auto name = after_split[1].substr(0, after_split[1].size() - 1);
            type_info_map[current_struct_name].push_back({type, name});
        }
    }
    file.close();
}