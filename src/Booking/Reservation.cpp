#include "../../include/Booking/Reservation.hpp"

Reservation::Reservation(const std::string& reservationId, const std::string& passengerId,
    const std::string &passengerName,const std::string& flightNumber, const std::string& seatNumber,
    const std::string &gate,const std::string &boardingTime,
    const std::string& status, double price)
: reservationId(reservationId), passengerId(passengerId), passengerName(passengerName), flightNumber(flightNumber),
seatNumber(seatNumber), gate(gate), boardingTime(boardingTime), status(status), price(price) {}


Reservation Reservation::fromJson(const nlohmann::json& j)
{
return Reservation(
j.at("reservationId").get<std::string>(),
j.at("passengerId").get<std::string>(),
j.at("passengerName").get<std::string>(),
j.at("flightNumber").get<std::string>(),
j.at("seatNumber").get<std::string>(),
j.at("gate").get<std::string>(),
j.at("boardingTime").get<std::string>(),
j.at("status").get<std::string>(),
j.at("price").get<double>()
);
}


nlohmann::json Reservation::toJson() const
{
return nlohmann::json{
{"reservationId", reservationId},
{"passengerId", passengerId},
{"passengerName", passengerName},
{"flightNumber", flightNumber},
{"seatNumber", seatNumber},
{"gate", gate},
{"boardingTime", boardingTime},
{"status", status},
{"price", price}
};
}