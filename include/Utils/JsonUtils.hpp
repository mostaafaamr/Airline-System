#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>

class JsonUtils
{
public:
    // Reads JSON data from a file
    static nlohmann::json readJsonFromFile(const std::string &filename);

    // Saves JSON data to a file (overwrites the entire file)
    static void saveJsonToFile(const nlohmann::json &data, const std::string &filename);

    // Deletes an entry from a JSON file by key and ID
    static bool deleteFromJsonFile(const std::string &filename, const std::string &key, const std::string &id);
};

#endif 