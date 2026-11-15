#pragma once

#include "domain/money.hpp"
#include "domain/rate.hpp"

namespace ccsim {

class PercentageCalculator {
public:
    static Result<Money> calculate(Money amount, Rate percentage);
};

}  // namespace ccsim
