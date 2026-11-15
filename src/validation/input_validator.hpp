#pragma once

#include "domain/money.hpp"

#include <cstdint>
#include <optional>
#include <string>

namespace ccsim {

class InputValidator {
public:
    static std::optional<std::string> validate_payment(Money payment, Money statement_amount);
    static std::optional<std::string> validate_period(
        std::uint32_t period_days,
        std::uint32_t day_count_basis);
};

}  // namespace ccsim
