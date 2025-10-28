#ifndef ADMINISTRATOR_HPP
#define ADMINISTRATOR_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include "User.hpp"
#include "../Flight/Aircraft.hpp"
#include "../Flight/Flight.hpp"
#include "../Flight/FlightService.hpp"
#include "../Flight/CrewService.hpp"
#include "../Booking/ReservationServiceAdmin.hpp"
#include "../Reporting/ReportGenerator.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/JsonUtils.hpp"
#include "../Utils/Logger.hpp"

class Administrator : public User
{
private:
    std::vector<Flight> flights;
    std::vector<Aircraft> aircrafts;
    std::vector<User> users;

    //Static flight service member to handle viewing flights
    static inline FlightService flightService{};

    //Static reservation service member to handle viewing reservations
    static inline ReservationServiceAdmin reservationService{};

    //Static report generator member to handle report generation
    static inline ReportGenerator reportGenerator{}; 

    //Static report generator member to handle report generation
    static inline ActivityLogger activityLogger{};
 
    // Helper methods for JSON file handling
    void loadFlightsFromJson(const std::string &filename);
    void saveFlightsToJson(const std::string &filename);
    void loadAircraftFromJson(const std::string &filename);
    void saveAircraftToJson(const std::string &filename);

    void saveCrewToJson(const nlohmann::json &crewData, const std::string &filename)
    {
        JsonUtils::saveJsonToFile(crewData, filename);
    }

    void loadUsersFromJson(const std::string &filename);
    void saveUsersToJson(const std::string &filename);

    void createSeatsForNewFlight(const std::string& flightNumber, const std::string& aircraftType);
    void removeFlightSeats(const std::string &flightNumber);
    void viewCrewForFlight(const std::string &flightNumber);

    void notifyPassengers(const Flight& flight) const
    {
        std::cout << std::endl << "Notification: Flight " << flight.getFlightNumber() << " is now " << flight.getStatus() <<std::endl;
    }


    // Helper methods for menu functionality
    void manageFlights();
    void manageAircrafts();
    void manageUsers();
    void reportsMenu();

    void addFlightMenu();
    void updateFlightMenu();
    void deleteFlightMenu();
    
    void addAircraftMenu();
    void updateAircraftMenu();
    void deleteAircraftMenu();

    void addUserMenu();
    void updateUserMenu();
    void deleteUserMenu();

public:
    Administrator(const std::string &id, const std::string &username, const std::string &password);


    // User management methods
    bool createUser(const User& user);
    void updateUser(const std::string &id, const User& updatedUser);
    void deleteUser(const std::string &id);
    const std::vector<User>& getUsers() const{ return users; }
    
    // Flight management
    void addFlight(const Flight& flight);
    void updateFlight(const std::string &flightNumber, const Flight& updatedFlight);
    void updateFlightStatus(Flight& flight, const std::string& newStatus);
    void deleteFlight(const std::string &flightNumber);
    const std::vector<Flight>& getFlights() const{ return flights; }
    void assignCrewToFlight(const std::string &flightNumber);

    // Viewing flights
    void viewFlights() const;
    // Viewing aircrafts
    void viewAircrafts() const;
    // Viewing users
    void viewUsers() const;

    // Aircraft management
    void addAircraft(const Aircraft& aircraft);
    void updateAircraft(const std::string &aircraftId, const Aircraft& updatedAircraft);
    void deleteAircraft(const std::string &aircraftId);
    const std::vector<Aircraft>& getAircrafts() const { return aircrafts; }

    // Generate a flight performance report for an aircraft
    void generateFlightPerformanceReport(const std::string& month, const std::string& year) const;

    // Schedule maintenance for an aircraft
    void scheduleMaintenance(const std::string& aircraftId, const std::string& date);

    // Add a maintenance log for an aircraft
    void addMaintenanceLog(const std::string& aircraftId, const std::string& log, const std::string& date);

    // Generate a maintenance report for an aircraft
    void generateMaintenanceReport(const std::string& aircraftId) const;

    // Generate user activity report
    void generateUserActivityReport(const std::optional<std::string>& userId = std::nullopt) const;

    // Main Menu
    void adminMenu();
    
};

#endif