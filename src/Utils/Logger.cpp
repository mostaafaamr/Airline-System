#include "../../include/Utils/Logger.hpp"


void ActivityLogger::logActivity(const std::string& userId, const std::string& role, const std::string& action, const std::string& details)
{
    // Get the current timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);
    std::ostringstream timestamp;
    timestamp << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");

    // Create the activity log entry
    nlohmann::json activity = {
        {"userId", userId},
        {"role", role},
        {"action", action},
        {"timestamp", timestamp.str()},
        {"details", details}
    };

    // Read the existing log file
    nlohmann::json log;
    std::ifstream logFile("data/reports/user_activity.json");
    if (logFile.good())
    {
        logFile >> log;
    }
    logFile.close();

    // Append the new activity
    log.push_back(activity);

    // Write the updated log back to the file
    std::ofstream outFile("data/reports/user_activity.json");
    outFile << log.dump(4); // Pretty-print with 4 spaces
    outFile.close();
}