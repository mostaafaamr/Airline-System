#include "../../include/Booking/PaymentService.hpp"

bool PaymentService::processPayment(const std::string &paymentMethod, double amount,  const std::optional<std::string>& paymentDetails)
{

    if (validatePaymentDetails(paymentMethod, paymentDetails))
    {
        return true;
    }
    else
    {
        std::cout << "Payment failed. Invalid payment details." << std::endl;
        return false;
    }
}

bool PaymentService::processRefund(const std::string &paymentMethod, double amount,  const std::optional<std::string>& paymentDetails)
{
    if (validatePaymentDetails(paymentMethod, paymentDetails))
    {
        std::cout << "Processing refund of $" << amount << " to " << paymentMethod << "..." << std::endl;
        std::cout << "Refund successful!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Refund failed. Invalid payment details." << std::endl;
        return false;
    }
}


// Validate payment details
bool PaymentService::validatePaymentDetails(const std::string &paymentMethod, const std::optional<std::string>& paymentDetails)
{
    if (paymentMethod == "Credit Card")
    {
        // Ensure payment details exist and are exactly 16 digits
        return paymentDetails.has_value() && paymentDetails->size() == 16;
    }
    else if (paymentMethod == "PayPal")
    {
        // Ensure payment details exist and contain '@'
        return paymentDetails.has_value() && paymentDetails->find('@') != std::string::npos;
    }
    else if (paymentMethod == "Cash")
    {
        // Cash payments do not require payment details
        return true;
    }
    return false;
}