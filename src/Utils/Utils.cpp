#include "../../include/Utils/Utils.hpp"

std::string Utils::getPassword()
{
    std::string password;
    char ch;
    while ((ch = _getch()) != '\r') // '\r' is the Enter key
    {
        if (ch == '\b') // Handle backspace
        {
            if (!password.empty())
            {
                password.pop_back();
                std::cout << "\b \b"; // Move cursor back, overwrite with space, move back again
            }
        }
        else
        {
            password.push_back(ch);
            std::cout << '*'; // Display '*' instead of the actual character
        }
    }
    std::cout << "\n"; // Move to the next line after Enter is pressed
    return password;
}

std::string Utils::generateUniqueReservationId() 
{
    static std::random_device rd;  // True random number generator
    static std::mt19937 gen(rd()); // Mersenne Twister engine
    static std::uniform_int_distribution<> dis(1000, 9999); // 4-digit IDs

    return "R" + std::to_string(dis(gen));
}

std::string Utils::getCurrentTimestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm localTime;

    // Get local time safely (cross-platform)
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);

    return std::string(buffer);
}

std::string Utils::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}