#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "include/User/User.hpp"
#include "include/User/Administrator.hpp"
#include "include/User/BookingAgent.hpp"
#include "include/User/Passenger.hpp"
#include "include/Utils/Utils.hpp"
#include <conio.h>


void showMenu()
{
    std::cout << "Welcome to Airline Reservation and Management System" << std::endl;
    std::cout << "Please select your role:" << std::endl;
    std::cout << "1. Administrator" << std::endl;
    std::cout << "2. Booking Agent" << std::endl;
    std::cout << "3. Passenger" << std::endl;
    std::cout << "Enter choice: ";
}

int main()
{
    while (true)
    {
        Utils::clearScreen();
        showMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        Utils::clearScreen();

        std::string username;
        std::string password, role;

        if (choice == 1)
        {
            std::cout << "\n--- Administrator Login ---" << std::endl;
            std::cout << "Username: ";
            std::getline(std::cin, username);
            std::cout << std::endl
                      << "Password: ";
            role = "Administrator";
            password = Utils::getPassword();
            auto [success, userId] = User::login(username, password, role);
            if (success)
            {
                std::cout << std::endl<< "Login successful!" << std::endl;
                Administrator admin(userId,username,password);
                admin.adminMenu();
            }
            else
            {
                std::cout << "\nInvalid username/password" << std::endl;
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
            }
        }
        else if (choice == 2)
        {
            std::cout << "\n--- Booking Agent Login ---" << std::endl;
            std::cout << "Username: ";
            std::getline(std::cin, username);
            std::cout << std::endl
                      << "Password: ";
            role = "Booking Agent";          
            password = Utils::getPassword();
            auto [success, userId] = User::login(username, password, role);
            if (success)
            {
                std::cout << std::endl<< "Login successful!" << std::endl;
                BookingAgent agent(userId,username,password);
                agent.bookingAgentMenu();
            }
            else
            {
                std::cout << "\nInvalid username/password" << std::endl;
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
            }
        }
        else if (choice == 3)
        {
            std::cout << "\n--- Passenger Login ---" << std::endl;
            std::cout << "Username: ";
            std::getline(std::cin, username);
            std::cout << std::endl
                      << "Password: ";
            role = "Passenger";
            password = Utils::getPassword();
            auto [success, userId] = User::login(username, password, role);
            if (success)
            {
                std::cout << std::endl<< "Login successful!" << std::endl;
                Passenger passenger(userId,username,password);
                passenger.passengerMenu();
            }
            else
            {
                std::cout << "\nInvalid username/password" << std::endl;
                std::cout << "Press any key to continue... " << std::endl;
                std::cin.get(); // Waits for a single character (e.g., Enter)
            }
        }
        else if (choice == 4)
        {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        else
        {
            std::cout << "Invalid choice. Please try again" << std::endl;
        }
    }

    return 0;
}




