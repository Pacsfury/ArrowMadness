#include <string>
#include "../include/requests.hpp"

int createRoom(const std::string& name, const std::string& roomId, int points) {
    std::string json_body = "{\"name\":\"" + name + "\", \"roomId\":\"" + roomId + "\", \"score\":" + std::to_string(points) + "}";
    
    std::string response = post(SERVER_URL, "/api/newroom", json_body);
    
    if (response.find("\"status\":\"success\"") == std::string::npos) {
        return 0;
    }
    
    return 1;
}

std::string getRanking(const std::string& roomId) {
    std::string endpoint = "/api/ranking?roomId=" + roomId;
    
    std::string response = get(SERVER_URL, endpoint);
    
    return response; 
}
