#include "../../include/Booking/ReservationServiceAdmin.hpp"

std::pair<int, double> ReservationServiceAdmin::countReservationsAndRevenue(const std::string& flightNumber) const
{
    int count = 0;
    double revenue = 0.0;

    auto reservations = getReservations();

    for(const auto& reservation:reservations)
    {
        if(reservation.getFlightNumber() == flightNumber)
        {
            count++;
            revenue += reservation.getPrice();
        }
    }
    return {count, revenue};
}