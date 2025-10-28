#include "../../include/Flight/FlightService.hpp"

std::vector<Flight> FlightService::getFlights() const 
{
    return loadFlightsFromJson("data/flights.json");
}


void FlightService::displayFlights() const
{
    std::vector<Flight> flights = getFlights();

    if (flights.empty())
    {
        std::cout << "No flights available." << std::endl;
        return;
    }

    for (const auto &flight : flights)
    {
        std::cout << "Flight Number: " << flight.getFlightNumber() << std::endl;
        std::cout << "Departure: "  << flight.getDepartureDateAndTime() << std::endl;
        std::cout << "Arrival: " << flight.getArrivalDate() << std::endl;
        std::cout << "Aircraft: " << flight.getAircraftType() << std::endl;
        std::cout << "Available Seats: " << flight.getAvailableSeats() << std::endl;
        std::cout << "Price: $" << std::fixed << std::setprecision(2) << flight.getPrice() << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
}


void FlightService::searchFlights(const std::string &origin, const std::string &destination, const std::string &departureDate) const
{
    Utils::clearScreen();
    std::vector<Flight> flights = getFlights();

    if (flights.empty())
    {
        std::cout << "No flights available" << std::endl;
        return;
    }

    bool flightsFound = false;
    int count = 0;

    std::cout << "Available Flights:" << std::endl;

    for (const auto &flight : flights)
    {
        if (flight.getOrigin() == origin && flight.getDestination() == destination && flight.getDepartureDate() == departureDate)
        {
            std::cout << ++count << ". " << "Flight Number: " << flight.getFlightNumber() << std::endl;
            std::cout << "\tDeparture: " << flight.getDepartureDate() << " " << flight.getDepartureDateAndTime() << std::endl;
            std::cout << "\tArrival: " << flight.getArrivalDate() << " " << flight.getArrivalDate() << std::endl;
            std::cout << "\tAircraft: " << flight.getAircraftType() << std::endl;
            std::cout << "\tAvailable Seats: " << flight.getAvailableSeats() << std::endl;
            std::cout << "\tPrice: $" << std::fixed << std::setprecision(2) << flight.getPrice() << std::endl;
            std::cout << "-------------------------" << std::endl;
            flightsFound = true;
        }
    }
    if(flightsFound == false)
    {
        std::cout << "No flights match the origin and destination given\n " << std::endl;
        std::cout << "The available flights are:\n " << std::endl;

        //Display all flights instead
        for (const auto &flight : flights)
        {
            std::cout << "Flight Number: " << flight.getFlightNumber() << std::endl;
            std::cout << "Departure: " << flight.getDepartureDate() << " " << flight.getDepartureDateAndTime() << std::endl;
            std::cout << "Arrival: " << flight.getArrivalDate() << " " << flight.getArrivalDate() << std::endl;
            std::cout << "Aircraft: " << flight.getAircraftType() << std::endl;
            std::cout << "Available Seats: " << flight.getAvailableSeats() << std::endl;
            std::cout << "Price: $" << std::fixed << std::setprecision(2) << flight.getPrice() << std::endl;
            std::cout << "-------------------------" << std::endl;
        }
    }
}


void FlightService::displayAvailableSeats(const std::string& flightNumber) const
{
    // Read seat data from JSON
    nlohmann::json flights;
    std::ifstream file("data/seats.json");
    if (file.good())
    {
        file >> flights;
        file.close();
    }

    if (!flights.contains(flightNumber))
    {
        std::cout << "Flight not found.\n";
        return;
    }

    std::cout << "--- Seat Map for Flight " << flightNumber << " (AVL: Available, BKD: booked) ---\n";
    int rows = flights[flightNumber]["rows"];
    int cols = flights[flightNumber]["cols"];
    std::map<int, char> colMap = {{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}};

    for (int row = 1; row <= rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::string seat = std::to_string(row) + colMap[col];
            std::cout << seat << (flights[flightNumber]["seats"][seat] == "available" ? " [AVL] " : " [BKD] ");
        }
        std::cout << "\n";
    }
}


bool FlightService::markSeatAsBooked(const std::string& flightNumber, const std::string& seatNumber)
{
    // Read the seats data from seats.json
    nlohmann::json seatsData;
    try {
        seatsData = JsonUtils::readJsonFromFile("data/seats.json");
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    // Check if the flight exists in seats.json
    if (!seatsData.contains(flightNumber) || !seatsData[flightNumber]["seats"].contains(seatNumber))
    {
        std::cout << "Seat " << seatNumber << " is invalid. Please enter a valid seat number" << std::endl;
        return false; // Seat does not exist
    }

    // Check if the seat is available
    if (seatsData[flightNumber]["seats"][seatNumber] == "available")
    {
        // Mark the seat as booked
        seatsData[flightNumber]["seats"][seatNumber] = "booked";

        // Now update the available seats in flights.json
        nlohmann::json flightsData;
        try {
            flightsData = JsonUtils::readJsonFromFile("data/flights.json");
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return false;
        }

        // Find the flight in flights.json
        auto flightIt = std::find_if(flightsData.begin(), flightsData.end(), [&](const nlohmann::json &flight)
                                      { return flight["flightNumber"] == flightNumber; });

        if (flightIt != flightsData.end())
        {
            // Decrement available seats
            int availableSeats = (*flightIt)["availableSeats"];
            if (availableSeats > 0)
            {
                (*flightIt)["availableSeats"] = availableSeats - 1;
            }
            else
            {
                std::cout << "No available seats left." << std::endl;
                return false; // No available seats left
            }

            // Write the modified flights.json back to the file
            try {
                JsonUtils::saveJsonToFile(flightsData, "data/flights.json");
            } catch (const std::runtime_error &e) {
                std::cerr << e.what() << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "Flight " << flightNumber << " not found in flights.json." << std::endl;
            return false; // Flight not found
        }

        // Write the modified seats.json back to the file
        try {
            JsonUtils::saveJsonToFile(seatsData, "data/seats.json");
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return false;
        }

        std::cout << "Seat " << seatNumber << " on flight " << flightNumber << " has been booked." << std::endl;
        return true;
    }
    else if (seatsData[flightNumber]["seats"][seatNumber] == "booked")
    {
        std::cout << "Seat is already booked" << std::endl;
        return false; // Seat is already booked
    }

    return false; // Default return in case of unexpected scenarios
}

bool FlightService::changeSeat(const std::string &flightNumber, const std::string &oldSeatNumber, const std::string &newSeatNumber)
{
    nlohmann::json flights;
    std::ifstream file("data/seats.json");
    if (file.good())
    {
        file >> flights;
        file.close();
    }

    if (flights.contains(flightNumber))
    {
        if (flights[flightNumber]["seats"][newSeatNumber] == "available")
        {
            flights[flightNumber]["seats"][newSeatNumber] = "booked";
            flights[flightNumber]["seats"][oldSeatNumber] = "available";
            std::ofstream outFile("data/seats.json");
            outFile << flights.dump(4);
            outFile.close();
            return true;
        }
        else if (flights[flightNumber]["seats"][newSeatNumber] == "booked")
        {
            return false;
        }
    }

    std::cout << "Flight doesn't exist! " << std::endl;
    return false;
}


std::optional<std::reference_wrapper<Flight>> FlightService::findFlight(const std::string& flightNumber) const
{
    auto flights = getFlights();
    for(auto& flight:flights)
    {   
        if(flight.getFlightNumber() == flightNumber)    // Found Flight
        {
            return flight;
        }
    }
    
    std::cout<<"Flight Not Found"<<std::endl;
    return std::nullopt;
    
}

std::vector<Flight> FlightService::loadFlightsFromJson(const std::string& filename) const 
{
    std::vector<Flight> flights;
    nlohmann::json flightsJson = JsonUtils::readJsonFromFile(filename);

    for (const auto &flightJson : flightsJson)
    {
        flights.push_back(Flight::fromJson(flightJson));
    }

    return flights;
}   
