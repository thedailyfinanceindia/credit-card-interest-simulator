#pragma once

#include "domain/result.hpp"

#include <cstdint>
#include <string>
#include <string_view>

namespace ccsim {

class Money {
public:
    static Result<Money> from_paise(std::int64_t paise);
    static Result<Money> from_rupees(std::int64_t rupees);
    static Result<Money> parse(std::string_view rupees);

    std::int64_t paise() const noexcept;
    std::string to_string() const;

    static Result<Money> add(Money lhs, Money rhs);
    static Result<Money> subtract(Money lhs, Money rhs);

private:
    explicit Money(std::int64_t paise) : paise_(paise) {}

    std::int64_t paise_;
};

}  // namespace ccsim
