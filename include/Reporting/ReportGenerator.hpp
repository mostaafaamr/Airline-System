#ifndef REPORTGENERATOR_HPP
#define REPORTGENERATOR_HPP

#include <string>
#include <vector>
#include <optional>
#include "../Flight/Flight.hpp"
#include "../Flight/Aircraft.hpp"
#include "../Booking/Reservation.hpp"
#include "../Flight/FlightServiceAdmin.hpp"
#include "../Booking/ReservationServiceAdmin.hpp"
#include "../Utils/Logger.hpp"

class ReportGenerator
{
private:
    //Static flight service member to handle viewing flights
    static inline FlightServiceAdmin flightService{};

    //Static reservation service member to handle viewing reservations
    static inline ReservationServiceAdmin reservationService{};

    std::string logFilePath;

public:
    ReportGenerator() = default;
    ~ReportGenerator()= default;

    // Generate a flight performance report for an aircraft
    void generateFlightPerformanceReport(const std::string& month, const std::string& year) const;
    // Generate a maintenance report for an aircraft
    void generateMaintenanceReport(const Aircraft& aircraft) const;
    // Generate user activity report
    void generateUserActivityReport(const std::optional<std::string>& userId = std::nullopt) const;

};





#endif