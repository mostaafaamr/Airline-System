#include "../../include/Flight/Crew.hpp"


Crew::Crew(const std::string& id, const std::string& name, const std::string& role, double totalFlightHours):
crewId(id), name(name), role(role), totalFlightHours(totalFlightHours) {}

bool Crew::assignFlight(const std::string &flightNumber, double flightDuration)
{
    if (totalFlightHours + flightDuration > 100) // 100-hour limit
    {
        std::cout << "Cannot assign flight: " << name << " has exceeded maximum flight hours.\n";
        return false;
    }
    assignedFlights.push_back(flightNumber);
    totalFlightHours += flightDuration;
    return true;
}


