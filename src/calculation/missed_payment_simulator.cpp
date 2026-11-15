#include "calculation/missed_payment_simulator.hpp"

#include "calculation/interest_calculator.hpp"
#include "calculation/tax_calculator.hpp"
#include "validation/input_validator.hpp"

namespace ccsim {

Result<MissedPaymentResult> MissedPaymentSimulator::calculate(
    const MissedPaymentInput& input) {
    if (const auto error = InputValidator::validate_period(input.period_days, input.day_count_basis)) {
        return Result<MissedPaymentResult>::failure(*error);
    }
    if (input.interest_bearing_principal.paise() > input.starting_balance.paise()) {
        return Result<MissedPaymentResult>::failure(
            "Interest-bearing principal cannot exceed the starting balance");
    }

    const auto interest = InterestCalculator::calculate(
        input.interest_bearing_principal,
        input.annual_interest_rate,
        input.period_days,
        input.day_count_basis);
    if (!interest.has_value()) {
        return Result<MissedPaymentResult>::failure(interest.error());
    }

    const auto gst_on_interest = TaxCalculator::calculate(interest.value(), input.gst_rate);
    if (!gst_on_interest.has_value()) {
        return Result<MissedPaymentResult>::failure(gst_on_interest.error());
    }

    const Money fee = input.late_fee_applicable
        ? input.late_fee
        : Money::from_paise(0).value();
    const auto gst_on_fee = TaxCalculator::calculate(fee, input.gst_rate);
    if (!gst_on_fee.has_value()) {
        return Result<MissedPaymentResult>::failure(gst_on_fee.error());
    }

    auto total = Money::add(interest.value(), gst_on_interest.value());
    if (!total.has_value()) {
        return Result<MissedPaymentResult>::failure(total.error());
    }
    total = Money::add(total.value(), fee);
    if (!total.has_value()) {
        return Result<MissedPaymentResult>::failure(total.error());
    }
    total = Money::add(total.value(), gst_on_fee.value());
    if (!total.has_value()) {
        return Result<MissedPaymentResult>::failure(total.error());
    }

    const auto final_balance = Money::add(input.starting_balance, total.value());
    if (!final_balance.has_value()) {
        return Result<MissedPaymentResult>::failure(final_balance.error());
    }

    return Result<MissedPaymentResult>::success(MissedPaymentResult{
        input.starting_balance,
        interest.value(),
        gst_on_interest.value(),
        fee,
        gst_on_fee.value(),
        total.value(),
        final_balance.value()});
}

}  // namespace ccsim
