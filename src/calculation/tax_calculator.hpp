#pragma once

#include "domain/money.hpp"
#include "domain/rate.hpp"

namespace ccsim {

class TaxCalculator {
public:
    static Result<Money> calculate(Money taxable_amount, Rate tax_rate);
};

}  // namespace ccsim
