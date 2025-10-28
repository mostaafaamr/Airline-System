#include "../../include/Utils/JsonUtils.hpp"

nlohmann::json JsonUtils::readJsonFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    nlohmann::json data;
    try
    {
        file >> data;
    }
    catch (const std::exception &e)
    {
        file.close();
        throw std::runtime_error("Failed to parse JSON from file: " + std::string(e.what()));
    }

    file.close();
    return data;
}

void JsonUtils::saveJsonToFile(const nlohmann::json &data, const std::string &filename)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // Write the JSON data to the file (overwrite)
    outputFile << data.dump(4); // Pretty-print with 4 spaces
    outputFile.close();
}

bool JsonUtils::deleteFromJsonFile(const std::string &filename, const std::string &key, const std::string &id)
{
    // Read the existing JSON data
    nlohmann::json data;
    try
    {
        data = readJsonFromFile(filename);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading file: " << e.what() << std::endl;
        return false;
    }

    // Ensure the data is an array
    if (!data.is_array())
    {
        std::cerr << "Error: JSON data is not an array." << std::endl;
        return false;
    }

    // Remove the entry with the matching key and ID
    size_t oldSize = data.size();
    data.erase(std::remove_if(data.begin(), data.end(),
                             [key, id](const nlohmann::json &item)
                             { return item.contains(key) && item[key] == id; }),
               data.end());

    // Check if an entry was removed
    if (data.size() < oldSize)
    {
        // Save the updated JSON data (overwrite the file)
        saveJsonToFile(data, filename);
        return true;
    }
    else
    {
        std::cerr << "Warning: No entry found with key '" << key << "' and ID '" << id << "'." << std::endl;
        return false;
    }
}