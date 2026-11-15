#include "calculation/percentage_calculator.hpp"

#include <cstdint>
#include <limits>

namespace ccsim {
namespace {

constexpr std::uint64_t kBasisPointsPerPercent = 10'000;

}  // namespace

Result<Money> PercentageCalculator::calculate(Money amount, Rate percentage) {
    const auto paise = static_cast<std::uint64_t>(amount.paise());
    const auto rate = static_cast<std::uint64_t>(percentage.basis_points());
    if (rate != 0 && paise > std::numeric_limits<std::uint64_t>::max() / rate) {
        return Result<Money>::failure("Percentage calculation overflow");
    }

    const std::uint64_t numerator = paise * rate;
    std::uint64_t rounded_paise = numerator / kBasisPointsPerPercent;
    if (numerator % kBasisPointsPerPercent >= kBasisPointsPerPercent / 2) {
        ++rounded_paise;
    }
    if (rounded_paise > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) {
        return Result<Money>::failure("Percentage result is too large");
    }
    return Money::from_paise(static_cast<std::int64_t>(rounded_paise));
}

}  // namespace ccsim
