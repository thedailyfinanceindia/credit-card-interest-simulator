#include "calculation/main_scenario_calculator.hpp"

#include "calculation/interest_calculator.hpp"
#include "calculation/percentage_calculator.hpp"
#include "calculation/tax_calculator.hpp"
#include "validation/input_validator.hpp"

namespace ccsim {

Result<MainScenarioResult> MainScenarioCalculator::calculate(const MainScenarioInput& input) {
    if (const auto error = InputValidator::validate_period(input.period_days, input.day_count_basis)) {
        return Result<MainScenarioResult>::failure(*error);
    }

    Money payment = input.statement_amount;
    switch (input.payment_mode) {
        case PaymentMode::Minimum: {
            const auto rate = input.minimum_due_rate.basis_points();
            if (rate == 0 || rate > 10'000) {
                return Result<MainScenarioResult>::failure(
                    "Illustrative minimum-payment percentage must be greater than 0% and no more than 100%");
            }
            const auto calculated = PercentageCalculator::calculate(
                input.statement_amount,
                input.minimum_due_rate);
            if (!calculated.has_value()) {
                return Result<MainScenarioResult>::failure(calculated.error());
            }
            payment = calculated.value();
            break;
        }
        case PaymentMode::Full:
            break;
        case PaymentMode::Custom:
            if (!input.custom_payment.has_value()) {
                return Result<MainScenarioResult>::failure("Enter a custom payment amount");
            }
            payment = input.custom_payment.value();
            break;
    }

    if (const auto error = InputValidator::validate_payment(payment, input.statement_amount)) {
        return Result<MainScenarioResult>::failure(*error);
    }

    const auto remaining = Money::subtract(input.statement_amount, payment);
    if (!remaining.has_value()) {
        return Result<MainScenarioResult>::failure(remaining.error());
    }

    const auto interest = InterestCalculator::calculate(
        remaining.value(),
        input.annual_interest_rate,
        input.period_days,
        input.day_count_basis);
    if (!interest.has_value()) {
        return Result<MainScenarioResult>::failure(interest.error());
    }

    const auto gst = TaxCalculator::calculate(interest.value(), input.gst_rate);
    if (!gst.has_value()) {
        return Result<MainScenarioResult>::failure(gst.error());
    }

    auto next_balance = Money::add(remaining.value(), interest.value());
    if (!next_balance.has_value()) {
        return Result<MainScenarioResult>::failure(next_balance.error());
    }
    next_balance = Money::add(next_balance.value(), gst.value());
    if (!next_balance.has_value()) {
        return Result<MainScenarioResult>::failure(next_balance.error());
    }

    return Result<MainScenarioResult>::success(MainScenarioResult{
        input.statement_amount,
        payment,
        remaining.value(),
        interest.value(),
        gst.value(),
        next_balance.value()});
}

}  // namespace ccsim
