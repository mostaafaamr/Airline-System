#ifndef BOARDINGPASS_HPP
#define BOARDINGPASS_HPP

#include <string>
#include <iostream>

class BoardingPass
{
private:
    std::string reservationId;
    std::string passengerName;
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string date;
    std::string seatNumber;
    std::string gate;
    std::string boardingTime;
 
public:
    BoardingPass() = default;
    BoardingPass(const std::string &reservationId, const std::string &passengerName, const std::string &flightNumber,
                 const std::string &origin, const std::string &destination, const std::string &date,
                 const std::string &seatNumber, const std::string &gate, const std::string &boardingTime);

    // Getters
    std::string getReservationId() const { return reservationId; }
    std::string getPassengerName() const { return passengerName; }
    std::string getFlightNumber() const { return flightNumber; }
    std::string getSeatNumber() const { return seatNumber; }
    std::string getGate() const { return gate; }
    std::string getBoardingTime() const { return boardingTime; }

    // Display boarding pass
    void display() const;
};

#endif