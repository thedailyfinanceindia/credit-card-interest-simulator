#pragma once

#include "domain/money.hpp"
#include "domain/rate.hpp"

#include <cstdint>

namespace ccsim {

class InterestCalculator {
public:
    static Result<Money> calculate(
        Money principal,
        Rate annual_rate,
        std::uint32_t period_days,
        std::uint32_t day_count_basis);
};

}  // namespace ccsim
