#include "../../include/CheckIn/BoardingPass.hpp"
#include <iostream>

// Constructor
BoardingPass::BoardingPass(const std::string &reservationId, const std::string &passengerName, const std::string &flightNumber,
                           const std::string &origin, const std::string &destination, const std::string &date,
                           const std::string &seatNumber, const std::string &gate, const std::string &boardingTime)
    : reservationId(reservationId), passengerName(passengerName), flightNumber(flightNumber), origin(origin),
      destination(destination), date(date), seatNumber(seatNumber), gate(gate), boardingTime(boardingTime) {}

// Display boarding pass
void BoardingPass::display() const
{
    std::cout << "Boarding Pass:"<<std::endl;
    std::cout << "-----------------------------"<<std::endl;
    std::cout << "Passenger: " << passengerName <<std::endl;
    std::cout << "Flight: " << flightNumber <<std::endl;
    std::cout << "Origin: " << origin <<std::endl;
    std::cout << "Destination: " << destination <<std::endl;
    std::cout << "Departure: " << date <<std::endl;
    std::cout << "Seat: " << seatNumber <<std::endl;
    std::cout << "Gate: " << gate <<std::endl;
    std::cout << "Boarding Time: " << boardingTime <<std::endl;
    std::cout << "-----------------------------"<<std::endl;
}