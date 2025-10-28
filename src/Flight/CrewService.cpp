#include "../../include/Flight/CrewService.hpp"

CrewService::CrewService(const std::string &crewFilePath) : crewFilePath(crewFilePath)
{
    loadCrewData();
}

void CrewService::loadCrewData()
{
    crewData = JsonUtils::readJsonFromFile(crewFilePath);
}

void CrewService::saveCrewData()
{
    JsonUtils::saveJsonToFile(crewData, crewFilePath);
}

void CrewService::assignCrewToFlight(const std::string &flightNumber, Flight &flight)
{
    auto flightDuration = flight.getDuration();
    assignPilot(flightNumber, flight, flightDuration);
    assignFlightAttendants(flightNumber, flight, flightDuration);
    saveCrewData();
}

void CrewService::assignPilot(const std::string &flightNumber, Flight &flight, double flightDuration)
{
    // Check if a pilot is already assigned to the flight
    if (isPilotAssigned(flightNumber))
    {
        std::cout << "A pilot is already assigned to flight " << flightNumber << "." << std::endl;
        return;
    }

    std::cout << "--- Crew Assignments ---\nAvailable Pilots:" << std::endl;
    for (const auto &pilot : crewData["pilots"])
    {
        std::cout << "Pilot ID: " << pilot["id"] << " - " << pilot["name"]
                  << " (" << pilot["totalFlightHours"] << " hours flown)" << std::endl;
    }

    std::cout << "\nSelect Pilot by ID: ";
    std::string selectedPilotId;
    std::getline(std::cin, selectedPilotId);

    auto pilotIt = std::find_if(crewData["pilots"].begin(), crewData["pilots"].end(),
                                [&selectedPilotId](const nlohmann::json &p)
                                {
                                    return p["id"] == selectedPilotId;
                                });

    if (pilotIt == crewData["pilots"].end())
    {
        std::cout << "Invalid Pilot ID" << std::endl;
        return;
    }

    auto selectedPilot = std::make_shared<Crew>(
        (*pilotIt)["id"], (*pilotIt)["name"], "Pilot", (*pilotIt)["totalFlightHours"]);

    if (!selectedPilot->assignFlight(flightNumber, flightDuration))
    {
        return; // Pilot exceeds working hours
    }

    flight.assignPilot(selectedPilot);

    // Update pilot's data in crewData
    (*pilotIt)["totalFlightHours"] = selectedPilot->getTotalFlightHours();
    (*pilotIt)["assignedFlights"].push_back(flightNumber);
}

bool CrewService::isPilotAssigned(const std::string &flightNumber) const
{
    for (const auto &pilot : crewData["pilots"])
    {
        auto assignedFlights = pilot["assignedFlights"];
        if (std::find(assignedFlights.begin(), assignedFlights.end(), flightNumber) != assignedFlights.end())
        {
            return true; // Pilot is already assigned to the flight
        }
    }
    return false; // No pilot is assigned to the flight
}

void CrewService::assignFlightAttendants(const std::string &flightNumber, Flight &flight, double flightDuration)
{
    std::vector<std::shared_ptr<Crew>> selectedAttendants;
    std::cout << "\nAvailable Flight Attendants:" << std::endl;
    for (const auto &fa : crewData["flight_attendants"])
    {
        std::cout << fa["id"] << " - " << fa["name"]
                  << " (" << fa["totalFlightHours"] << " hours flown)" << std::endl;
    }

    std::cout << "\nSelect Flight Attendants by IDs (comma-separated, e.g., FA101,FA102): ";
    std::string selectedFaIds;
    std::getline(std::cin, selectedFaIds);

    std::istringstream faStream(selectedFaIds);
    std::string faId;
    while (std::getline(faStream, faId, ','))
    {
        auto faIt = std::find_if(crewData["flight_attendants"].begin(), crewData["flight_attendants"].end(),
                                 [&faId](const nlohmann::json &fa)
                                 {
                                     return fa["id"] == faId;
                                 });

        if (faIt != crewData["flight_attendants"].end())
        {
            auto attendant = std::make_shared<Crew>(
                (*faIt)["id"], (*faIt)["name"], "Flight Attendant");

            if (attendant->assignFlight(flightNumber, flightDuration))
            {
                selectedAttendants.push_back(attendant);
                (*faIt)["totalFlightHours"] = attendant->getTotalFlightHours();
                (*faIt)["assignedFlights"].push_back(flightNumber);
            }
        }
        else
        {
            std::cout << "Invalid Flight Attendant ID: " << faId << std::endl;
        }
    }

    // Add selected attendants to the flight
    for (const auto &attendant : selectedAttendants)
    {
        flight.addFlightAttendant(attendant);
    }
}


std::vector<std::shared_ptr<Crew>> CrewService::getCrewForFlight(const std::string &flightNumber) const
{
    std::vector<std::shared_ptr<Crew>> crewMembers;

    // Search for pilot
    for (const auto &pilot : crewData["pilots"])
    {
        auto assignedFlights = pilot["assignedFlights"];
        if (std::find(assignedFlights.begin(), assignedFlights.end(), flightNumber) != assignedFlights.end())
        {
            crewMembers.push_back(std::make_shared<Crew>(
                pilot["id"], pilot["name"], "Pilot"));
        }
    }

    // Search for flight attendants
    for (const auto &fa : crewData["flight_attendants"])
    {
        auto assignedFlights = fa["assignedFlights"];
        if (std::find(assignedFlights.begin(), assignedFlights.end(), flightNumber) != assignedFlights.end())
        {
            crewMembers.push_back(std::make_shared<Crew>(
                fa["id"], fa["name"], "Flight Attendant"));
        }
    }

    return crewMembers;
}