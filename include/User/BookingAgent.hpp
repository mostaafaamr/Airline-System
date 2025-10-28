#ifndef BOOKINGAGENT_HPP
#define BOOKINGAGENT_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include "User.hpp"
#include "../Flight/FlightService.hpp"
#include "../Booking/Reservation.hpp"
#include "../Booking/ReservationService.hpp"
#include "../CheckIn/BoardingPass.hpp"
#include "../Booking/PaymentService.hpp"
#include "../Utils/JsonUtils.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/Utils.hpp"

class BookingAgent: public User
{
private:
    std::vector<Reservation> reservations;
    std::vector<BoardingPass> scannedPasses;

    //Static flight service member to handle viewing flights
    static inline FlightService flightService{};

    // Static reservation service member to handle viewing reservations
    static inline ReservationService reservationService{};

    //Static flight service member to handle payment services
    static inline PaymentService paymentService{};

    //Static report generator member to handle report generation
    static inline ActivityLogger activityLogger{};

    bool isValidBoardingPass(const BoardingPass& boardingPass) const;

    // Helper methods for JSON file handling
    void loadReservationsFromJson(const std::string &filename);
    void saveReservationsToJson(const std::string &filename);

    // Helper methods for menu functionality
    void searchFlightsMenu();
    void bookFlightMenu();
    void modifyReservationMenu();
    void cancelReservationMenu();

public:
    BookingAgent(const std::string &id, const std::string &username, const std::string &password);

    // Search for a specific flight
    void searchFlights(const std::string &origin, const std::string &destination, const std::string &departureDate) const;
    
    // Reservation management
    bool bookFlight(Reservation &reservation, const std::string& paymentMethod, const std::optional<std::string>& paymentDetails = std::nullopt);
    void updateReservation(const std::string &reservationId, const Reservation &updatedReservation);
    void cancelReservation(const std::string &reservationId);
    const std::vector<Reservation>& getReservations() const;

    bool changeSeat(const std::string &flightNumber, const std::string &oldSeatNumber, const std::string &newSeatNumber);
    // Find a reservation by ID
    std::optional<std::reference_wrapper<Reservation>> findReservation(const std::string &reservationId);

    void displayAvailableSeats(const std::string& flightNumber) const
    { flightService.displayAvailableSeats(flightNumber); }

    void scanBoardingPass(const BoardingPass& boardingPass);
    
    // Viewing flights
    void viewFlights() const;
    std::optional<std::reference_wrapper<Flight>> findFlight(const std::string &flightNumber);

    // Main Menu
    void bookingAgentMenu();
    
};

#endif