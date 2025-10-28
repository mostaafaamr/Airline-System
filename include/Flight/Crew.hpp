#ifndef CREW_HPP
#define CREW_HPP

#include <iostream>
#include <string>
#include <vector>

class Crew
{
private:
    std::string crewId;
    std::string name;
    std::string role;
    double totalFlightHours;
    std::vector<std::string> assignedFlights;

public:
    Crew(const std::string& id, const std::string& name, const std::string& role, double totalFlightHours = 0);
    ~Crew()= default;


    // Getters
    std::string getCrewId() const { return crewId; }
    std::string getName() const { return name; }
    std::string getRole() const { return role; }
    double getTotalFlightHours() const { return totalFlightHours; }
    const std::vector<std::string>& getAssignedFlights() const { return assignedFlights; }

    // Assign a flight
    bool assignFlight(const std::string& flightNumber, double flightDuration);

};





#endif