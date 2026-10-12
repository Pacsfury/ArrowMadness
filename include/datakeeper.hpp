#pragma once
#include <string>
#include <unordered_map>

class Table {
private:
    std::unordered_map<std::string, std::string> KV;
    int id;

public:
    void save(const std::string& K, const std::string& V);
    std::string get(const std::string& K);
    void file();
    void reset();
    void load();
    Table(int id);
};