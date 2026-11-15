#include "domain/money.hpp"

#include <limits>

namespace ccsim {
namespace {

constexpr std::int64_t kPaisePerRupee = 100;

bool is_digit(char value) {
    return value >= '0' && value <= '9';
}

}  // namespace

Result<Money> Money::from_paise(std::int64_t paise) {
    if (paise < 0) {
        return Result<Money>::failure("Please enter zero or a positive amount");
    }
    return Result<Money>::success(Money(paise));
}

Result<Money> Money::from_rupees(std::int64_t rupees) {
    if (rupees < 0) {
        return Result<Money>::failure("Please enter zero or a positive amount");
    }
    if (rupees > std::numeric_limits<std::int64_t>::max() / kPaisePerRupee) {
        return Result<Money>::failure("Please enter a smaller amount");
    }
    return Result<Money>::success(Money(rupees * kPaisePerRupee));
}

Result<Money> Money::parse(std::string_view rupees) {
    if (rupees.empty()) {
        return Result<Money>::failure("Please enter an amount");
    }

    std::size_t decimal_position = rupees.find('.');
    std::string_view whole = rupees;
    std::string_view fraction;
    if (decimal_position != std::string_view::npos) {
        if (rupees.find('.', decimal_position + 1) != std::string_view::npos) {
            return Result<Money>::failure("Please use a single decimal point in the amount");
        }
        whole = rupees.substr(0, decimal_position);
        fraction = rupees.substr(decimal_position + 1);
        if (fraction.empty() || fraction.size() > 2) {
            return Result<Money>::failure("Please use up to two digits after the decimal point");
        }
    }

    if (whole.empty()) {
        return Result<Money>::failure("Please include the rupee amount before the decimal point");
    }
    for (char digit : whole) {
        if (!is_digit(digit)) {
            return Result<Money>::failure("Please use numbers only for the amount");
        }
    }
    for (char digit : fraction) {
        if (!is_digit(digit)) {
            return Result<Money>::failure("Please use numbers only for the amount");
        }
    }

    std::int64_t rupee_value = 0;
    for (char digit : whole) {
        const int numeric_digit = digit - '0';
        if (rupee_value > (std::numeric_limits<std::int64_t>::max() / kPaisePerRupee
                           - numeric_digit) / 10) {
            return Result<Money>::failure("Please enter a smaller amount");
        }
        rupee_value = rupee_value * 10 + numeric_digit;
    }

    int paise_value = 0;
    if (!fraction.empty()) {
        paise_value = (fraction[0] - '0') * 10;
        if (fraction.size() == 2) {
            paise_value += fraction[1] - '0';
        }
    }

    if (rupee_value > (std::numeric_limits<std::int64_t>::max() - paise_value)
                          / kPaisePerRupee) {
        return Result<Money>::failure("Please enter a smaller amount");
    }
    return Result<Money>::success(Money(rupee_value * kPaisePerRupee + paise_value));
}

std::int64_t Money::paise() const noexcept {
    return paise_;
}

std::string Money::to_string() const {
    const auto rupees = paise_ / kPaisePerRupee;
    const auto paise = paise_ % kPaisePerRupee;
    std::string fraction = paise < 10 ? "0" + std::to_string(paise) : std::to_string(paise);
    return std::to_string(rupees) + "." + fraction;
}

Result<Money> Money::add(Money lhs, Money rhs) {
    if (lhs.paise_ > std::numeric_limits<std::int64_t>::max() - rhs.paise_) {
        return Result<Money>::failure("Money arithmetic overflow");
    }
    return Result<Money>::success(Money(lhs.paise_ + rhs.paise_));
}

Result<Money> Money::subtract(Money lhs, Money rhs) {
    if (rhs.paise_ > lhs.paise_) {
        return Result<Money>::failure("Money amount cannot become negative");
    }
    return Result<Money>::success(Money(lhs.paise_ - rhs.paise_));
}

}  // namespace ccsim
