#ifndef FLIGHTSERVICEADMIN_HPP
#define FLIGHTSERVICEADMIN_HPP

#include "FlightService.hpp"

class FlightServiceAdmin : public FlightService
{

public:
    // Get flights
    std::vector<Flight> getFlightsForReport() const;
};




#endif