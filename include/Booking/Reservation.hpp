#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <string>
#include <optional>
#include "PaymentService.hpp"
#include <nlohmann/json.hpp>

class Reservation
{
private:
    std::string reservationId;
    std::string passengerId;
    std::string passengerName;
    std::string flightNumber;
    std::string seatNumber;
    std::string gate;
    std::string boardingTime;
    std::string status;    
    double price;
    std::string paymentMethod;
    std::optional<std::string> paymentDetails = std::nullopt;
    std::string paymentStatus = "Unpaid";

public:
    Reservation() = default;
    Reservation(const std::string &reservationId, const std::string &passengerId,
                const std::string &passengerName,const std::string &flightNumber, const std::string &seatNumber,
                const std::string &gate,const std::string &boardingTime,
                const std::string &status, double price);

    // Getters
    std::string getReservationId() const { return reservationId; }
    std::string getPassengerId() const { return passengerId; }
    std::string getPassengerName() const { return passengerName; }
    std::string getFlightNumber() const { return flightNumber; }
    std::string getSeatNumber() const { return seatNumber; }
    std::string getStatus() const { return status; }
    std::string getGate() const { return gate; }
    std::string getBoardingTime() const { return boardingTime; }
    double getPrice() const { return price; }
    std::string getPaymentMethod() const { return paymentMethod; }
    std::optional<std::string> getPaymentDetails() const { return paymentDetails; }
    std::string getPaymentStatus() const { return paymentStatus; }
    
    // Setters
    void setStatus(const std::string& newStatus) { status = newStatus; }
    void setSeatNumber(const std::string& newSeat) { seatNumber = newSeat; }
    void setPassengerName(const std::string& name) { passengerName = name; }

    // Set payment details
    void setPaymentDetails(const std::string& method, const std::optional<std::string>& details = std::nullopt)
    { paymentMethod = method;  paymentDetails = details; }

    // Set payment status
    void setPaymentStatus(const std::string& status) { paymentStatus = status; }

    //Serialization
    static Reservation fromJson(const nlohmann::json &j);
    nlohmann::json toJson() const;
};

#endif