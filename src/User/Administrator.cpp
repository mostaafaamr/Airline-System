#include "../../include/User/Administrator.hpp"

Administrator::Administrator(const std::string &id, const std::string &username, const std::string &password)
    : User(id, username, password, "Administrator")
{
    loadFlightsFromJson("data/flights.json");
    loadAircraftFromJson("data/aircraft.json"); 
    loadUsersFromJson("data/users.json");
}

void Administrator::loadUsersFromJson(const std::string &filename)
{
    // Clear the existing users
    users.clear();

    try
    {
        auto usersJson = JsonUtils::readJsonFromFile(filename);

        for (const auto &userJson : usersJson)
        {
            users.push_back(User::fromJson(userJson));
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error loading users from file: " << e.what() << std::endl;
    }
}

void Administrator::loadFlightsFromJson(const std::string &filename)
{
    // Clear the existing flights
    flights.clear();

    try
    {
        nlohmann::json flightsJson = JsonUtils::readJsonFromFile(filename);

        for (const auto &flightJson : flightsJson)
        {
            flights.push_back(Flight::fromJson(flightJson));
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error loading flights from file: " << e.what() << std::endl;
    }
}

void Administrator::saveFlightsToJson(const std::string &filename)
{
    nlohmann::json flightsJson;

    // Save all flights
    for (const auto &flight : flights)
    {
        flightsJson.push_back(flight.toJson());
    }

    // Use overwrite = true to replace the entire file
    JsonUtils::saveJsonToFile(flightsJson, filename);
}

void Administrator::loadAircraftFromJson(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: File not found or cannot be opened: " << filename << std::endl;
        return;
    }

    // Clear the existing aircrafts
    aircrafts.clear();

    try
    {
        // Read the JSON data from the file
        nlohmann::json aircraftsJson = JsonUtils::readJsonFromFile(filename);

        // Debug: Print the JSON data to verify its structure
       // std::cout << "Loaded JSON data: " << aircraftsJson.dump(4) << std::endl;

        // Ensure the JSON data is an array
        if (!aircraftsJson.is_array()) {
            throw std::runtime_error("Invalid JSON structure: Expected an array of aircraft objects.");
        }

        // Handle nested array (if present)
        nlohmann::json aircraftArray;
        if (aircraftsJson.size() == 1 && aircraftsJson[0].is_array()) {
            // If the outer array contains a single inner array, use the inner array
            aircraftArray = aircraftsJson[0];
        } else {
            // Otherwise, assume the outer array contains the aircraft objects directly
            aircraftArray = aircraftsJson;
        }

        // Parse each aircraft object
        for (const auto &aircraftJson : aircraftArray)
        {
            // Ensure each item is an object
            if (!aircraftJson.is_object()) {
                throw std::runtime_error("Invalid JSON structure: Expected an aircraft object.");
            }

            // Convert JSON to Aircraft object and add to the list
            aircrafts.push_back(Aircraft::fromJson(aircraftJson));
        }

       // std::cout << "Successfully loaded " << aircrafts.size() << " aircraft(s) from " << filename << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error loading aircrafts from file: " << e.what() << std::endl;
    }
}

void Administrator::saveAircraftToJson(const std::string &filename)
{
    nlohmann::json aircraftsJson;

    // Save all aircrafts
    for (const auto &aircraft : aircrafts)
    {
        aircraftsJson.push_back(aircraft.toJson());
    }

    // Use overwrite = true to replace the entire file
    JsonUtils::saveJsonToFile(aircraftsJson, filename);
}

void Administrator::saveUsersToJson(const std::string &filename)
{
    nlohmann::json usersJson;
    for (const auto &user : users) {
        usersJson.push_back(user.toJson());
    }
    JsonUtils::saveJsonToFile(usersJson, filename);
}

void Administrator::createSeatsForNewFlight(const std::string& flightNumber, const std::string& aircraftType) 
{
    nlohmann::json seatsData = JsonUtils::readJsonFromFile("data/seats.json");

    // Check if flight already exists in seats.json
    if (seatsData.contains(flightNumber)) 
    {
        return; // Flight seats already exist
    }

    nlohmann::json seatMap;
    std::string seatLabels = "ABCDEF"; // Standard seat labels

    int cols = 6;
    int rows;
    bool aircraftFound = false;

    // Get the correct row and column values from the aircraft vector
    for (const auto& aircraft : aircrafts) 
    {
        if (aircraft.getAircraftType() == aircraftType) 
        {
            rows = aircraft.getRows();
            aircraftFound = true;
            break;
        }
    }

    if (!aircraftFound) 
    {
        std::cerr << "Error: Aircraft type not found!" << std::endl;
        std::cout << "Press any key to continue... " << std::endl;
        std::cin.get(); // Waits for a single character (e.g., Enter)
        return;
    }
    for (int row = 1; row <= rows; row++) 
    {
        for (int col = 0; col < cols; col++) 
        {
            std::string seatId = std::to_string(row) + seatLabels[col];
            seatMap[seatId] = "available";
        }
    }

    // Store the seat data
    seatsData[flightNumber] = {
        {"rows", rows},
        {"cols", cols},
        {"seats", seatMap}
    };

    JsonUtils::saveJsonToFile(seatsData, "data/seats.json");
    std::cout << "Press any key to continue... " << std::endl;
        std::cin.get(); // Waits for a single character (e.g., Enter)
}


bool Administrator::createUser(const User& newUser)
{
    std::string roleLower = Utils::toLowerCase(newUser.getRole());

    // Validate role (e.g., only allow "Passenger", "BookingAgent", or "Administrator")
    if (roleLower != "passenger" && roleLower != "booking agent" && roleLower != "administrator")
    {
        std::cout << "Invalid role assigned. Role must be 'Passenger', 'Booking Agent', or 'Administrator'." << std::endl;
        return false;
    }

    // Check for duplicate user ID or username
    for (const auto &user : users)
    {
        if (user.getId() == newUser.getId())
        {
            std::cout << "A user with the same ID already exists." << std::endl;
            return false;
        }
        if (user.getUsername() == newUser.getUsername())
        {
            std::cout << "A user with the same username already exists." << std::endl;
            return false;
        }
    }

    // Add the new user to the vector
    users.push_back(newUser);

    // Save the updated users to the JSON file
    saveUsersToJson("data/users.json");

    // Log the activity
    activityLogger.logActivity(id, "admin", "Created user", "Username: " + newUser.getUsername() + ", Role: " + newUser.getRole());
    std::cout << "User created successfully." << std::endl;

    return true;
}

void Administrator::updateUser(const std::string &id, const User& updatedUser)
{
    // Validate the role of the updated user
    std::string roleLower = Utils::toLowerCase(updatedUser.getRole());
    if (roleLower != "passenger" && roleLower != "booking agent" && roleLower != "administrator")
    {
        std::cout << "Invalid role assigned. Role must be 'Passenger', 'Booking Agent', or 'Administrator'." << std::endl;
        return;
    }

    // Search for the user by ID
    for (auto &user : users)
    {
        if (user.getId() == id)
        {
            // Update the user
            user = updatedUser;
            saveUsersToJson("data/users.json");
            activityLogger.logActivity(id, "admin", "Updated user", "User ID: " + id);
            std::cout << "User with ID: " << id << " updated successfully." << std::endl;
            return;
        }
    }

    std::cout << "The user with ID " << id << " does not exist." << std::endl;


}

void Administrator::deleteUser(const std::string &id)
{
    // Remove the user from the vector
    auto initialSize = users.size();
    users.erase(std::remove_if(users.begin(), users.end(),
                               [id](const User &user)
                               { return user.getId() == id; }),
                users.end());

    // Check if a user was deleted
    if (users.size() < initialSize)
    {
        // Save the updated vector to the JSON file
        nlohmann::json usersJson;
        for (const auto &user : users)
        {
            usersJson.push_back(user.toJson());
        }
        JsonUtils::saveJsonToFile(usersJson, "data/users.json");

        // Log the activity
        activityLogger.logActivity(id, "admin", "Deleted user", "User ID: " + id);
        std::cout << "User with ID: " << id << " deleted successfully." << std::endl;
    }
    else
    {
        std::cout << "User with ID " << id << " not found." << std::endl;
    }
}


void Administrator::addAircraft(const Aircraft &aircraft)
{
    aircrafts.push_back(aircraft);
    activityLogger.logActivity(id, "admin", "Added Aircraft", "Aircraft Id: " + aircraft.getId());
    saveAircraftToJson("data/aircraft.json");
}

void Administrator::updateAircraft(const std::string &aircraftId, const Aircraft &updatedAircraft)
{

    for (auto &aircraft : aircrafts)
    {
        if (aircraft.getId() == aircraftId)
        {
            // Update the aircraft's data with the new data
            aircraft = updatedAircraft;
            saveAircraftToJson("data/aircraft.json");
            activityLogger.logActivity(id, "admin", "Updated Aircraft", "Aircraft Id: " + updatedAircraft.getId());
            return;
        }
    }
    // If aircraft isn't found
    std::cout << "The aircraft given doesn't exist" << std::endl;
}

void Administrator::deleteAircraft(const std::string &aircraftId)
{
    // Remove the aircraft from the vector
    auto initialSize = aircrafts.size();
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                  [aircraftId](const Aircraft &aircraft)
                                  { return aircraft.getId() == aircraftId; }),
                    aircrafts.end());

    // Check if an aircraft was deleted
    if (aircrafts.size() < initialSize)
    {
        // Save the updated vector to the JSON file
        nlohmann::json aircraftsJson;
        for (const auto &aircraft : aircrafts)
        {
            aircraftsJson.push_back(aircraft.toJson());
        }
        JsonUtils::saveJsonToFile(aircraftsJson, "data/aircraft.json");

        // Log the activity
        activityLogger.logActivity(aircraftId, "admin", "Deleted aircraft", "Aircraft ID: " + aircraftId);
        std::cout << "Aircraft with ID: " << aircraftId << " deleted successfully." << std::endl;
    }
    else
    {
        std::cout << "Aircraft with ID " << aircraftId << " not found." << std::endl;
    }
}



void Administrator::addFlight(const Flight &flight)
{
    createSeatsForNewFlight(flight.getFlightNumber(), flight.getAircraftType()); // Ensure seat map is created
    flights.push_back(flight);
    activityLogger.logActivity(id, "admin", "Added Flight", "Flight Number: " + flight.getFlightNumber());
    saveFlightsToJson("data/flights.json");
}
void Administrator::updateFlight(const std::string &flightNumber, const Flight &updatedFlight)
{
    for (auto &flight : flights)
    {
        if (flight.getFlightNumber() == flightNumber)
        {
            // Update the flight's data with the new data
            flight = updatedFlight;
            saveFlightsToJson("data/flights.json");
            activityLogger.logActivity(id, "admin", "Updated Flight", "Flight Number: " + flight.getFlightNumber());
            std::cout<<"Flight details updated successfully!"<<std::endl;
            return;
        }
    }
    // If flight isn't found
    std::cout << "The flight given doesn't exist" << std::endl;
}

void Administrator::updateFlightStatus(Flight& flight, const std::string& newStatus)
{
    flight.setStatus(newStatus);
    updateFlight(flight.getFlightNumber(),flight);
    notifyPassengers(flight);
}

void Administrator::deleteFlight(const std::string &flightNumber)
{
    // Remove the flight from the vector
    auto initialSize = flights.size();
    flights.erase(std::remove_if(flights.begin(), flights.end(),
                                 [flightNumber](const Flight &flight)
                                 { return flight.getFlightNumber() == flightNumber; }),
                  flights.end());

    // Check if a flight was deleted
    if (flights.size() < initialSize)
    {
        // Save the updated vector to the JSON file
        nlohmann::json flightsJson;
        for (const auto &flight : flights)
        {
            flightsJson.push_back(flight.toJson());
        }
        JsonUtils::saveJsonToFile(flightsJson, "data/flights.json");

        // Remove the flight's seat data from seats.json
        removeFlightSeats(flightNumber);

        // Log the activity
        activityLogger.logActivity(flightNumber, "admin", "Deleted flight", "Flight Number: " + flightNumber);
        std::cout << "Flight with number: " << flightNumber << " deleted successfully." << std::endl;
    }
    else
    {
        std::cout << "Flight with number " << flightNumber << " not found." << std::endl;
    }
}

void Administrator::removeFlightSeats(const std::string &flightNumber)
{

    // Read the seats.json file
    nlohmann::json seatsJson;
    std::ifstream file("data/seats.json");
    if (file.good())
    {
        file >> seatsJson;
        file.close();
    }

    // Check if the flight exists in the seats data
    if (seatsJson.contains(flightNumber))
    {
        // Remove the flight's seat data
        seatsJson.erase(flightNumber);

        // Save the updated seats data to the JSON file using JsonUtils::saveJsonToFile
        JsonUtils::saveJsonToFile(seatsJson, "data/seats.json");

        std::cout << "Seat data for flight " << flightNumber << " removed successfully." << std::endl;
    }
    else
    {
        std::cout << "No seat data found for flight " << flightNumber << "." << std::endl;
    }
}

void Administrator::viewCrewForFlight(const std::string &flightNumber)
{
    // Create a CrewService instance
    CrewService crewService("data/crew.json");

    // Retrieve crew assignments for the flight
    auto crewMembers = crewService.getCrewForFlight(flightNumber);

    if (crewMembers.empty())
    {
        std::cout << "No crew assigned to flight " << flightNumber << std::endl;
        return;
    }

    std::cout << "--- Crew Assignments for Flight " << flightNumber << " ---" << std::endl;
    for (const auto &crewMember : crewMembers)
    {
        std::cout << "ID: " << crewMember->getCrewId() << " - " << crewMember->getName()
                  << " (" << crewMember->getRole() << ")" << std::endl;
    }
}

void Administrator::viewFlights() const
{
    Utils::clearScreen();
    if (flights.empty())
    {
        std::cout << "No flights available." << std::endl;
        return;
    }

    for (const auto &flight : flights)
    {
        std::cout << "\n\nFlight Number: " << flight.getFlightNumber() << std::endl;
        std::cout << "Departure: " << flight.getDepartureDateAndTime() << std::endl;
        std::cout << "Arrival: " << flight.getArrivalDate()  << std::endl;
        std::cout << "Aircraft: " << flight.getAircraftType() << std::endl;
        std::cout << "Available Seats: " << flight.getAvailableSeats() << std::endl;
        std::cout << "Price: $" << std::fixed << std::setprecision(2) << flight.getPrice() << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
    
}

void Administrator::viewAircrafts() const
{
    Utils::clearScreen();
    if (aircrafts.empty())
    {
        std::cout << "No aircrafts available." << std::endl;
        return;
    }

    for (const auto &aircraft : aircrafts)
    {
        std::cout << "\n\nAircraft Id: " << aircraft.getId() << std::endl;
        std::cout << "Aircraft Model: " << aircraft.getAircraftType()<<std::endl;
        std::cout << "Capacity: " << aircraft.getCapacity() << std::endl;
        std::cout << "Maintenance Due: " << aircraft.getMaintenanceDue() << std::endl;
        std::cout << "Status: " << aircraft.getStatus() << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
}

void Administrator::viewUsers() const
{
    Utils::clearScreen();
    if (users.empty())
    {
        std::cout << "No users available." << std::endl;
        return;
    }

    for (const auto &user : users)
    {
        std::cout << "\n\nUser Id: " << user.getId() << std::endl;
        std::cout << "Username: " << user.getUsername()<<std::endl;
        std::cout << "Role: " << user.getRole() << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
}


void Administrator::generateFlightPerformanceReport(const std::string& month, const std::string& year) const
{
    reportGenerator.generateFlightPerformanceReport(month,year);
    activityLogger.logActivity(id, "admin", "Generated Flight Performance Report");
}

void Administrator::scheduleMaintenance(const std::string& aircraftId, const std::string& date)
{

    for (auto &aircraft : aircrafts)
    {
        if (aircraft.getId() == aircraftId)
        {
            aircraft.addMaintenanceSchedule(date);
            std::cout << "Maintenance scheduled for Aircraft " << aircraftId << " on " << date <<std::endl;
            activityLogger.logActivity(id, "admin", "Scheduled Maintenance", "Aircraft Id: " + aircraftId);
            return;
        }
    }

    std::cout << "Aircraft " << aircraftId << " not found" << std::endl;
}

void Administrator::addMaintenanceLog(const std::string& aircraftId, const std::string& log, const std::string& date)
{
    // Update the aircraft's maintenance logs
    for (auto &aircraft : aircrafts)
    {
        if (aircraft.getId() == aircraftId)
        {
            aircraft.addMaintenanceLog(log, date);
            std::cout << "Maintenance log added for Aircraft " << aircraftId << std::endl;
            activityLogger.logActivity(id, "admin", "Added Maintenance Log", "Aircraft Id: " + aircraftId);
            return;
        }
    }

    std::cout << "Aircraft " << aircraftId << " not found" << std::endl;
}


void Administrator::generateMaintenanceReport(const std::string& aircraftId) const
{
    for (const auto &aircraft : aircrafts)
    {
        if (aircraft.getId() == aircraftId)
        {
            reportGenerator.generateMaintenanceReport(aircraft);
            activityLogger.logActivity(id, "admin", "Generated Maintenance Report for Aircraft", "Aircraft Id: " + aircraftId );
            return;
        }
    }
    std::cout << "Aircraft " << aircraftId << " not found.\n";
}

void Administrator::generateUserActivityReport(const std::optional<std::string>& userId) const
{
    reportGenerator.generateUserActivityReport(userId);
}




void Administrator::adminMenu()
{
    while (true)
    {
        int choice;
        Utils::clearScreen();
        std::cout << "--- Administrator Menu ---" << std::endl;
        std::cout << "1. Manage Flights" << std::endl;
        std::cout << "2. Manage Aircraft" << std::endl;
        std::cout << "3. Manage Users" << std::endl;
        std::cout << "4. Generate Reports" << std::endl;
        std::cout << "5. Logout" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice)
        {
        case 1:
            manageFlights();
            break;
        case 2:
            manageAircrafts();
            break;
        case 3:
            manageUsers();
            break;
        case 4:
            reportsMenu();
            break;
        case 5:
            logout();
            Utils::clearScreen();
            return;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void Administrator::manageFlights()
{
    int choice;
    while (true)
    {
        Utils::clearScreen();
        std::cout << "1. Add New Flight" << std::endl;
        std::cout << "2. Update Existing Flight" << std::endl;
        std::cout << "3. Remove Flight" << std::endl;
        std::cout << "4. View All Flights" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice)
            {
            case 1:
                addFlightMenu();
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
                break;
            case 2:
                updateFlightMenu();
                break;
            case 3:
                deleteFlightMenu();
                break;
            case 4:
                Utils::clearScreen();
                viewFlights();
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
                break;
            case 5:
                Utils::clearScreen();
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
            }

    }
}


void Administrator::manageAircrafts()
{
    std::string aircraftId, aircraftType, status, maintenanceDue;
    int choice;
    while (true)
    {
        Utils::clearScreen();
        std::cout << "--- Manage Aircraft ---" << std::endl;
        std::cout << "1. Add New Aircraft" << std::endl;
        std::cout << "2. Update Existing Aircraft" << std::endl;
        std::cout << "3. Remove Aircraft" << std::endl;
        std::cout << "4. View All Aircraft" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice)
        {
        case 1: // Add New Aircraft
            addAircraftMenu();
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            break;

        case 2: // Update Existing Aircraft
            updateAircraftMenu();
            break;

        case 3: // Remove Aircraft
            deleteAircraftMenu();
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            break;

        case 4: // View All Aircrafts
            Utils::clearScreen();
            viewAircrafts();
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            Utils::clearScreen();
            break;

        case 5: // Back to Main Menu
            Utils::clearScreen();
            return;
            break;

        default: // Invalid choice
            std::cout << "Invalid choice! Please try again.\n";
            break;
        }
    }
}

void Administrator::manageUsers()
{
    std::string userId, username, password, role;
    int choice;

    while (true) // Keeps looping inside "Manage Users"
    {
        Utils::clearScreen();
        std::cout << "--- Manage Users ---" << std::endl;
        std::cout << "1. Add New User" << std::endl;
        std::cout << "2. Update Existing User" << std::endl;
        std::cout << "3. Remove User" << std::endl;
        std::cout << "4. View All Users" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: // Add New User
                addUserMenu();
                break;
        
            case 2: // Update Existing User
                updateUserMenu();
                break;
        
            case 3: // Remove User
                deleteUserMenu();
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
                break;
        
            case 4: // View All User
                Utils::clearScreen();
                viewUsers();
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
                break;
        
            case 5: // Back to Main Menu
                Utils::clearScreen();
                return;
                break;
        
            default: // Invalid choice
                std::cout << "Invalid choice! Please try again.\n";
                break;
        }
    }
}

void Administrator::reportsMenu()
{
    int choice;

    while (true) // Generate Reports
    {
        Utils::clearScreen();
        std::cout << "--- Generate Reports ---" << std::endl;
        std::cout << "1. Operational Reports" << std::endl;
        std::cout << "2. Maintenance Reports" << std::endl;
        std::cout << "3. User Activity Reports" << std::endl;
        std::cout << "4. Back to Main Menu" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice)
        {
            case 1: // Generate Operational Reports
            {
                std::string month, year;
                Utils::clearScreen();
                std::cout << "--- Operational Reports ---" << std::endl;
                std::cout << "Enter Month and Year for Report (MM-YYYY): ";
                std::string input;
                std::getline(std::cin, input);

                while (input.length() != 7 && input[2] != '-')
                {
                    std::cout << "Invalid format! Please enter in MM-YYYY format: ";
                    std::getline(std::cin, input);
                }
               
                // Extract month and year
                month = input.substr(0, 2);
                year = input.substr(3, 4);
                std::cout << "\nGenerating Operational Report for " << month << "-" << year << "..." << std::endl;
                generateFlightPerformanceReport(month, year);
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
            }
                break;
        
            case 2: // Generate Maintenance Reports
            {
                std::string aircraftId;
                Utils::clearScreen();
                std::cout << "--- Maintenance Reports ---" << std::endl;
                std::cout << "Enter Aircraft ID: ";
                std::getline(std::cin, aircraftId);

                std::cout << "\nGenerating Maintenance Report for Aircraft " << aircraftId << "..." << std::endl;
                generateMaintenanceReport(aircraftId);
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
            }
                break;
        
            case 3: // User Activity Reports
            {
                Utils::clearScreen();
                std::cout << "--- User Activity Reports ---" << std::endl;
                std::cout << "1. Generate System-Wide Activity Report" << std::endl;
                std::cout << "2. Generate Report for a Specific User" << std::endl;
                std::cout << "3. Back to Reports Menu" << std::endl;
                std::cout << "Enter choice: ";
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (choice == 1) // Generate for All Users
                {
                    Utils::clearScreen();
                    std::cout << "\nGenerating system-wide user activity report..." << std::endl;
                    generateUserActivityReport();
                    std::cout << "Press any key to continue... " << std::endl;
                    std::cin.get(); // Waits for a single character (e.g., Enter)
                }
                else if (choice == 2) // Generate for a Specific User
                {
                    Utils::clearScreen();
                    viewUsers();
                    std::string userId;
                    std::cout << "\nEnter User Id: ";
                    std::getline(std::cin, userId);

                    std::cout << "\nGenerating activity report for " << userId << "..." << std::endl;
                    generateUserActivityReport(userId);
                    std::cout << "Press any key to continue... " << std::endl;
                    std::cin.get(); // Waits for a single character (e.g., Enter)
                }
                else if (choice == 3) // Back to Reports Menu
                {
                    Utils::clearScreen();
                }
                else
                {
                    std::cout << "Invalid choice! Please try again" << std::endl;
                    std::cout << "Press any key to continue... " << std::endl;
                    std::cin.get(); // Waits for a single character (e.g., Enter)
                }
            }
                break;
        
            case 4: // Back to Main Menu
                Utils::clearScreen();
                return;
                break;
        
            default: // Invalid choice
                std::cout << "Invalid choice! Please try again.\n";
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
                break;
        }
    }
}

void Administrator::addFlightMenu()
{
    Utils::clearScreen();
    std::string flightNumber, origin, destination, departure, arrival, aircraftId, aircraftType, status;
    int price;
    std::cout << std::endl
              << "Enter Flight Number: ";
    std::getline(std::cin, flightNumber);
    std::cout << std::endl
              << "Enter Origin: ";
    std::getline(std::cin, origin);
    std::cout << std::endl
              << "Enter Destination: ";
    std::getline(std::cin, destination);
    std::cout << std::endl
              << "Enter Departure Date and Time (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, departure);
    departure += ":00Z";
    std::cout << std::endl
              << "Enter Arrival Date and Time (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, arrival);
    arrival += ":00Z";
    Utils::clearScreen();

    std::cout<<"Available Aircrafts: "<<std::endl;
    viewAircrafts();
    std::cout << std::endl
              << "Enter Aircraft Id: ";
    std::getline(std::cin, aircraftId);
    Utils::clearScreen();

    int totalSeats = 0;
    bool aircraftFound = false;

    for (const auto &aircraft : aircrafts)
    {
        if (aircraft.getId() == aircraftId) // Get total seats from Aircraft capacity
        {
            totalSeats = aircraft.getCapacity();
            aircraftType = aircraft.getAircraftType();
            std::cout << "Aircraft " << aircraftId << " has a capacity of " << totalSeats << " seats." << std::endl;
            aircraftFound = true;
            break;
        }
    }

    if (!aircraftFound)
    {
        std::cerr << "Error: Aircraft type not found! Please enter a valid type." << std::endl;
        return; // Exit function if aircraft is invalid
    }

    std::cout << std::endl
              << "Enter Status (Scheduled/Delayed/Canceled): ";
    std::getline(std::cin, status);
    std::cout << std::endl
              << "Enter Price: ";
    std::cin >> price;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Flight flight(flightNumber, origin, destination, departure, arrival, aircraftType, status, totalSeats, totalSeats, price);
    
    addFlight(flight);
    std::cout << std::endl
              << "Flight " << flightNumber << " has been successfully added to the schedule\nS";
}

void Administrator::updateFlightMenu()
{
    Utils::clearScreen();
    std::string flightNumber, origin, destination, departure, arrival, aircraftType, status;
    int price;
    int choice;
    viewFlights();
    std::cout << std::endl
              << "Enter Flight Number to update: ";
    std::getline(std::cin, flightNumber);
    Utils::clearScreen();
    bool flightFound = false;
    for (auto &flight : flights)
    {

        if (flight.getFlightNumber() == flightNumber)
        {
            while (true)
            {
                flightFound = true;
                Utils::clearScreen();
                std::cout << "Select information to update: " << std::endl;
                std::cout << "1.Flight Details: " << std::endl;
                std::cout << "2.Crew Assignment: " << std::endl;
                std::cout << "3.Status: " << std::endl;
                std::cout << "4.Back to Manage Flights: " << std::endl;
                std::cout << "Enter Choice: " << std::endl;
                int updateChoice;
                std::cin >> updateChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                
                if (updateChoice == 1)
                {
                    while (true)
                    {
                        Utils::clearScreen();
                        std::cout << "Current Flight Details: " << std::endl;
                        std::cout << "----------------------------" << std::endl;

                        std::cout << std::endl
                                  << "Flight Number: " << flightNumber << std::endl;
                        std::cout << std::endl
                                  << "Origin: " << flight.getOrigin() << std::endl;
                        std::cout << std::endl
                                  << "Destination: " << flight.getDestination() << std::endl;
                        std::cout << std::endl
                                  << "Departure Date and Time (YYYY-MM-DD HH:MM): " << flight.getDepartureDateAndTime() << std::endl;
                        std::cout << std::endl
                                  << "Arrival Date and Time (YYYY-MM-DD HH:MM): " << flight.getArrivalDate() << std::endl;
                        std::cout << std::endl
                                  << "Status: " << flight.getStatus() << std::endl;
                        std::cout << "----------------------------" << std::endl;

                        std::cout << "Select field to update: (0 to cancel)" << std::endl;
                        std::cout << "1. Departure Time" << std::endl;
                        std::cout << "2. Arrival Time" << std::endl;
                        std::cout << "3. Origin" << std::endl;
                        std::cout << "4. Destination" << std::endl;
                        std::cout << "5. Price" << std::endl;
                        std::cout << "Enter your choice: ";
                        std::cin >> choice;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        Utils::clearScreen();
                        if (choice == 0)
                        {
                            break;
                        }
                        else if (choice == 1)
                        {
                            std::cout << std::endl
                                      << "Enter new Departure Date Time (YYYY-MM-DD HH:MM): ";
                            std::getline(std::cin, departure);
                            departure += ":00Z";
                            flight.setDepartureDate(departure);
                            updateFlight(flight.getFlightNumber(), flight);
                        }
                        else if (choice == 2)
                        {
                            std::cout << std::endl
                                      << "Enter new Arrival Time (YYYY-MM-DD HH:MM): ";
                            std::getline(std::cin, arrival);
                            flight.setArrivalDate(arrival);
                            arrival += ":00Z";
                            updateFlight(flight.getFlightNumber(), flight);
                        }
                        else if (choice == 3)
                        {
                            std::cout << std::endl
                                      << "Enter new Origin: ";
                            std::getline(std::cin, origin);
                            flight.setOrigin(origin);
                            updateFlight(flight.getFlightNumber(), flight);
                        }
                        else if (choice == 4)
                        {
                            std::cout << std::endl
                                      << "Enter new Destination: ";
                            std::getline(std::cin, destination);
                            flight.setDestination(destination);
                            updateFlight(flight.getFlightNumber(), flight);
                        }
                        else if (choice == 5)
                        {
                            std::cout << std::endl
                                      << "Enter new Price: ";
                            std::cin >> price;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            flight.setPrice(price);
                            updateFlight(flight.getFlightNumber(), flight);
                        }
                        std::cout << "Do you want to update another field? (Y/N): ";
                        char choice2;
                        std::cin >> choice2;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        if (choice2 == 'Y' || choice2 == 'y')
                        {
                            continue;
                        }
                        else
                            break;
                    }
                }

                else if(updateChoice == 2)
                {
                    Utils::clearScreen();
                    assignCrewToFlight(flightNumber);
                }
                else if(updateChoice == 3)
                {
                    std::cout << std::endl<< "Enter new Status (Scheduled/Delayed/Canceled): ";
                    std::getline(std::cin, status);
                    updateFlightStatus(flight, status);
                }
                else if(updateChoice == 4)
                {
                    break;
                }
                else
                {
                    std::cout<< "Invalid Choice "<<std::endl;
                }
            }
        }
    }

    if(!flightFound)
    {
        std::cout << "Flight wasn't found " << std::endl;
    }
}


void Administrator::deleteFlightMenu()
{
    Utils::clearScreen();
    std::string flightNumber;
    viewFlights();
    std::cout << std::endl
              << "Enter Flight Number to Remove: ";
    std::getline(std::cin, flightNumber);
    bool flightFound = false;
    for (auto &flight : flights)
    {
        if (flight.getFlightNumber() == flightNumber)
        {
            deleteFlight(flightNumber);
            flightFound = true;
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            Utils::clearScreen();
        }
    }
    if (!flightFound)
    {
        std::cout << "Flight Not Found" << std::endl;
        std::cout << "Press any key to continue... " << std::endl;
        std::cin.get(); // Waits for a single character (e.g., Enter)
    }
}

void Administrator::assignCrewToFlight(const std::string &flightNumber)
{
    // Find the flight
    auto flightOpt = flightService.findFlight(flightNumber);
    if (!flightOpt)
    {
        std::cout << "Flight not found" << std::endl;
        return;
    }

    Flight &flight = flightOpt.value().get();

    // Create a CrewService instance
    CrewService crewService("data/crew.json");
    crewService.assignCrewToFlight(flightNumber, flight);

    saveFlightsToJson("data/flights.json");
    std::cout << "Crew assigned successfully to flight " << flightNumber << "!" << std::endl;
    activityLogger.logActivity(id, "admin", "Assigned Crew", "Flight Number: " + flightNumber);
}


void Administrator::addAircraftMenu()
{
    std::string aircraftId, aircraftType, status, maintenanceDue;
    int capacity;
    Utils::clearScreen();
    std::cout << "\nEnter Aircraft ID: ";
    std::getline(std::cin, aircraftId);
    std::cout << std::endl
              << "Enter Aircraft Type: ";
    std::getline(std::cin, aircraftType);
    std::cout << std::endl
              << "Enter Capacity: ";
    std::cin >> capacity;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl
              << "Enter Maintenance Due (YYYY-MM-DD): ";
    std::getline(std::cin, maintenanceDue);
    std::cout << std::endl
              << "Enter Status (Active/Under Maintenance): ";
    std::getline(std::cin, status);

    Aircraft aircraft(aircraftId, aircraftType, capacity, maintenanceDue, status);
    addAircraft(aircraft);

    std::cout << "\nAircraft " << aircraftId << " has been successfully added" << std::endl;
}

void Administrator::updateAircraftMenu()
{
    Utils::clearScreen();
    std::string aircraftId, aircraftType, status, maintenanceDue;
    int choice;
    viewAircrafts();
    std::cout << "\nEnter Aircraft ID to update: " << std::endl;
    std::getline(std::cin, aircraftId);

    bool aircraftFound = false;
    for (auto &aircraft : aircrafts)
    {
        if (aircraft.getId() == aircraftId)
        {
            aircraftFound = true;

            while (true)
            {
                Utils::clearScreen();
                std::cout << "Current Aircraft Details: " << std::endl;
                std::cout << "----------------------------" << std::endl;
                std::cout << "Aircraft ID: " << aircraft.getId() << std::endl;
                std::cout << "Aircraft Type: " << aircraft.getAircraftType() << std::endl;
                std::cout << "Capacity: " << aircraft.getCapacity() << std::endl;
                std::cout << "Maintenance Due: " << aircraft.getMaintenanceDue() << std::endl;
                std::cout << "Status: " << aircraft.getStatus() << std::endl;
                std::cout << "----------------------------" << std::endl;

                Utils::clearScreen();
                std::cout << "Select field to update: (0 to cancel)" << std::endl;
                std::cout << "1. Update Status"<<std::endl;
                std::cout << "2. Add Maintenance Log"<<std::endl;
                std::cout << "3. Add Maintenance Schedule"<<std::endl;
                std::cout << "4. Back to Manage Aircrafts" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                Utils::clearScreen();

                if (choice == 0)
                {
                    break;
                }
                else if (choice == 1)
                {
                    std::string newStatus;
                    std::cout << "Enter new status (Active/Under Maintenance): ";
                    std::getline(std::cin, newStatus);
                    aircraft.setStatus(newStatus);
                    updateAircraft(aircraft.getId(), aircraft);
                    std::cout << "Aircraft status updated successfully!"<<std::endl;
                }
                else if (choice == 2)
                {
                    std::string log, date;
                    std::cout << "Enter maintenance log: ";
                    std::getline(std::cin, log);
                    std::cout << "Enter date (YYYY-MM-DD): ";
                    std::getline(std::cin, date);
                    aircraft.addMaintenanceLog(log,date);
                }
                else if (choice == 3)
                {
                    std::cout << "Enter new maintenance schedule date (YYYY-MM-DD): ";
                    std::string date;
                    std::getline(std::cin, date);
                    aircraft.addMaintenanceSchedule(date);
                }
                else if (choice == 4)
                {
                    return;
                }
                else 
                {
                    std::cout << "Invalid choice, try again"<<std::endl;
                }

                std::cout << "\nDo you want to update another field? (Y/N): ";
                char updateChoice;
                std::cin >> updateChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (updateChoice == 'Y' || updateChoice == 'y')
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
    }

    if (!aircraftFound)
    {
        std::cout << "Aircraft Not Found.\n";
        std::cout << "Press any key to continue... " << std::endl;
        std::cin.get(); // Waits for a single character (e.g., Enter)
    }
}

void Administrator::deleteAircraftMenu()
{
    Utils::clearScreen();
    std::string aircraftId;
    viewAircrafts();
    std::cout << "\nEnter Aircraft ID to Remove: ";
    std::getline(std::cin, aircraftId);

    bool aircraftFound = false;
    for (auto &aircraft : aircrafts)
    {
        if (aircraft.getId() == aircraftId)
        {
            deleteAircraft(aircraftId);
            aircraftFound = true;
            break;
        }
    }

    if (!aircraftFound)
    {
        std::cout << "Aircraft Not Found.\n";
        std::cout << "Press any key to continue... " << std::endl;
        std::cin.get(); // Waits for a single character (e.g., Enter)
    }
}

void Administrator::addUserMenu()
{
    std::string userId, username, password, role;
    Utils::clearScreen();
    std::cout << "\nEnter User ID: ";
    std::getline(std::cin, userId);
    std::cout << "\nEnter Username: ";
    std::getline(std::cin, username);
    std::cout << "\nEnter Password: ";
    password = Utils::getPassword();
    std::cout << "\nEnter Role (Administrator/Booking Agent/Passenger): ";
    std::getline(std::cin, role);

    User newUser(userId, username, role, password);
    if (createUser(newUser))
    {
        std::cout << "\nUser " << username << " has been successfully added" << std::endl;
        std::cout << "Press any key to continue... " << std::endl;
        std::cin.get(); // Waits for a single character (e.g., Enter)
        Utils::clearScreen();
    }
}

void Administrator::updateUserMenu()
{
    Utils::clearScreen();
    std::string userId, newUsername, newRole, newPassword, confirmPass;
    int choice;
    viewUsers(); // Assuming this function displays the list of users
    std::cout << std::endl << "Enter User ID to update: ";
    std::getline(std::cin, userId);
    Utils::clearScreen();
    bool userFound = false;

    for (auto &user : users)
    {
        if (user.getId() == userId)
        {
            userFound = true; // User found
            while (true)
            {
                Utils::clearScreen();
                std::cout << "Current User Details: " << std::endl;
                std::cout << "----------------------------" << std::endl;
                std::cout << "User  ID: " << user.getId() << std::endl;
                std::cout << "Username: " << user.getUsername() << std::endl;
                std::cout << "Role: " << user.getRole() << std::endl;
                std::cout << "----------------------------" << std::endl;

                std::cout << "Select information to update: " << std::endl;
                std::cout << "1. Username" << std::endl;
                std::cout << "2. Role" << std::endl;
                std::cout << "3. Password" << std::endl;
                std::cout << "4. Back to Manage Users" << std::endl;
                std::cout << "Enter Choice: ";
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (choice == 1)
                {
                    std::cout << "Enter new Username: ";
                    std::getline(std::cin, newUsername);
                    user.setUsername(newUsername);
                    updateUser (user.getId(), user);
                }
                else if (choice == 2)
                {
                    std::cout << "Enter new Role (Administrator/Booking Agent/Passenger): ";
                    std::getline(std::cin, newRole);
                    user.setRole(newRole);
                    updateUser (user.getId(), user); 
                }
                else if (choice == 3)
                {
                    while (true)
                    {
                        std::cout << "\nEnter new Password: ";
                        newPassword = Utils::getPassword(); 
                        std::cout << "\nConfirm Password: ";
                        confirmPass = Utils::getPassword(); 

                        if (newPassword == confirmPass)
                        {
                            user.setPassword(newPassword); // Set the new password
                            updateUser (user.getId(), user); 
                            break; // Exit confirmation loop
                        }
                        else
                        {
                            std::cout << "\nPasswords do not match. Try again." << std::endl;
                        }
                    }
                }
                else if (choice == 4)
                {
                    break; // Exit the update menu
                }
                else
                {
                    std::cout << "Invalid Choice. Please try again." << std::endl;
                }

                std::cout << "Do you want to update another field? (Y/N): ";
                char continueChoice;
                std::cin >> continueChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (continueChoice != 'Y' && continueChoice != 'y')
                {
                    break; // Exit the update loop
                }
            }
        }
    }

    if (!userFound)
    {
        std::cout << "User  wasn't found." << std::endl;
    }
}

void Administrator::deleteUserMenu()
{
    std::string userId;
    Utils::clearScreen();
    viewUsers();
    std::cout << "\nEnter User ID to Remove: ";
    std::getline(std::cin, userId);

    bool userFound = false;
    for (auto &user : users)
    {
        if (user.getId() == userId)
        {
            deleteUser(userId);
            userFound = true;
            break;
        }
    }

    if (!userFound)
    {
        std::cout << "User Not Found" << std::endl;
    }
}