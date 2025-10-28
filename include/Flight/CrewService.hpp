#ifndef CREWSERVICE_HPP
#define CREWSERVICE_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "Flight.hpp"
#include "../Utils/JsonUtils.hpp"
#include "Crew.hpp"

class CrewService
{
public:
    CrewService(const std::string &crewFilePath);
    void assignCrewToFlight(const std::string &flightNumber, Flight &flight);
    std::vector<std::shared_ptr<Crew>> getCrewForFlight(const std::string &flightNumber) const;

private:
    nlohmann::json crewData;
    std::string crewFilePath;

    void loadCrewData();
    void saveCrewData();
    void assignPilot(const std::string &flightNumber, Flight &flight, double flightDuration);
    bool isPilotAssigned(const std::string &flightNumber) const;
    void assignFlightAttendants(const std::string &flightNumber, Flight &flight, double flightDuration);
};

#endif