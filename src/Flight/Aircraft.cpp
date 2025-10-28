#include "../../include/Flight/Aircraft.hpp"

Aircraft::Aircraft(const std::string& aircraftId,const std::string& aircraftType,int capacity,
    const std::string& maintenanceDue, const std::string& status):
aircraftId(aircraftId),aircraftType(aircraftType),capacity(capacity),maintenanceDue(maintenanceDue)
,status(status),utilization(0.0) 
{
    loadMaintenanceLogs();
    loadMaintenanceSchedule();
}

void Aircraft::loadMaintenanceLogs() 
{
    try
    {
        // Read the JSON data from the file
        auto jsonData = JsonUtils::readJsonFromFile("data/maintenance_logs.json");

        // Check if jsonData is an array and contains at least one object
        if (jsonData.is_array() && !jsonData.empty())
        {
            // Get the first object in the array
            const auto &logsObject = jsonData[0];

            // Check if logsObject contains the aircraftId
            if (logsObject.is_object() && logsObject.contains(aircraftId))
            {
                const auto &logs = logsObject[aircraftId]["maintenance_logs"];

                // Ensure logs is an array
                if (logs.is_array())
                {
                    // Clear existing logs
                    maintenanceLogs.clear();

                    // Parse each log entry
                    for (const auto &log : logs)
                    {
                        if (log.contains("date") && log.contains("description"))
                        {
                            maintenanceLogs.push_back({log["date"], log["description"]});
                        }
                    }
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error loading maintenance logs: " << e.what() << std::endl;
    }
}

void Aircraft::loadMaintenanceSchedule() 
{
    try
    {
        // Read the JSON data from the file
        auto jsonData = JsonUtils::readJsonFromFile("data/maintenance_schedule.json");

        // Check if jsonData is an object and contains the aircraftId
        if (jsonData.is_object() && jsonData.contains(aircraftId))
        {
            // Get the schedule for this aircraft
            const auto &schedule = jsonData[aircraftId]["schedule"];

            // Ensure schedule is an array
            if (schedule.is_array())
            {
                // Clear existing schedule
                maintenanceSchedule.clear();

                // Parse each schedule entry
                for (const auto &entry : schedule)
                {
                    if (entry.contains("date") && entry.contains("description"))
                    {
                        maintenanceSchedule.push_back(entry["date"]);
                    }
                    
                }
            }
           
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error loading maintenance schedule: " << e.what() << std::endl;
    }
}


void Aircraft::saveMaintenanceLogs() const
{
    try
    {
        // Read existing JSON data from the file
        nlohmann::json jsonData = JsonUtils::readJsonFromFile("data/maintenance_logs.json");

        // Ensure jsonData is an array and has at least one object
        if (!jsonData.is_array() || jsonData.empty())
        {
            jsonData = nlohmann::json::array(); // Initialize as an empty array if needed
            jsonData.push_back(nlohmann::json::object()); // Add an empty object
        }

        // Access the first object in the array
        auto &logsObject = jsonData[0];

        // Clear existing logs before adding new ones
        logsObject[aircraftId]["maintenance_logs"] = nlohmann::json::array();
        
        // Update or create the maintenance logs for this aircraft
        if (!logsObject.contains(aircraftId))
        {
            logsObject[aircraftId] = {
                {"maintenance_logs", nlohmann::json::array()},
                {"type", aircraftType} // Set the type based on your requirements
            };
        }

        // Add maintenance logs to the JSON array
        for (const auto &log : maintenanceLogs)
        {
            logsObject[aircraftId]["maintenance_logs"].push_back({
                {"date", log.first},
                {"description", log.second}
            });
        }

        // Save the updated JSON data
        JsonUtils::saveJsonToFile(jsonData, "data/maintenance_logs.json");
    }
    catch (const std::exception &e)
    {
        std::cout << "Error saving maintenance logs: " << e.what() << std::endl;
    }
}


void Aircraft::saveMaintenanceSchedule() const 
{
    auto jsonData = JsonUtils::readJsonFromFile("data/maintenance_schedule.json");

    jsonData[aircraftId]["type"] = aircraftType;
    jsonData[aircraftId]["schedule"] = nlohmann::json::array();
    for (const auto &schedule : maintenanceSchedule) 
    {
        jsonData[aircraftId]["schedule"].push_back(schedule);
    }
    JsonUtils::saveJsonToFile(jsonData,"data/maintenance_schedule.json");
}

void Aircraft::setCapacity(int newCapacity) 
{
    if (newCapacity <= 0) {
        throw std::invalid_argument("Capacity must be greater than zero");
    }
    capacity = newCapacity;
}

void Aircraft::addMaintenanceLog(const std::string& description, const std::string& date)  
{
    maintenanceLogs.emplace_back(date, description);
    saveMaintenanceLogs(); 
}

void Aircraft::addMaintenanceSchedule(const std::string& date)  
{
    maintenanceSchedule.push_back(date);
    saveMaintenanceSchedule(); 
}

// Remove a maintenance date
void Aircraft::removeMaintenanceSchedule(const std::string& date)
{
    auto it = std::remove(maintenanceSchedule.begin(), maintenanceSchedule.end(), date);
    if (it != maintenanceSchedule.end()) {
        maintenanceSchedule.erase(it, maintenanceSchedule.end());
    }
    saveMaintenanceSchedule();
}





Aircraft Aircraft::fromJson(const nlohmann::json& j) {
    return Aircraft(
        j.at("aircraftId").get<std::string>(),
        j.at("aircraftType").get<std::string>(),
        j.at("capacity").get<int>(),
        j.at("maintenanceDue").get<std::string>(),
        j.at("status").get<std::string>()
    );
}

nlohmann::json Aircraft::toJson() const{
    return {
        {"aircraftId", aircraftId},
        {"aircraftType", aircraftType},
        {"capacity", capacity},
        {"maintenanceDue", maintenanceDue},
        {"status", status}
    };
}