#include "calculation/interest_calculator.hpp"
#include "calculation/main_scenario_calculator.hpp"
#include "calculation/missed_payment_simulator.hpp"
#include "calculation/tax_calculator.hpp"
#include "app/wasm_api.hpp"
#include "domain/money.hpp"
#include "domain/rate.hpp"
#include "validation/input_validator.hpp"

#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using ccsim::Money;
using ccsim::Rate;

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

Money money(const std::string& amount) {
    const auto parsed = Money::parse(amount);
    require(parsed.has_value(), "Expected valid money string " + amount);
    return parsed.value();
}

Rate rate(std::int64_t basis_points) {
    const auto parsed = Rate::from_basis_points(basis_points);
    require(parsed.has_value(), "Expected valid rate");
    return parsed.value();
}

ccsim::MissedPaymentInput golden_input(Money fee = money("750.00")) {
    return {
        money("99146.17"),
        money("99146.17"),
        rate(4500),
        30,
        365,
        fee,
        rate(1800),
        true};
}

ccsim::MainScenarioInput main_input(ccsim::PaymentMode mode) {
    return {
        money("100000.00"),
        rate(4500),
        mode,
        std::nullopt,
        rate(500),
        45,
        365,
        rate(1800)};
}

void test_money_parsing_and_formatting() {
    require(money("0").to_string() == "0.00", "Zero formatting");
    require(money("123.4").to_string() == "123.40", "One decimal formatting");
    require(money("99146.17").to_string() == "99146.17", "Two decimal formatting");

    require(!Money::parse("").has_value(), "Empty amount must be rejected");
    require(!Money::parse("-1.00").has_value(), "Negative amount must be rejected");
    require(!Money::parse("1.234").has_value(), "Excess precision must be rejected");
    require(!Money::parse("1.2.3").has_value(), "Multiple decimal points must be rejected");
    require(!Money::parse("1,000").has_value(), "Non-digit separators must be rejected");
    require(!Money::from_paise(-1).has_value(), "Negative paise must be rejected");
}

void test_money_overflow() {
    require(!Money::parse("99999999999999999.99").has_value(), "Unrepresentable amount rejected");
    const auto largest = Money::from_paise(std::numeric_limits<std::int64_t>::max());
    require(largest.has_value(), "Maximum paise should be representable");
    require(!Money::add(largest.value(), money("0.01")).has_value(), "Addition overflow rejected");
    require(!Money::subtract(money("1.00"), money("1.01")).has_value(), "Negative result rejected");
}

void test_interest_and_half_up_rounding() {
    const auto golden = ccsim::InterestCalculator::calculate(money("99146.17"), rate(4500), 30, 365);
    require(golden.has_value() && golden.value().to_string() == "3667.05", "Golden interest");

    const auto half_up = ccsim::InterestCalculator::calculate(money("0.01"), rate(5000), 1, 1);
    require(half_up.has_value() && half_up.value().to_string() == "0.01", "Half-paise rounds up");

    const auto zero_rate = ccsim::InterestCalculator::calculate(money("100.00"), rate(0), 30, 365);
    require(zero_rate.has_value() && zero_rate.value().paise() == 0, "Zero rate interest");

    require(!ccsim::InterestCalculator::calculate(money("1.00"), rate(100), 0, 365).has_value(),
            "Zero-day period rejected");
    require(!ccsim::InterestCalculator::calculate(money("1.00"), rate(100), 30, 0).has_value(),
            "Zero day-count basis rejected");

    const auto maximum = Money::from_paise(std::numeric_limits<std::int64_t>::max());
    require(maximum.has_value(), "Maximum money");
    require(!ccsim::InterestCalculator::calculate(maximum.value(), rate(10000), 365, 365).has_value(),
            "Interest intermediate overflow reported");
}

void test_tax_rounding() {
    const auto tax = ccsim::TaxCalculator::calculate(money("3667.05"), rate(1800));
    require(tax.has_value() && tax.value().to_string() == "660.07", "Golden GST rounding");

    const auto half_up = ccsim::TaxCalculator::calculate(money("0.01"), rate(5000));
    require(half_up.has_value() && half_up.value().to_string() == "0.01", "GST half-up rounding");

    const auto zero_tax = ccsim::TaxCalculator::calculate(money("0.00"), rate(1800));
    require(zero_tax.has_value() && zero_tax.value().paise() == 0, "Zero taxable amount");
}

void test_missed_payment_golden_scenario() {
    const auto result = ccsim::MissedPaymentSimulator::calculate(golden_input());
    require(result.has_value(), "Golden missed-payment calculation succeeds");
    require(result.value().interest.to_string() == "3667.05", "Golden interest");
    require(result.value().gst_on_interest.to_string() == "660.07", "Golden GST on interest");
    require(result.value().late_fee.to_string() == "750.00", "Golden late fee");
    require(result.value().gst_on_late_fee.to_string() == "135.00", "Golden GST on fee");
    require(result.value().total_additional_charges.to_string() == "5212.12", "Golden charges");
    require(result.value().estimated_new_balance.to_string() == "104358.29", "Golden final balance");
}

void test_main_minimum_full_and_custom_payments() {
    auto minimum = ccsim::MainScenarioCalculator::calculate(main_input(ccsim::PaymentMode::Minimum));
    require(minimum.has_value(), "Minimum scenario succeeds");
    require(minimum.value().payment_amount.to_string() == "5000.00", "Minimum payment uses 5% assumption");
    require(minimum.value().remaining_principal.to_string() == "95000.00", "Minimum remaining principal");
    require(minimum.value().interest.to_string() == "5270.55", "45-day main interest");
    require(minimum.value().gst_on_interest.to_string() == "948.70", "45-day main GST");
    require(minimum.value().estimated_next_balance.to_string() == "101219.25", "45-day main estimated balance");

    auto full = ccsim::MainScenarioCalculator::calculate(main_input(ccsim::PaymentMode::Full));
    require(full.has_value(), "Full scenario succeeds");
    require(full.value().payment_amount.to_string() == "100000.00", "Full payment amount");
    require(full.value().interest.to_string() == "0.00", "Full payment has no new interest");
    require(full.value().gst_on_interest.to_string() == "0.00", "Full payment has no new GST");
    require(full.value().estimated_next_balance.to_string() == "0.00", "Full payment has no new modeled charges");

    auto custom_input = main_input(ccsim::PaymentMode::Custom);
    custom_input.custom_payment = money("25000.00");
    const auto custom = ccsim::MainScenarioCalculator::calculate(custom_input);
    require(custom.has_value(), "Custom scenario succeeds");
    require(custom.value().payment_amount.to_string() == "25000.00", "Custom payment amount");
    require(custom.value().remaining_principal.to_string() == "75000.00", "Custom remaining principal");
}

void test_main_interest_periods() {
    auto input = main_input(ccsim::PaymentMode::Minimum);
    require(input.period_days == 45, "Main test input defaults to 45 days");

    input.period_days = 30;
    const auto thirty_day = ccsim::MainScenarioCalculator::calculate(input);
    require(thirty_day.has_value(), "30-day main calculation succeeds");
    require(thirty_day.value().interest.to_string() == "3513.70", "30-day interest");
    require(thirty_day.value().gst_on_interest.to_string() == "632.47", "30-day GST");
    require(thirty_day.value().estimated_next_balance.to_string() == "99146.17", "30-day next balance");

    input.period_days = 60;
    const auto custom_period = ccsim::MainScenarioCalculator::calculate(input);
    require(custom_period.has_value(), "Custom interest period calculation succeeds");
    require(custom_period.value().interest.to_string() == "7027.40", "60-day interest");
    require(custom_period.value().gst_on_interest.to_string() == "1264.93", "60-day GST");
    require(custom_period.value().estimated_next_balance.to_string() == "103292.33",
            "60-day next balance");

    input.period_days = 0;
    require(!ccsim::MainScenarioCalculator::calculate(input).has_value(), "Zero-day main period rejected");
}

void test_integrated_minimum_and_missed_payment_regression() {
    const auto main = ccsim::MainScenarioCalculator::calculate(main_input(ccsim::PaymentMode::Minimum));
    require(main.has_value(), "Integrated main scenario succeeds");
    require(main.value().payment_amount.to_string() == "5000.00", "Integrated payment");
    require(main.value().remaining_principal.to_string() == "95000.00", "Integrated remaining principal");
    require(main.value().interest.to_string() == "5270.55", "Integrated main interest");
    require(main.value().gst_on_interest.to_string() == "948.70", "Integrated main GST");
    require(main.value().estimated_next_balance.to_string() == "101219.25", "Integrated main next balance");

    const auto missed = ccsim::MissedPaymentSimulator::calculate({
        main.value().estimated_next_balance,
        main.value().remaining_principal,
        rate(4500),
        45,
        365,
        money("750.00"),
        rate(1800),
        true});
    require(missed.has_value(), "Integrated missed-payment scenario succeeds");
    require(missed.value().starting_balance.to_string() == "101219.25", "Integrated starting balance");
    require(missed.value().interest.to_string() == "5270.55", "Integrated additional interest");
    require(missed.value().gst_on_interest.to_string() == "948.70", "Integrated GST on interest");
    require(missed.value().late_fee.to_string() == "750.00", "Integrated late fee");
    require(missed.value().gst_on_late_fee.to_string() == "135.00", "Integrated GST on late fee");
    require(missed.value().total_additional_charges.to_string() == "7104.25",
            "Integrated total additional charges");
    require(missed.value().estimated_new_balance.to_string() == "108323.50",
            "Integrated final estimated balance");
}

void test_main_boundaries_and_errors() {
    auto input = main_input(ccsim::PaymentMode::Full);
    input.statement_amount = money("0.00");
    const auto zero_balance = ccsim::MainScenarioCalculator::calculate(input);
    require(zero_balance.has_value(), "Zero balance succeeds");
    require(zero_balance.value().estimated_next_balance.to_string() == "0.00", "Zero result");

    input = main_input(ccsim::PaymentMode::Custom);
    require(!ccsim::MainScenarioCalculator::calculate(input).has_value(), "Missing custom payment rejected");
    input.custom_payment = money("100000.01");
    require(!ccsim::MainScenarioCalculator::calculate(input).has_value(), "Payment above balance rejected");

    input = main_input(ccsim::PaymentMode::Minimum);
    input.minimum_due_rate = rate(0);
    require(!ccsim::MainScenarioCalculator::calculate(input).has_value(), "Zero MAD rate rejected");

    input.minimum_due_rate = rate(10000);
    const auto full_mad = ccsim::MainScenarioCalculator::calculate(input);
    require(full_mad.has_value() && full_mad.value().payment_amount.to_string() == "100000.00",
            "MAD percentage cannot exceed statement amount");

    input = main_input(ccsim::PaymentMode::Full);
    input.annual_interest_rate = rate(0);
    input.payment_mode = ccsim::PaymentMode::Custom;
    input.custom_payment = money("50000.00");
    const auto zero_interest = ccsim::MainScenarioCalculator::calculate(input);
    require(zero_interest.has_value(), "Zero interest scenario succeeds");
    require(zero_interest.value().interest.paise() == 0, "Zero APR produces no interest");
}

void test_missed_scenario_uses_main_result_without_interest_on_gst() {
    const auto main = ccsim::MainScenarioCalculator::calculate(main_input(ccsim::PaymentMode::Minimum));
    require(main.has_value(), "Main calculation succeeds");

    const auto optional = ccsim::MissedPaymentSimulator::calculate({
        main.value().estimated_next_balance,
        main.value().remaining_principal,
        rate(4500),
        45,
        365,
        money("750.00"),
        rate(1800),
        true});
    require(optional.has_value(), "Optional calculation consumes main result");
    require(optional.value().starting_balance.paise() == main.value().estimated_next_balance.paise(),
            "Optional starting balance comes from main output");
    require(optional.value().interest.to_string() == "5270.55",
            "Optional interest excludes prior GST and charges");

    const auto original_main_balance = main.value().estimated_next_balance.to_string();
    const auto changed_fee = ccsim::MissedPaymentSimulator::calculate({
        main.value().estimated_next_balance,
        main.value().remaining_principal,
        rate(4500),
        45,
        365,
        money("1200.00"),
        rate(1800),
        true});
    require(changed_fee.has_value(), "Late-fee change recalculates optional result");
    require(main.value().estimated_next_balance.to_string() == original_main_balance,
            "Changing optional fee does not mutate main result");
    require(changed_fee.value().starting_balance.to_string() == optional.value().starting_balance.to_string(),
            "Changing optional fee leaves its starting balance unchanged");

    auto changed_main_input = main_input(ccsim::PaymentMode::Minimum);
    changed_main_input.statement_amount = money("200000.00");
    const auto changed_main =
        ccsim::MainScenarioCalculator::calculate(changed_main_input);
    require(changed_main.has_value(), "Changed main input recalculates");
    const auto recalculated_optional = ccsim::MissedPaymentSimulator::calculate({
        changed_main.value().estimated_next_balance,
        changed_main.value().remaining_principal,
        rate(4500),
        45,
        365,
        money("750.00"),
        rate(1800),
        true});
    require(recalculated_optional.has_value(), "Optional scenario recalculates from changed main output");
    require(recalculated_optional.value().starting_balance.paise()
                == changed_main.value().estimated_next_balance.paise(),
            "Optional start follows changed main result");
}

void test_late_fee_options_and_custom_amount() {
    for (const auto& fee : {"500.00", "750.00", "1200.00", "1300.00", "923.41"}) {
        const auto result = ccsim::MissedPaymentSimulator::calculate(golden_input(money(fee)));
        require(result.has_value(), std::string("Late fee option accepted: ") + fee);
        require(result.value().late_fee.paise() == money(fee).paise(),
                std::string("Selected late fee retained: ") + fee);
    }
}

void test_zero_balance_and_inapplicable_fee() {
    auto input = golden_input(money("0.00"));
    input.starting_balance = money("0.00");
    input.interest_bearing_principal = money("0.00");
    const auto no_balance = ccsim::MissedPaymentSimulator::calculate(input);
    require(no_balance.has_value(), "Zero balance calculation succeeds");
    require(no_balance.value().interest.paise() == 0, "Zero balance interest");

    input.late_fee_applicable = false;
    const auto no_fee = ccsim::MissedPaymentSimulator::calculate(input);
    require(no_fee.has_value(), "No-fee calculation succeeds");
    require(no_fee.value().late_fee.paise() == 0, "Inapplicable fee excluded");
    require(no_fee.value().gst_on_late_fee.paise() == 0, "Fee GST excluded when fee is inapplicable");

    input.late_fee_applicable = true;
    const auto zero_fee = ccsim::MissedPaymentSimulator::calculate(input);
    require(zero_fee.has_value(), "Zero fee calculation succeeds");
    require(zero_fee.value().late_fee.paise() == 0, "Zero late fee remains zero");
    require(zero_fee.value().gst_on_late_fee.paise() == 0, "Zero late fee produces zero GST");
}

void test_validation_and_source_independence() {
    const Money original_statement = money("1000.00");
    const Money original_payment = money("50.00");
    const auto invalid_payment =
        ccsim::InputValidator::validate_payment(money("1000.01"), original_statement);
    require(invalid_payment.has_value(), "Payment above statement amount rejected");
    require(original_statement.to_string() == "1000.00", "Validation does not mutate statement");
    require(original_payment.to_string() == "50.00", "Validation does not mutate payment");
    require(!ccsim::InputValidator::validate_period(30, 365).has_value(), "Valid period accepted");
    require(ccsim::InputValidator::validate_period(0, 365).has_value(), "Zero days rejected");
    require(ccsim::InputValidator::validate_period(30, 0).has_value(), "Zero basis rejected");
}

void test_rate_boundaries() {
    require(Rate::from_basis_points(0).has_value(), "Zero rate accepted");
    require(Rate::from_basis_points(Rate::kMaximumBasisPoints).has_value(), "Maximum rate accepted");
    require(!Rate::from_basis_points(-1).has_value(), "Negative rate rejected");
    require(!Rate::from_basis_points(Rate::kMaximumBasisPoints + 1).has_value(),
            "Rate above maximum rejected");
    const auto parsed = Rate::parse_percent("45.5");
    require(parsed.has_value() && parsed.value().basis_points() == 4550, "Percent text parsing");
    require(!Rate::parse_percent("-1").has_value(), "Negative percent text rejected");
}

void test_calculation_api_contract() {
    const std::string main_json = ccsim_main_result("100000.00", "45", 0, "", "5", "45");
    require(main_json.find("\"ok\":true") != std::string::npos, "Main API returns success JSON");
    require(main_json.find("\"estimatedNextBalance\":\"101219.25\"") != std::string::npos,
            "Main API returns calculated balance");

    const std::string custom_period_json =
        ccsim_main_result("100000.00", "45", 0, "", "5", "60");
    require(custom_period_json.find("\"interest\":\"7027.40\"") != std::string::npos,
            "Main API uses a custom interest period");
    require(custom_period_json.find("\"estimatedNextBalance\":\"103292.33\"") != std::string::npos,
            "Main API returns the custom-period balance");

    const std::string missed_json =
        ccsim_missed_result("99146.17", "99146.17", "45", "750", "30");
    require(missed_json.find("\"totalAdditionalCharges\":\"5212.12\"") != std::string::npos,
            "Missed API returns golden charges");

    const std::string integrated_missed_json =
        ccsim_missed_result("101219.25", "95000.00", "45", "750", "45");
    require(integrated_missed_json.find("\"interest\":\"5270.55\"") != std::string::npos,
            "Missed API uses the main scenario's selected period and principal");
    require(integrated_missed_json.find("\"estimatedNewBalance\":\"108323.50\"") != std::string::npos,
            "Missed API returns the integrated final balance");

    const std::string invalid_json = ccsim_main_result("-1", "45", 0, "", "5", "45");
    require(invalid_json.find("\"ok\":false") != std::string::npos,
            "Main API returns explicit validation error");
    const std::string zero_days_json = ccsim_main_result("100000", "45", 0, "", "5", "0");
    require(zero_days_json.find("\"ok\":false") != std::string::npos,
            "Main API rejects a zero-day period");
    const std::string fractional_days_json =
        ccsim_main_result("100000", "45", 0, "", "5", "1.5");
    require(fractional_days_json.find("\"ok\":false") != std::string::npos,
            "Main API rejects fractional days");
    const std::string negative_days_json =
        ccsim_missed_result("99146.17", "99146.17", "45", "750", "-1");
    require(negative_days_json.find("\"ok\":false") != std::string::npos,
            "Missed API rejects negative days");
    const std::string empty_days_json = ccsim_main_result("100000", "45", 0, "", "5", "");
    require(empty_days_json.find("\"ok\":false") != std::string::npos,
            "Main API rejects an empty period");
    const std::string oversized_days_json =
        ccsim_missed_result("99146.17", "99146.17", "45", "750", "4294967296");
    require(oversized_days_json.find("\"ok\":false") != std::string::npos,
            "Missed API rejects an out-of-range period");
}

}  // namespace

int main() {
    const std::vector<std::pair<std::string, std::function<void()>>> tests{
        {"money parsing and formatting", test_money_parsing_and_formatting},
        {"money overflow", test_money_overflow},
        {"interest and half-up rounding", test_interest_and_half_up_rounding},
        {"tax rounding", test_tax_rounding},
        {"missed-payment golden scenario", test_missed_payment_golden_scenario},
        {"main minimum, full and custom payments", test_main_minimum_full_and_custom_payments},
        {"main 30-day, 45-day, and custom periods", test_main_interest_periods},
        {"integrated minimum and missed-payment regression", test_integrated_minimum_and_missed_payment_regression},
        {"main boundaries and errors", test_main_boundaries_and_errors},
        {"missed scenario consumes main result independently", test_missed_scenario_uses_main_result_without_interest_on_gst},
        {"late fee options and custom amount", test_late_fee_options_and_custom_amount},
        {"zero balance and inapplicable fee", test_zero_balance_and_inapplicable_fee},
        {"validation and source independence", test_validation_and_source_independence},
        {"rate boundaries", test_rate_boundaries},
        {"calculation API contract", test_calculation_api_contract},
    };

    int failures = 0;
    for (const auto& [name, test] : tests) {
        try {
            test();
            std::cout << "[PASS] " << name << '\n';
        } catch (const std::exception& error) {
            ++failures;
            std::cerr << "[FAIL] " << name << ": " << error.what() << '\n';
        }
    }
    std::cout << tests.size() - failures << '/' << tests.size() << " test groups passed\n";
    return failures == 0 ? 0 : 1;
}
