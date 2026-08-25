#include <string>
#include <unordered_map>

class Table {
private:
    std::unordered_map<std::string, std::string> KV;
    int id;

public:
    void save(std::string K, std::string V);
    std::string get(std::string K);
    void file();
    void reset();
    void load();
    Table(int id);
};