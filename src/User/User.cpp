#include "../../include/User/User.hpp"

User::User(const std::string& id, const std::string& username, const std::string& role, const std::string& password) : 
id(id), username(username), role(role), password(password){}

std::string User::getId() const
{
    return id;
}

std::string User::getUsername() const
{
    return username;
}

std::string User::getRole() const
{
    return role;
}

std::pair<bool, std::string> User::login(const std::string &username, const std::string &password, const std::string &role)
{
    nlohmann::json usersJson = JsonUtils::readJsonFromFile("data/users.json");

    for (const auto &user : usersJson)
    {
        if (user["username"] == username && user["password"] == password && user["role"] == role)
        {
            return {true, user["id"]}; // Return success and user ID
        }
    }

    return {false, ""}; // Authentication failed
}

void User::setRole(const std::string &newRole)
{
    std::string roleLower = Utils::toLowerCase(newRole);
    if (roleLower == "administrator" || roleLower == "booking agent" || roleLower == "passenger")
    {
        role = newRole;
    }
}

void User::logout()
{
    std::cout << "Logged out Successfuly" << std::endl;
}


User User::fromJson(const nlohmann::json &j)
{
    return User(
        j.at("id").get<std::string>(),
        j.at("username").get<std::string>(),
        j.at("role").get<std::string>(),
        j.at("password").get<std::string>());
}

nlohmann::json User::toJson() const
{
    return {
        {"id", id},
        {"username", username},
        {"password", password},
        {"role", role}};
}