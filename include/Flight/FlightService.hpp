#ifndef FLIGHTSERVICE_HPP
#define FLIGHTSERVICE_HPP

#include <vector>
#include <optional>
#include <sstream>
#include "Flight.hpp"
#include "../Utils/JsonUtils.hpp"
#include "../Utils/Utils.hpp"

class FlightService
{
public:
   
    // Display all flights
    void displayFlights() const;

    //Search for specific flights
    void searchFlights(const std::string &origin, const std::string &destination, const std::string &departureDate) const;

    // Display seats in a flight
    void displayAvailableSeats(const std::string& flightNumber) const;

    // Marking seat as booked after reservation
    bool markSeatAsBooked(const std::string& flightNumber, const std::string& seatNumber);

    // Change seats for a passenger
    bool changeSeat(const std::string &flightNumber, const std::string &oldSeatNumber, const std::string &newSeatNumber);
    
    // Find a specific flight
    std::optional<std::reference_wrapper<Flight>> findFlight(const std::string& flightNumber) const;

protected:
    std::vector<Flight> loadFlightsFromJson(const std::string &filename) const;
    
    // Get flights
    std::vector<Flight> getFlights() const;
};

#endif