#include "domain/rate.hpp"

#include "domain/money.hpp"

namespace ccsim {

Result<Rate> Rate::from_basis_points(std::int64_t basis_points) {
    if (basis_points < 0 || basis_points > kMaximumBasisPoints) {
        return Result<Rate>::failure("Please enter a rate between 0% and 10,000%");
    }
    return Result<Rate>::success(Rate(basis_points));
}

Result<Rate> Rate::parse_percent(std::string_view percentage) {
    const auto parsed = Money::parse(percentage);
    if (!parsed.has_value()) {
        return Result<Rate>::failure(parsed.error());
    }
    return from_basis_points(parsed.value().paise());
}

std::int64_t Rate::basis_points() const noexcept {
    return basis_points_;
}

}  // namespace ccsim
