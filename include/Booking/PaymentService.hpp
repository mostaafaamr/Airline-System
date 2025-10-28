#ifndef PAYMENTSERVICE_HPP
#define PAYMENTSERVICE_HPP

#include <iostream>
#include <string>
#include <optional>

class PaymentService
{
public:
    // Process a payment
    bool processPayment(const std::string &paymentMethod, double amount,  const std::optional<std::string>& paymentDetails = std::nullopt);   

    // Process a refund
    bool processRefund(const std::string &paymentMethod, double amount, const std::optional<std::string>& paymentDetails = std::nullopt);
    
private:
    // Validate payment details (simplified for demonstration)
    bool validatePaymentDetails(const std::string &paymentMethod, const std::optional<std::string>& paymentDetails = std::nullopt);
    
};

#endif