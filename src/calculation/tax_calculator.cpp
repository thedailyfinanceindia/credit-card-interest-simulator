#include "calculation/tax_calculator.hpp"
#include "calculation/percentage_calculator.hpp"

namespace ccsim {

Result<Money> TaxCalculator::calculate(Money taxable_amount, Rate tax_rate) {
    return PercentageCalculator::calculate(taxable_amount, tax_rate);
}

}  // namespace ccsim
