#ifndef USER_HPP
#define USER_HPP

#include "../Utils/JsonUtils.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <utility>
#include <iostream>
#include "../Utils/Utils.hpp"

class User
{

protected:
    std::string id;
    std::string username;
    std::string role;

private:
    std::string password;

public:
    User(const std::string& id, const std::string& username, const std::string& role, const std::string& password);
    ~User()= default;

    // Getters
    std::string getUsername() const;
    std::string getRole() const;
    std::string getId() const;

    // Setters
    void setUsername(const std::string& userName) {username = userName;}
    void setId(const std::string& Id) {id = Id;}
    void setPassword(const std::string& newPass) {password = newPass;}
    void setRole(const std::string& newRole);

    // Authentication
    static std::pair<bool, std::string> login(const std::string &username, const std::string &password, const std::string &role);
    void logout();


    // Json Serialization
    static User fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;
};






#endif