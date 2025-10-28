#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

class ActivityLogger
{
public:
    ActivityLogger()=default;

    void logActivity(const std::string& userId, const std::string& role, const std::string& action, const std::string& details = "");
};


#endif