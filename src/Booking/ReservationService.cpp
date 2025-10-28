#include "../../include/Booking/ReservationService.hpp"
 

ReservationService::ReservationService()
{
    reservations = getReservations();
}
void ReservationService::displayReservations(const std::string &passengerId)
{ 
    bool found = false;
    reservations = getReservations();

    for (const auto &reservation : reservations)
    {
        if (reservation.getPassengerId() == passengerId)
        {   
            // Find the flight
            auto flightOpt = flightService.findFlight(reservation.getFlightNumber());

            // Check if the flight was found
            if (!flightOpt)
            {
                std::cout << "Reservation ID: " << reservation.getReservationId() << std::endl;
                std::cout << "Flight: " << reservation.getFlightNumber() << " (Flight details not found)" << std::endl;
                std::cout << "Seat: " << reservation.getSeatNumber() << std::endl;
                std::cout << "Status: " << reservation.getStatus() << std::endl;
                std::cout << "-------------------------" << std::endl;
                found = true;
                continue; // Skip to the next reservation
            }

            // Flight was found, display details
            Flight &flight = flightOpt.value().get();
            std::cout << "Reservation ID: " << reservation.getReservationId() << std::endl;
            std::cout << "Flight: " << reservation.getFlightNumber() << " from " << flight.getOrigin();
            std::cout << " to " << flight.getDestination() << std::endl;
            std::cout << "Departure: " << flight.getDepartureDateAndTime() << std::endl;
            std::cout << "Seat: " << reservation.getSeatNumber() << std::endl;
            std::cout << "Status: " << reservation.getStatus() << std::endl;
            std::cout << "-------------------------" << std::endl;
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No reservations found for passenger ID " << passengerId << "." << std::endl;
    }
}


void ReservationService::displayReservation(const Reservation &reservation)
{ 
    std::optional<std::reference_wrapper<Flight>> flightOpt;
    bool found = false;
    reservations = getReservations();
    for (const auto &reservationSearch : reservations)
    {
        if (reservationSearch.getReservationId() == reservation.getReservationId())
        {   
            flightOpt = flightService.findFlight(reservation.getFlightNumber());
            Flight &flight = flightOpt.value().get();
            std::cout << "Reservation ID: " << reservation.getReservationId() << std::endl;
            std::cout << "Flight: " << reservation.getFlightNumber() << " from "<<flight.getOrigin();
            std::cout << " to " << flight.getDestination() << std::endl;
            std::cout << "Departure: " << flight.getDepartureDateAndTime() << std::endl;
            std::cout << "Seat: " << reservation.getSeatNumber() << std::endl;
            std::cout << "Status: " << reservation.getStatus() << std::endl;
            std::cout << "-------------------------" << std::endl;
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "Reservation not found " << std::endl;
    }
}

bool ReservationService::bookFlight(Reservation &reservation, const std::string& paymentMethod, const std::optional<std::string>& paymentDetails)
{
    // Check for duplicates
    for (const auto &existingReservation : reservations)
    {
        if (existingReservation.getReservationId() == reservation.getReservationId())
        {
            std::cout << "A reservation with the same ID already exists." << std::endl;
            return false;
        }
    }
    // Process payment
    if (paymentService.processPayment(paymentMethod, reservation.getPrice(), paymentDetails))
    {
        if (flightService.markSeatAsBooked(reservation.getFlightNumber(), reservation.getSeatNumber()))
        {
            std::cout << "Processing payment of $" << reservation.getPrice() << " via " << paymentMethod << "..." << std::endl;
            std::cout << "Payment successful!" << std::endl;
            reservation.setPaymentStatus("Paid");
            reservation.setPaymentDetails(paymentMethod, paymentDetails);
            std::cout << "Booking successful!\nReservation ID: " << reservation.getReservationId() << std::endl;
            reservations.push_back(reservation);
            saveReservationsToJson("data/reservations.json");
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            return true;
        }
        else
        {
            std::cout << "Booking failed." << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "Booking failed. Payment could not be processed." << std::endl;
        return false;
    }
}

std::optional<std::reference_wrapper<Reservation>> ReservationService::findReservation(const std::string& reservationId)
{

    for (auto &reservation : reservations)
    {
        if (reservation.getReservationId() == reservationId)
        {
            return std::ref(reservation);
        }
    }

    // Return an empty reservation if not found
    return std::nullopt; 
}


void ReservationService::updateReservationStatus(const std::string& reservationId, const std::string& status)
{
        for (auto& reservation : reservations)
        {
            if (reservation.getReservationId() == reservationId)
            {
                reservation.setStatus(status);
                saveReservationsToJson("data/reservations.json");
                return;
            }
        }

        std::cout << "Reservation not found." << std::endl;
}


std::vector<Reservation> ReservationService::getReservations() const
{
    return loadReservationsFromJson("data/reservations.json");
}



std::vector<Reservation> ReservationService::loadReservationsFromJson(const std::string &filename) const
{
    std::vector<Reservation> reservations;
    nlohmann::json reservationsJson = JsonUtils::readJsonFromFile(filename);

    // Ensure the JSON data is an array
    if (!reservationsJson.is_array())
    {
        throw std::runtime_error("Invalid JSON format: Expected an array of reservations.");
    }

    for (const auto &reservationJson : reservationsJson)
    {
        reservations.push_back(Reservation::fromJson(reservationJson));
    }

    return reservations;
}

void ReservationService::saveReservationsToJson(const std::string& filename) const 
{
    nlohmann::json reservationsJson;

    for (const auto &reservation : reservations)
    {
        reservationsJson.push_back(reservation.toJson());
    }

    JsonUtils::saveJsonToFile(reservationsJson, filename);
}
