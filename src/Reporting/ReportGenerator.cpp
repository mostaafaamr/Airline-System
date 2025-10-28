#include "../../include/Reporting/ReportGenerator.hpp"

void ReportGenerator::generateFlightPerformanceReport(const std::string& month, const std::string& year) const
{
    int totalFlightsScheduled = 0;
    int flightsCompleted = 0;
    int flightsDelayed = 0;
    int flightsCanceled = 0;
    int totalReservations = 0;
    double totalRevenue = 0.0;
    auto flights = flightService.getFlightsForReport();

    // Loop through flights and reservations to calculate metrics
    for (const auto& flight : flights)
    {
        // Check if flight is in the specified month/year
        if (flight.getDepartureDate().substr(0, 7) == year + "-" + month) 
        {
            totalFlightsScheduled++;
            if (flight.getStatus() == "Completed") flightsCompleted++;
            else if (flight.getStatus() == "Delayed") flightsDelayed++;
            else if (flight.getStatus() == "Canceled") flightsCanceled++;

            // Calculate reservations and revenue for this flight
            auto [reservationsCount, revenue] = reservationService.countReservationsAndRevenue(flight.getFlightNumber());
            totalReservations += reservationsCount;
            totalRevenue += revenue;
        }
    }

    // Display the report
    std::cout << "Flight Performance Report for " << month << "-" << year << std::endl;
    std::cout << "----------------------------------------"<<std::endl;
    std::cout << "Total Flights Scheduled: " << totalFlightsScheduled << std::endl;
    std::cout << "Flights Completed: " << flightsCompleted <<std::endl;
    std::cout << "Flights Delayed: " << flightsDelayed << std::endl;
    std::cout << "Flights Canceled: " << flightsCanceled << std::endl;
    std::cout << "Total Reservations Made: " << totalReservations << std::endl;
    std::cout << "Total Revenue: $" << totalRevenue << std::endl;
    std::cout << "----------------------------------------"<<std::endl;

    totalFlightsScheduled = 0;
    // Loop through flights to get performance of each flight
    for (const auto& flight : flights)
    {
        // Check if flight is in the specified month/year
        if (flight.getDepartureDate().substr(0, 7) == year + "-" + month) 
        {
            totalFlightsScheduled++;
            auto [reservationsCount, revenue] = reservationService.countReservationsAndRevenue(flight.getFlightNumber());
            if (flight.getStatus() == "Completed" || flight.getStatus() == "Delayed") 
            {   
                std::cout<<totalFlightsScheduled<<". "<<"Flight "<<flight.getFlightNumber()<<": ";
                std::cout<<flight.getStatus()<<" ("<<reservationsCount<<"Bookings, "<<revenue<<")"<<std::endl;
            }
            else if (flight.getStatus() == "Canceled")
            {
                std::cout<<totalFlightsScheduled<<". "<<"Flight "<<flight.getFlightNumber()<<": ";
                std::cout<<flight.getStatus()<<std::endl;
            }
        }
    }
}

void ReportGenerator::generateMaintenanceReport(const Aircraft& aircraft) const
{

    std::cout << "Maintenance Report for Aircraft " << aircraft.getId() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Maintenance Logs: " << std::endl;
    for (const auto &log : aircraft.getMaintenanceLogs())
    {
        std::cout << "- Date: " << log.first << ", Description: " << log.second << std::endl;
    }
    std::cout << "----------------------------------------\n";
    std::cout << "Maintenance Schedule:\n";
    for (const auto &date : aircraft.getMaintenanceSchedule())
    {
        std::cout << "- " << date << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Utilization: " << aircraft.getUtilization() << " hours" << std::endl;
}


void ReportGenerator::generateUserActivityReport(const std::optional<std::string>& userId) const
{
    // Read the activity log
    nlohmann::json log;
    std::ifstream logFile("data/reports/user_activity.json");
    if (!logFile.good())
    {
        std::cout << "No activity log found.\n";
        return;
    }
    logFile >> log;
    logFile.close();

    if (userId) // 🔹 Generate report for a specific user
    {
        std::cout << "Activity Report for User ID: " << *userId << "\n";
        std::cout << "----------------------------------------\n";
        bool found = false;
        for (const auto &activity : log)
        {
            if (activity["userId"] == *userId)
            {
                std::cout << "Action: " << activity["action"] << "\n";
                std::cout << "Timestamp: " << activity["timestamp"] << "\n";
                std::cout << "Details: " << activity["details"] << "\n";
                std::cout << "----------------------------------------\n";
                found = true;
            }
        }
        if (!found)
        {
            std::cout << "No activities found for this user.\n";
        }
    }
    else // 🔹 Generate system-wide report
    {
        std::cout << "System-Wide User Activity Report\n";
        std::cout << "----------------------------------------\n";
        for (const auto &activity : log)
        {
            std::cout << "User ID: " << activity["userId"] << "\n";
            std::cout << "Action: " << activity["action"] << "\n";
            std::cout << "Timestamp: " << activity["timestamp"] << "\n";
            std::cout << "Details: " << activity["details"] << "\n";
            std::cout << "----------------------------------------\n";
        }
    }
}
