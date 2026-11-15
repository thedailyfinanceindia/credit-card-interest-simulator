#include "calculation/interest_calculator.hpp"

#include <cstdint>
#include <limits>

namespace ccsim {
namespace {

constexpr std::uint64_t kBasisPointsPerPercent = 10'000;

bool checked_multiply(std::uint64_t lhs, std::uint64_t rhs, std::uint64_t& result) {
    if (rhs != 0 && lhs > std::numeric_limits<std::uint64_t>::max() / rhs) {
        return false;
    }
    result = lhs * rhs;
    return true;
}

}  // namespace

Result<Money> InterestCalculator::calculate(
    Money principal,
    Rate annual_rate,
    std::uint32_t period_days,
    std::uint32_t day_count_basis) {
    if (period_days == 0) {
        return Result<Money>::failure("Calculation period must be greater than zero days");
    }
    if (day_count_basis == 0) {
        return Result<Money>::failure("Day-count basis must be greater than zero");
    }

    const auto rate = static_cast<std::uint64_t>(annual_rate.basis_points());
    const auto days = static_cast<std::uint64_t>(period_days);
    const auto paise = static_cast<std::uint64_t>(principal.paise());
    const auto basis = static_cast<std::uint64_t>(day_count_basis);

    std::uint64_t numerator = 0;
    if (!checked_multiply(paise, rate, numerator)
        || !checked_multiply(numerator, days, numerator)) {
        return Result<Money>::failure("Interest calculation overflow");
    }

    std::uint64_t denominator = 0;
    if (!checked_multiply(kBasisPointsPerPercent, basis, denominator)) {
        return Result<Money>::failure("Interest calculation overflow");
    }

    std::uint64_t rounded_paise = numerator / denominator;
    const std::uint64_t remainder = numerator % denominator;
    if (remainder >= denominator / 2 + denominator % 2) {
        ++rounded_paise;
    }
    if (rounded_paise > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) {
        return Result<Money>::failure("Interest result is too large");
    }
    return Money::from_paise(static_cast<std::int64_t>(rounded_paise));
}

}  // namespace ccsim
