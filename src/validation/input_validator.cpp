#include "validation/input_validator.hpp"

namespace ccsim {

std::optional<std::string> InputValidator::validate_payment(
    Money payment,
    Money statement_amount) {
    if (payment.paise() > statement_amount.paise()) {
        return "Payment amount needs to be no higher than the statement amount";
    }
    return std::nullopt;
}

std::optional<std::string> InputValidator::validate_period(
    std::uint32_t period_days,
    std::uint32_t day_count_basis) {
    if (period_days == 0) {
        return "Please enter a period of at least one day";
    }
    if (day_count_basis == 0) {
        return "Day-count basis must be greater than zero";
    }
    return std::nullopt;
}

}  // namespace ccsim
