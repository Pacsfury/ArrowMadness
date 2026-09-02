#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../include/datakeeper.hpp"

void Table::save(std::string K, std::string V) { KV[K] = V; }

std::string Table::get(std::string K) {
    auto it = KV.find(K);
    return it != KV.end() ? it->second : "";
}

Table::Table(int id) { this->id = id; }

void Table::file() {
    std::ofstream table("_ArrowMadnessUserData.txt");

    for (const auto& [K, V] : this->KV) {
        table << K << ":" << V << ";\n";
    }
}

void Table::reset() { this->KV.clear(); }

void Table::load() {
    std::ifstream table("_ArrowMadnessUserData.txt");
    if (!table.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(table, line)) {
        std::stringstream ss(line);
        std::string Kbuf;
        std::string Vbuf;

        if (std::getline(ss, Kbuf, ':') && std::getline(ss, Vbuf, ';')) {
            this->KV[Kbuf] = Vbuf;
        }
    }
}