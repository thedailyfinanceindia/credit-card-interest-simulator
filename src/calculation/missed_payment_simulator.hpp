#pragma once

#include "domain/money.hpp"
#include "domain/rate.hpp"

#include <cstdint>

namespace ccsim {

struct MissedPaymentInput {
    Money starting_balance;
    Money interest_bearing_principal;
    Rate annual_interest_rate;
    std::uint32_t period_days;
    std::uint32_t day_count_basis;
    Money late_fee;
    Rate gst_rate;
    bool late_fee_applicable;
};

struct MissedPaymentResult {
    Money starting_balance;
    Money interest;
    Money gst_on_interest;
    Money late_fee;
    Money gst_on_late_fee;
    Money total_additional_charges;
    Money estimated_new_balance;
};

class MissedPaymentSimulator {
public:
    static Result<MissedPaymentResult> calculate(const MissedPaymentInput& input);
};

}  // namespace ccsim
