#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <limits>
#include <string>
#include <ctime>
#include <conio.h>
#include <sstream>
#include <iomanip>
#include <random>

namespace Utils
{
    // Clears the console screen
    inline void clearScreen()
    {
        std::cout << "\033[2J\033[H" << std::flush;
    }

    // Ignore leftover input in std::cin (useful after std::cin >> someVariable)
    inline void ignoreInput()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Convert string to uppercase
    inline std::string toUpperCase(const std::string &str)
    {
        std::string result = str;
        for (char &c : result)
        {
            c = std::toupper(c);
        }
        return result;
    }

    // Convert string to lowercase
    inline std::string toLowerCase(const std::string &str)
    {
        std::string lowerStr = str; // Create a copy of the input string
        for (char &c : lowerStr)    // Iterate over each character
        {
            c = std::tolower(static_cast<unsigned char>(c)); // Convert to lowercase
        }
        return lowerStr;
    }

    std::string trim(const std::string& str);
    std::string getCurrentTimestamp();
    std::string getPassword();
    std::string generateUniqueReservationId();
}

#endif // UTILS_HPP
