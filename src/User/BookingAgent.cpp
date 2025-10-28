#include "../../include/User/BookingAgent.hpp"

BookingAgent::BookingAgent(const std::string &id, const std::string &username, const std::string &password):
User(id, username, password, "Booking Agent") 
{
    loadReservationsFromJson("data/reservations.json");
}



void BookingAgent::loadReservationsFromJson(const std::string &filename)
{
    // Clear the existing reservations
    reservations.clear();

    try
    {
        nlohmann::json reservationsJson = JsonUtils::readJsonFromFile(filename);

        for (const auto &reservationJson : reservationsJson)
        {
            reservations.push_back(Reservation::fromJson(reservationJson));
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error loading reservations from file: " << e.what() << std::endl;
    }
}

void BookingAgent::saveReservationsToJson(const std::string &filename)
{
    nlohmann::json reservationsJson;

    // Save all reservations
    for (const auto &reservation : reservations)
    {
        reservationsJson.push_back(reservation.toJson());
    }

    // Use overwrite = true to replace the entire file
    JsonUtils::saveJsonToFile(reservationsJson, filename);
}


void BookingAgent::searchFlights(const std::string &origin, const std::string &destination, const std::string &departureDate) const
{
    flightService.searchFlights(origin,destination,departureDate);
    activityLogger.logActivity(id, "booking agent", "Searched Flights");
}


bool BookingAgent::bookFlight(Reservation &reservation, const std::string& paymentMethod, const std::optional<std::string>& paymentDetails)
{
    if (reservationService.bookFlight(reservation, paymentMethod, paymentDetails))
    {
        reservations.push_back(reservation);
        saveReservationsToJson("data/reservations.json");
        activityLogger.logActivity(id, "booking agent", "Booked Flight", "Reservation ID: " + reservation.getReservationId());
        return true;
    }
    return false;
}

void BookingAgent::updateReservation(const std::string &reservationId, const Reservation &updatedReservation)
{
    for (auto &reservation : reservations)
    {
        if (reservation.getReservationId() == reservationId)
        {
            // Update the reservation's data with the new data
            reservation = updatedReservation;
            saveReservationsToJson("data/reservations.json");
            activityLogger.logActivity(id, "booking agent", "Updated Reservation", "Reservation ID: " + reservation.getReservationId());
            return;
        }
    }
    // If reservation isn't found
    std::cout << "The reservation given doesn't exist" << std::endl;
}

void BookingAgent::cancelReservation(const std::string &reservationId)
{
    // Find the reservation in the vector
    auto it = std::find_if(reservations.begin(), reservations.end(),
                           [&reservationId](const Reservation &reservation)
                           {
                               return reservation.getReservationId() == reservationId;
                           });

    if (it != reservations.end()) // Reservation found
    {
        // Process refund if payment was made
        if (it->getPaymentStatus() == "Paid")
        {
            if (paymentService.processRefund(it->getPaymentMethod(), it->getPrice(), it->getPaymentDetails()))
            {
                std::cout << "Refund processed successfully." << std::endl;
            }
            else
            {
                std::cout << "Refund failed." << std::endl;
            }
        }
        else
        {
            std::cout << "No refund required." << std::endl;
        }

        // Change the seat back to available in seats.json
        std::string flightNumber = it->getFlightNumber();
        std::string seatNumber = it->getSeatNumber();

        // Read seats.json
        nlohmann::json seatsData;
        try {
            seatsData = JsonUtils::readJsonFromFile("data/seats.json");
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return;
        }

        // Check if the flight exists in seats.json
        if (seatsData.contains(flightNumber) && seatsData[flightNumber]["seats"].contains(seatNumber))
        {
            // Mark the seat as available
            seatsData[flightNumber]["seats"][seatNumber] = "available";
        }
        else
        {
            std::cout << "Flight or seat not found in seats.json." << std::endl;
        }

        // Update available seats in flights.json
        nlohmann::json flightsData;
        try {
            flightsData = JsonUtils::readJsonFromFile("data/flights.json");
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return;
        }

        // Find the flight in flights.json
        auto flightIt = std::find_if(flightsData.begin(), flightsData.end(), [&](const nlohmann::json &flight)
                                      { return flight["flightNumber"] == flightNumber; });

        if (flightIt != flightsData.end())
        {
            // Increment available seats
            int availableSeats = (*flightIt)["availableSeats"];
            (*flightIt)["availableSeats"] = availableSeats + 1; // Increment available seats
        }
        else
        {
            std::cout << "Flight " << flightNumber << " not found in flights.json." << std::endl;
        }

        // Save the updated seats.json
        try {
            JsonUtils::saveJsonToFile(seatsData, "data/seats.json");
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return;
        }

        // Save the updated flights.json
        try {
            JsonUtils::saveJsonToFile(flightsData, "data/flights.json");
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return;
        }

        // Remove the reservation from the vector
        reservations.erase(it);

        // Save the updated vector to the JSON file
        nlohmann::json reservationsJson;
        for (const auto &reservation : reservations)
        {
            reservationsJson.push_back(reservation.toJson());
        }
        JsonUtils::saveJsonToFile(reservationsJson, "data/reservations.json");

        // Log the activity
        activityLogger.logActivity(id, "booking agent", "Cancelled Reservation", "Reservation ID: " + reservationId);
        std::cout << "Reservation with ID " << reservationId << " cancelled successfully." << std::endl;
    }
    else // Reservation not found
    {
        std::cout << "Reservation with ID " << reservationId << " not found." << std::endl;
    }
}

bool BookingAgent::changeSeat(const std::string &flightNumber, const std::string &oldSeatNumber, const std::string &newSeatNumber)
{
    return flightService.changeSeat(flightNumber,oldSeatNumber,newSeatNumber);
}

const std::vector<Reservation>& BookingAgent::getReservations() const
{
    return reservations;
}

std::optional<std::reference_wrapper<Reservation>> BookingAgent::findReservation(const std::string &reservationId)
{
    auto reservation = reservationService.findReservation(reservationId);
    return reservation;
}

std::optional<std::reference_wrapper<Flight>> BookingAgent::findFlight(const std::string &flightNumber)
{
    auto flight = flightService.findFlight(flightNumber);
    return flight;
}

void BookingAgent::viewFlights() const
{
    flightService.displayFlights();
    activityLogger.logActivity(id, "booking agent", "Displayed Flights");
}




void BookingAgent::scanBoardingPass(const BoardingPass& boardingPass)
{
    // Verify the boarding pass (e.g., check if the passenger is on the flight)
    if (isValidBoardingPass(boardingPass))
    {
        scannedPasses.push_back(boardingPass);
        std::cout << "Boarding pass scanned for passenger " << boardingPass.getPassengerName() <<std::endl;
        activityLogger.logActivity(id, "booking agent", "Scanned Boarding Pass", "Reservation ID: " + boardingPass.getReservationId());
    }
    else
    {
        std::cout << "Invalid boarding pass "<<std::endl;
    }
}


bool BookingAgent::isValidBoardingPass(const BoardingPass& boardingPass) const
{
    for(const auto& reservation:reservations)
    {
        // Check the boarding pass from the reservations
        if(reservation.getReservationId() == boardingPass.getReservationId() && reservation.getFlightNumber() == boardingPass.getFlightNumber() )
        {
            return true;  
        }
    }
    return false;
    
}


void BookingAgent::bookingAgentMenu()
{
    int choice;
    while (true)
    {
        Utils::clearScreen();
        std::cout << "--- Booking Agent Menu ---" << std::endl;
        std::cout << "1. Search Flights" << std::endl;
        std::cout << "2. Book a Flight" << std::endl;
        std::cout << "3. Modify Reservation" << std::endl;
        std::cout << "4. Cancel Reservation" << std::endl;
        std::cout << "5. Logout" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice)
        {
        case 1:
            searchFlightsMenu();
            break;
        case 2:
            bookFlightMenu();
            break;
        case 3:
            modifyReservationMenu();
            break;
        case 4:
            cancelReservationMenu();
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            break;
        case 5:
            Utils::clearScreen();
            logout();
            return;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void BookingAgent::searchFlightsMenu()
{
    Utils::clearScreen();
    std::string origin, destination, departureDate;
    std::cout << "--- Search Flights ---" << std::endl;

    std::cout << "Enter Origin: " << std::endl;
    std::getline(std::cin, origin);
    std::cout << "Enter Destination: " << std::endl;
    std::getline(std::cin, destination);
    std::cout << "Enter Departure Date (YYYY-MM-DD): " << std::endl;
    std::getline(std::cin, departureDate);
    Utils::clearScreen();
    searchFlights(origin, destination, departureDate);
    std::cout << "Press any key to continue... " << std::endl;
    std::cin.get(); // Waits for a single character (e.g., Enter)
}

void BookingAgent::bookFlightMenu()
{
    Utils::clearScreen();
    std::string passengerId, passengerName, flightNumber, seatNumber, paymentMethod, gate, boardingTime;
    std::optional<std::string> paymentDetails = std::nullopt;

    viewFlights();
    std::cout << "\n\n--- Book a Flight ---" << std::endl;
    std::cout << "Enter Passenger ID: ";
    std::getline(std::cin, passengerId);
    std::cout << "Enter Passenger Name: ";
    std::getline(std::cin, passengerName);
    std::cout << "Enter Flight Number: ";
    std::getline(std::cin, flightNumber);
    auto flightOpt = findFlight(flightNumber);
    if (!flightOpt)
    {
        std::cout << "Flight not found!" << std::endl;
    }

    else
    {
        Flight &flight = flightOpt.value().get();
        Utils::clearScreen();
        displayAvailableSeats(flightNumber);
        std::cout << "Enter Seat Number (e.g., 12A): ";
        std::getline(std::cin, seatNumber);
        Utils::clearScreen();

        std::cout << "Enter Payment Method (Credit Card/Cash/PayPal): ";
        std::getline(std::cin, paymentMethod);

        if (paymentMethod == "Credit Card" || paymentMethod == "credit card" || paymentMethod == "Credit card" || paymentMethod == "PayPal" || paymentMethod == "Paypal" || paymentMethod == "paypal")
        {
            std::cout << "Enter Payment Details: ";
            std::string details;
            std::getline(std::cin, details);
            paymentDetails = details;
        }

        // Generate a unique reservation ID
        std::string reservationId = Utils::generateUniqueReservationId();
        std::string status = "Confirmed";
        std::cout << "\nEnter Gate Number: ";
        std::getline(std::cin, gate);
        std::cout << "Enter Boarding Time: ";
        std::getline(std::cin, boardingTime);

        // Create the Reservation object
        Reservation reservation(reservationId, passengerId, passengerName, flightNumber, seatNumber, gate, boardingTime, status, flight.getPrice());

        // Call the bookFlight function
        if (bookFlight(reservation, paymentMethod, paymentDetails))
        {
            auto flightOpt = findFlight(flightNumber);
            Flight &flight = flightOpt.value();
            std::cout << "Passenger: " << passengerName << std::endl;
            std::cout << "Flight: " << flightNumber << " from " << flight.getOrigin() << " to " << flight.getDestination() << std::endl;
            std::cout << "Seat: " << seatNumber << std::endl;
            std::cout << "Price: " << flight.getPrice() << std::endl;
            std::cout << "Payment Method: " << paymentMethod << std::endl;
        }
    }
    std::cout << "Press any key to continue... " << std::endl;
    std::cin.get(); // Waits for a single character (e.g., Enter)
}

void BookingAgent::modifyReservationMenu()
{
    Utils::clearScreen();
    std::cout << "--- Modify Reservation ---\n";
    std::cout << "Enter Reservation ID: ";
    std::string reservationId;
    std::getline(std::cin, reservationId);

    auto it = std::find_if(reservations.begin(), reservations.end(),
                           [reservationId](const Reservation &reservation)
                           {
                               return reservation.getReservationId() == reservationId;
                           });

    if (it == reservations.end())
    {
        std::cout << "Reservation not found.\n";
        return;
    }

    while (true)
    {
        Utils::clearScreen();
        reservationService.displayReservation(*it);
        std::cout << "1. Change Seat\n";
        std::cout << "2. Update Passenger Details\n";
        std::cout << "3. Update Reservation Status\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Enter choice: ";
        int modifyChoice;
        std::cin >> modifyChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (modifyChoice == 1)
        {
            Utils::clearScreen();
            displayAvailableSeats(it->getFlightNumber());
            std::cout << "Enter new seat number: ";
            std::string newSeat;
            std::getline(std::cin, newSeat);

            // Ensure seat is available
            if (!changeSeat(it->getFlightNumber(), it->getSeatNumber(), newSeat))
            {
                std::cout << "Seat is already booked. Please choose another " << std::endl;
            }
            else
            {
                it->setSeatNumber(newSeat);
                updateReservation(it->getReservationId(), *it);
                std::cout << "Seat changed successfully" << std::endl;
            }
        }
        else if (modifyChoice == 2)
        {
            Utils::clearScreen();
            std::cout << "Enter new passenger name: ";
            std::string newName;
            std::getline(std::cin, newName);
            it->setPassengerName(newName);
            updateReservation(it->getReservationId(), *it);
            std::cout << "Passenger details updated.\n";
        }
        else if (modifyChoice == 3)
        {
            Utils::clearScreen();
            std::cout << "Enter new Status: ";
            std::string newStatus;
            std::getline(std::cin, newStatus);
            it->setStatus(newStatus);
            updateReservation(it->getReservationId(), *it);
            std::cout << "Passenger details updated.\n";
        }
        else if (modifyChoice == 4)
        {
            break;
        }
    }
}

void BookingAgent::cancelReservationMenu()
{
    Utils::clearScreen();
    std::cout << "--- Cancel Reservation ---" << std::endl;
    std::cout << "Enter Reservation ID: ";
    std::string reservationId;
    std::getline(std::cin, reservationId);

    std::cout << "Are you sure you want to cancel this reservation ? (Y/N): ";
    char cancelChoice;
    std::cin >> cancelChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (cancelChoice == 'Y' || cancelChoice == 'y')
    {
        cancelReservation(reservationId);
    }
}