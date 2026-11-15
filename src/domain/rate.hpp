#pragma once

#include "domain/result.hpp"

#include <cstdint>
#include <string_view>

namespace ccsim {

class Rate {
public:
    static constexpr std::int64_t kBasisPointsPerPercent = 100;
    static constexpr std::int64_t kMaximumBasisPoints = 1'000'000;

    static Result<Rate> from_basis_points(std::int64_t basis_points);
    static Result<Rate> parse_percent(std::string_view percentage);

    std::int64_t basis_points() const noexcept;

private:
    explicit Rate(std::int64_t basis_points) : basis_points_(basis_points) {}

    std::int64_t basis_points_;
};

}  // namespace ccsim
