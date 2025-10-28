#include "../../include/Flight/FlightServiceAdmin.hpp"

std::vector<Flight> FlightServiceAdmin::getFlightsForReport() const
{
    auto flights = getFlights();
    return flights;
}