#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include <fstream>
#include "User.hpp"
#include "../Flight/FlightService.hpp"
#include "../Booking/ReservationService.hpp"
#include "../CheckIn/BoardingPass.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/Utils.hpp"

class Passenger: public User
{
private:
    std::string contactDetails;
    std::vector<Reservation> travelHistory;
    int loyaltyPoints;

    // Static flight service member to handle viewing flights
    static inline FlightService flightService{};

    // Static reservation service member to handle viewing reservations
    static inline ReservationService reservationService{};

    //Static report generator member to handle report generation
    static inline ActivityLogger activityLogger{};

    // Helper methods for menu functionality
    void searchFlightsMenu();

public:
    Passenger(const std::string& id, const std::string& username, const std::string& password);

    void searchFlights(const std::string &origin, const std::string &destination, const std::string &departureDate) const;
    void viewReservations() const;

    // Add reservation to travel history
    bool bookFlight(Reservation& reservation, const std::string& paymentMethod, const std::optional<std::string>& paymentDetails = std::nullopt);

    // Display the seat map of a flight
    void displayAvailableSeats(const std::string& flightNumber) const
    {flightService.displayAvailableSeats(flightNumber);}

    // Find Reservation
    std::optional<std::reference_wrapper<Reservation>> findReservation(const std::string &reservationId);
    // Find Flight
    std::optional<std::reference_wrapper<Flight>> findFlight(const std::string &flightNumber);


    // Redeem loyalty points for a discount
    void redeemPoints(int points);

    // Check-In method
    void checkIn() const;

    // Main Menu
    void passengerMenu();

    ~Passenger()= default;
};


#endif