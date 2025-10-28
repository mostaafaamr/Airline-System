#include "../../include/Flight/Flight.hpp"

// Constructor
Flight::Flight(std::string FlightNum, std::string origin, std::string destination,
               std::string departureDate, std::string arrivalDate, std::string aircraftType,
               std::string status, int totalSeats, int availableSeats, double price)
    : FlightNumber(FlightNum), Origin(origin), Destination(destination),
      DepartureDate(departureDate), ArrivalDate(arrivalDate),
      AircraftType(aircraftType), Status(status), TotalSeats(totalSeats),
      availableSeats(availableSeats), price(price) 
      {
        duration = calculateFlightDuration();
      }

std::vector<std::string> Flight::getFlightAttendantNames() const
{
    std::vector<std::string> attendants;
    for (const auto &attendant : flightAttendants)
    {
        attendants.push_back(attendant->getName());
    }
    return attendants;
}

std::vector<std::string> Flight::getFlightAttendantIds() const
{
    std::vector<std::string> attendants;
    for (const auto &attendant : flightAttendants)
    {
        attendants.push_back(attendant->getCrewId());
    }
    return attendants;
}

std::string Flight::getDepartureDate() const
{ 
    // Find the position of ' ' in the DepartureDate string
    size_t tPos = DepartureDate.find(' ');

    // If ' ' is found, extract the substring before it (the date)
    if (tPos != std::string::npos)
    {
        return DepartureDate.substr(0, tPos);
    }

    // If ' ' is not found, return the entire string (assume it's already a date)
    return DepartureDate;
}


int Flight::calculateFlightDuration()
{
    std::tm depTm = {}, arrTm = {};
    std::istringstream depStream(DepartureDate), arrStream(ArrivalDate);

    // Parse the timestamps
    depStream >> std::get_time(&depTm, "%Y-%m-%d %H:%M:%S");
    arrStream >> std::get_time(&arrTm, "%Y-%m-%d %H:%M:%S");

    if (depStream.fail() || arrStream.fail())
    {
        std::cerr << "Failed to parse timestamps!" << std::endl;
        return -1; // Error case
    }

    // Convert to time_t (epoch time)
    std::time_t depTime = std::mktime(&depTm);
    std::time_t arrTime = std::mktime(&arrTm);

    if (depTime == -1 || arrTime == -1)
    {
        std::cerr << "Error converting time!" << std::endl;
        return -1;
    }

    // Calculate duration in hours
    int duration = static_cast<int>(std::difftime(arrTime, depTime) / 3600);
    return duration;
}

void Flight::assignPilot(const std::shared_ptr<Crew>& pilotMember)
{
    if (pilotMember->getRole() != "Pilot")
    {
        std::cout << "Cannot assign non-pilot as pilot." << std::endl;
    }
    if (pilot)
    {
        std::cout << "A pilot is already assigned to this flight." << std::endl;
    }
    pilot = pilotMember;
}

void Flight::addFlightAttendant(const std::shared_ptr<Crew>& attendant)
{
    if (attendant->getRole() != "Flight Attendant")
    {
        std::cout << "Cannot assign non-flight attendant as flight attendant."<<std::endl;
    }
    flightAttendants.push_back(attendant);
}


// Convert Flight object to JSON
nlohmann::json Flight::toJson() const
{
    nlohmann::json flightJson = {
        {"flightNumber", FlightNumber},
        {"origin", Origin},
        {"destination", Destination},
        {"departure", DepartureDate},
        {"arrival", ArrivalDate},
        {"aircraftModel", AircraftType},
        {"status", Status},
        {"totalSeats", TotalSeats},
        {"availableSeats", availableSeats},
        {"price", price}
    };

    // Include pilot only if assigned
    if (pilot)
    {
        flightJson["pilot"] = {
            {"id", pilot->getCrewId()},
            {"name", pilot->getName()},
            {"role", pilot->getRole()}
        };
    }

    // Include flight attendants only if assigned
    if (!flightAttendants.empty())
    {
        flightJson["flightAttendants"] = nlohmann::json::array();
        for (const auto &attendant : flightAttendants)
        {
            flightJson["flightAttendants"].push_back({
                {"id", attendant->getCrewId()},
                {"name", attendant->getName()},
                {"role", attendant->getRole()}
            });
        }
    }

    return flightJson;
}

// Convert JSON to Flight object
Flight Flight::fromJson(const nlohmann::json& j)
{
    Flight flight(
        j.at("flightNumber").get<std::string>(),
        j.at("origin").get<std::string>(),
        j.at("destination").get<std::string>(),
        j.at("departure").get<std::string>(),
        j.at("arrival").get<std::string>(),
        j.at("aircraftModel").get<std::string>(),
        j.at("status").get<std::string>(),
        j.at("totalSeats").get<int>(),
        j.at("availableSeats").get<int>(),
        j.at("price").get<double>()
    );

    // Assign pilot if present in JSON
    if (j.contains("pilot"))
    {
        auto pilotJson = j["pilot"];
        auto pilot = std::make_shared<Crew>(
            pilotJson["id"].get<std::string>(),
            pilotJson["name"].get<std::string>(),
            pilotJson["role"].get<std::string>()
        );
        flight.assignPilot(pilot);
    }

    // Assign flight attendants if present in JSON
    if (j.contains("flightAttendants"))
    {
        for (const auto &attendantJson : j["flightAttendants"])
        {
            auto attendant = std::make_shared<Crew>(
                attendantJson["id"].get<std::string>(),
                attendantJson["name"].get<std::string>(),
                attendantJson["role"].get<std::string>()
            );
            flight.addFlightAttendant(attendant);
        }
    }

    return flight;
}
