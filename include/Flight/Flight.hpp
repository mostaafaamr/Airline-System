#ifndef FLIGHT_HPP
#define FLIGHT_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "Crew.hpp" 

class Flight
{
private:
    std::string FlightNumber;
    std::string Origin;
    std::string Destination;
    std::string DepartureDate;
    std::string ArrivalDate;
    std::string AircraftType;
    std::string Status;
    int duration;
    int TotalSeats;
    int availableSeats;
    double price;
    std::shared_ptr<Crew> pilot; // Only 1 pilot
    std::vector<std::shared_ptr<Crew>> flightAttendants; // Multiple flight attendants

    // Method to calculate flight duration
    int calculateFlightDuration() ;
public:
    Flight()=default;
    Flight(std::string FlightNum, std::string origin, std::string destination,
           std::string departureDate, std::string arrivalDate, std::string aircraftType,
           std::string status, int totalSeats, int availableSeats, double price);
    ~Flight()= default;

    // Getters
    std::string getFlightNumber() const { return FlightNumber; }
    std::string getOrigin() const { return Origin; }
    std::string getDestination() const { return Destination; }
    std::string getDepartureDateAndTime() const { return DepartureDate; }
    std::string getArrivalDate() const { return ArrivalDate; }
    std::string getAircraftType() const { return AircraftType; }
    std::string getStatus() const { return Status; }
    int getTotalSeats() const { return TotalSeats; }
    int getAvailableSeats() const { return availableSeats; }
    double getPrice() const { return price; }
    int getDuration() const { return duration; }

    std::string getPilotName() const { return pilot->getName(); }
    std::string getPilotId() const { return pilot->getCrewId(); }
    std::vector<std::string> getFlightAttendantNames() const;
    std::vector<std::string> getFlightAttendantIds() const;
    std::string getDepartureDate() const;

    // Setters
    void setFlightNumber(const std::string &flightNum) { FlightNumber = flightNum; }
    void setOrigin(const std::string &origin) { Origin = origin; }
    void setDestination(const std::string &destination) { Destination = destination; }
    void setDepartureDate(const std::string &departureDate) { DepartureDate = departureDate; }
    void setArrivalDate(const std::string &arrivalDate) { ArrivalDate = arrivalDate; }
    void setAircraftType(const std::string &aircraftType) { AircraftType = aircraftType; }
    void setStatus(const std::string &status) { Status = status; }
    void setTotalSeats(int totalSeats) { TotalSeats = totalSeats; }
    void setAvailableSeats(int seats) { availableSeats = seats; }
    void setPrice(double newPrice) { price = newPrice; }
    
    
    // Crew Assignment
    void assignPilot(const std::shared_ptr<Crew>& pilotMember);
    void addFlightAttendant(const std::shared_ptr<Crew>& attendant);

    

    // JSON Serialization Methods
    nlohmann::json toJson() const;
    static Flight fromJson(const nlohmann::json &j);

    
};

#endif