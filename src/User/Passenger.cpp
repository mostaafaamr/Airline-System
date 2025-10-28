#include "../../include/User/Passenger.hpp"

Passenger::Passenger(const std::string& id, const std::string& username, const std::string& password):
User(id, username, password, "Passenger"), loyaltyPoints(0) {}


void Passenger::searchFlights(const std::string &origin, const std::string &destination, const std::string &departureDate) const
{
    flightService.searchFlights(origin,destination,departureDate);
    activityLogger.logActivity(id, "passenger", "Searched Flights");
}


void Passenger::viewReservations() const
{
    reservationService.displayReservations(this->getId());
    activityLogger.logActivity(id, "passenger", "Viewed Reservations");
}

bool Passenger::bookFlight(Reservation &reservation, const std::string& paymentMethod, const std::optional<std::string>& paymentDetails)
{
    if(reservationService.bookFlight(reservation, paymentMethod, paymentDetails))
    {
        travelHistory.push_back(reservation);
        activityLogger.logActivity(id, "Passenger", "Booked Flight", "Reservation ID: " + reservation.getReservationId());
        loyaltyPoints += static_cast<int>(reservation.getPrice()); // 1 point per dollar;
        return true;
    }
    return false;
}

std::optional<std::reference_wrapper<Reservation>> Passenger::findReservation(const std::string &reservationId)
{
    auto reservation = reservationService.findReservation(reservationId);
    return reservation;
}

std::optional<std::reference_wrapper<Flight>> Passenger::findFlight(const std::string &flightNumber)
{
    auto flight = flightService.findFlight(flightNumber);
    return flight;
}




void Passenger::redeemPoints(int points)
{
    if (points > loyaltyPoints)
    {
        std::cout << "Not enough points to redeem"<<std::endl;
        return;
    }
    loyaltyPoints -= points;
    std::cout << "Redeemed " << points << " points, Remaining points: "<<loyaltyPoints<<std::endl;
}

void Passenger::checkIn() const 
{
    std::string reservationId;
    std::cout << "Enter Reservation ID: ";
    std::getline(std::cin,reservationId);

    // Try to find the reservation
    auto reservationOpt = reservationService.findReservation(reservationId);
    if (!reservationOpt) // Check if the optional contains a value
    {
        std::cout << "Reservation not found" << std::endl;
        return;
    }

    // Extract the reference to the actual reservation object
    Reservation &reservation = reservationOpt.value().get();

    if (reservation.getPassengerId() != getId())
    {
        std::cout<<"This reservation doesn't belong to you"<<std::endl;
        return;
    }

    if (reservation.getStatus() == "Checked-In")
    {
        std::cout<<"You are already checked in"<<std::endl;
        return;
    }
     
    auto flightOpt  = flightService.findFlight(reservation.getFlightNumber());
    if (!flightOpt) // Check if flight was found
    {
        std::cout << "Flight not found" << std::endl;
        return;
    }
    
    Flight &flight = flightOpt.value().get();

    // Generate boarding pass
    BoardingPass boardingPass(reservation.getReservationId(), reservation.getPassengerName(), reservation.getFlightNumber(),
                              flight.getOrigin(), flight.getDestination(), flight.getDepartureDateAndTime(),
                              reservation.getSeatNumber(), reservation.getGate(), reservation.getBoardingTime());
    boardingPass.display();

    // Update reservation status
    reservationService.updateReservationStatus(reservationId, "Checked-In");;
    std::cout<<"Check-in successful!"<<std::endl;
    activityLogger.logActivity(id, "passenger", "Checked-In");
    
}


void Passenger::passengerMenu()
{
    int choice;
    while (true)
    {
        Utils::clearScreen();
        std::cout << "--- Passenger Menu ---" << std::endl;
        std::cout << "1. Search Flights" << std::endl;
        std::cout << "2. View my Reservations" << std::endl;
        std::cout << "3. Check In" << std::endl;
        std::cout << "4. Logout" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice)
        {
        case 1: // Search Flights
            searchFlightsMenu();
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            break;
        case 2: // View my Reservations
            Utils::clearScreen();
            std::cout<<"--- View My Reservations ---"<<std::endl;
            viewReservations();
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            break;
        case 3: // Check In
            Utils::clearScreen();
            std::cout << "--- Check In ---" << std::endl;
            checkIn();
            std::cout << "Press any key to continue... " << std::endl;
            std::cin.get(); // Waits for a single character (e.g., Enter)
            break;
        case 4: // Log out
            logout();
            Utils::clearScreen();
            return;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}


void Passenger::searchFlightsMenu()
{
    Utils::clearScreen();
    std::string origin, destination, departureDate;
    std::cout << "--- Search Flights ---" << std::endl;

    std::cout << "Enter Origin: " ;
    std::getline(std::cin, origin);
    std::cout << "Enter Destination: " ;
    std::getline(std::cin, destination);
    std::cout << "Enter Departure Date (YYYY-MM-DD): " ;
    std::getline(std::cin, departureDate);
    Utils::clearScreen();
    searchFlights(origin, destination, departureDate);

    std::string flightNumber;
    std::cout << "Enter the Flight Number you wish to book (or '0' to cancel): ";
    std::getline(std::cin, flightNumber);
    if (flightNumber != "0")
    {
        auto flightOpt = findFlight(flightNumber);
        if (!flightOpt)
        {
            std::cout << "Flight not found " << std::endl;
        }
        else
        {
            std::string seatNumber, paymentMethod, paymentDetails, passengerName;

            std::cout << "Enter Passenger Name: ";
            std::getline(std::cin, passengerName);
            displayAvailableSeats(flightNumber);

            std::cout << "Enter Seat Number: ";
            std::getline(std::cin, seatNumber);

            Utils::clearScreen();
            std::cout << "Enter Payment Method (Credit Card/Cash/PayPal): ";
            std::getline(std::cin, paymentMethod);

            std::optional<std::string> paymentDetailsOpt;
            if (paymentMethod == "Credit Card" || paymentMethod == "credit card" || paymentMethod == "Credit card" || paymentMethod == "PayPal" || paymentMethod == "Paypal" || paymentMethod == "paypal")
            {
                std::cout << "Enter Payment Details: ";
                std::getline(std::cin, paymentDetails);
                paymentDetailsOpt = paymentDetails;
            }
            Flight &flight = flightOpt.value().get();
            std::string reservationId = Utils::generateUniqueReservationId();

            Reservation newReservation(reservationId, getId(), passengerName, flightNumber, seatNumber, "A12", "8:00", "Pending", flight.getPrice());
            if (bookFlight(newReservation, paymentMethod, paymentDetailsOpt))
            {
                std::cout << "Booking successful!" << std::endl;
            }
            
        }
    }
}