#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include <string>
#include <stdexcept>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>
#include "../Utils/JsonUtils.hpp"

class Aircraft
{
private:
    std::string aircraftId;
    std::string aircraftType;
    int capacity;
    std::string maintenanceDue;
    std::string status;
    std::vector<std::string> maintenanceSchedule; // List of scheduled maintenance dates
    std::vector<std::pair<std::string, std::string>> maintenanceLogs;    // Details of maintenance activities
    double utilization; // Percentage of time in use


    void loadMaintenanceLogs();
    void loadMaintenanceSchedule();
    void saveMaintenanceLogs() const;
    void saveMaintenanceSchedule() const;
public:
    Aircraft(const std::string& aircraftId,const std::string& aircraftType,int capacity,
            const std::string& maintenanceDue, const std::string& status);
    ~Aircraft() = default;

     // Getters
     std::string getId() const { return aircraftId; }
     std::string getAircraftType() const { return aircraftType; }
     int getCapacity() const { return capacity; }
     int getRows() const { return capacity/6 ;}
     std::string getMaintenanceDue() const { return maintenanceDue; }
     double getUtilization() const { return utilization; }
     std::string getStatus() const { return status; }

     // Get maintenance logs
    const std::vector<std::pair<std::string, std::string>>& getMaintenanceLogs() const { return maintenanceLogs; }

    // Get maintenance schedule
    const std::vector<std::string>& getMaintenanceSchedule() const { return maintenanceSchedule; }
 
     // Setters
     void setId(const std::string& newId) { aircraftId = newId; }
     void setAircraftType(const std::string& aircraftType) {this->aircraftType = aircraftType;}
     void setCapacity(int newCapacity);
     void setMaintenanceDue(const std::string& maintenanceDue) {this->maintenanceDue = maintenanceDue;}
     void setStatus(const std::string& newStatus) {status = newStatus;}

     // Add a maintenance log
    void addMaintenanceLog(const std::string& description, const std::string& date);

    // Schedule maintenance
    void addMaintenanceSchedule(const std::string& date);  

    // Remove a maintenance date
    void removeMaintenanceSchedule(const std::string &date);

    // Update utilization
    void updateUtilization(double hours) { utilization += hours; }

    // Serialization
    static Aircraft fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;
};


#endif


