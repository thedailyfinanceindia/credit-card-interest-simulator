#pragma once

#include "domain/money.hpp"
#include "domain/rate.hpp"

#include <cstdint>
#include <optional>

namespace ccsim {

enum class PaymentMode {
    Minimum,
    Full,
    Custom
};

struct MainScenarioInput {
    Money statement_amount;
    Rate annual_interest_rate;
    PaymentMode payment_mode;
    std::optional<Money> custom_payment;
    Rate minimum_due_rate;
    std::uint32_t period_days;
    std::uint32_t day_count_basis;
    Rate gst_rate;
};

struct MainScenarioResult {
    Money statement_amount;
    Money payment_amount;
    Money remaining_principal;
    Money interest;
    Money gst_on_interest;
    Money estimated_next_balance;
};

class MainScenarioCalculator {
public:
    static Result<MainScenarioResult> calculate(const MainScenarioInput& input);
};

}  // namespace ccsim
