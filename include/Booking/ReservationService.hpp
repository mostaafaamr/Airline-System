#ifndef RESERVATIONSERVICE_HPP
#define RESERVATIONSERVICE_HPP

#include "Reservation.hpp"
#include <iostream>
#include "../Utils/JsonUtils.hpp"
#include "../Flight/FlightService.hpp"
#include <vector>
#include <functional>
#include <utility>

class ReservationService
{
protected :
    // Get all reservations
    std::vector<Reservation> getReservations() const;

public:
    
    ReservationService();
    // Display reservations for a specific passenger
    void displayReservations(const std::string &passengerId);

    // Display reservations for a specific passenger
    void displayReservation(const Reservation &reservation);

    // Booking a flight
    bool bookFlight(Reservation &reservation, const std::string& paymentMethod, const std::optional<std::string>& paymentDetails = std::nullopt);

    // Find a reservation by ID
    std::optional<std::reference_wrapper<Reservation>> findReservation(const std::string& reservationId);

    // Update the status of a reservation
    void updateReservationStatus(const std::string& reservationId, const std::string& status);

private:

    // Static flight service member to handle viewing flights
    static inline FlightService flightService{};

    //Static flight service member to handle payment services
    static inline PaymentService paymentService{};

    // Load reservations from JSON file
    std::vector<Reservation> loadReservationsFromJson(const std::string &filename) const;

    // Save reservations to JSON file
    void saveReservationsToJson(const std::string& filename) const;

    std::vector<Reservation> reservations;
};

#endif