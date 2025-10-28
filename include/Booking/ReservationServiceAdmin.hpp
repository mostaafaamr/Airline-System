#ifndef RESERVATIONSERVICEADMIN_HPP
#define RESERVATIONSERVICEADMIN_HPP

#include "ReservationService.hpp"

class ReservationServiceAdmin : public ReservationService
{

public:
    // Get the revenue from a flight for reports
    std::pair<int, double> countReservationsAndRevenue(const std::string &flightNumber) const;

};

#endif