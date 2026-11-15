#include "calculation/main_scenario_calculator.hpp"
#include "calculation/missed_payment_simulator.hpp"
#include "domain/money.hpp"
#include "domain/rate.hpp"

#include <charconv>
#include <cstdint>
#include <string>
#include <string_view>
#include <system_error>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#define CCSIM_KEEPALIVE EMSCRIPTEN_KEEPALIVE
#else
#define CCSIM_KEEPALIVE
#endif

namespace {

std::string response_buffer;

std::string escape_json(std::string_view value) {
    std::string escaped;
    for (char character : value) {
        if (character == '"' || character == '\\') {
            escaped.push_back('\\');
        }
        if (character == '\n') {
            escaped += "\\n";
        } else if (character == '\r') {
            escaped += "\\r";
        } else {
            escaped.push_back(character);
        }
    }
    return escaped;
}

const char* failure(std::string_view message) {
    response_buffer = "{\"ok\":false,\"error\":\"" + escape_json(message) + "\"}";
    return response_buffer.c_str();
}

ccsim::Result<ccsim::Money> parse_money(const char* value) {
    if (value == nullptr) {
        return ccsim::Result<ccsim::Money>::failure("Enter a monetary amount");
    }
    return ccsim::Money::parse(value);
}

ccsim::Result<ccsim::Rate> parse_rate(const char* value) {
    if (value == nullptr) {
        return ccsim::Result<ccsim::Rate>::failure("Enter a percentage");
    }
    return ccsim::Rate::parse_percent(value);
}

ccsim::Result<std::uint32_t> parse_period_days(const char* value) {
    if (value == nullptr || value[0] == '\0') {
        return ccsim::Result<std::uint32_t>::failure("Please enter the interest period in days");
    }

    const std::string_view text(value);
    std::uint32_t days = 0;
    const auto parsed = std::from_chars(text.data(), text.data() + text.size(), days);
    if (parsed.ec == std::errc::result_out_of_range) {
        return ccsim::Result<std::uint32_t>::failure("Please enter a smaller interest period");
    }
    if (parsed.ec != std::errc{} || parsed.ptr != text.data() + text.size()) {
        return ccsim::Result<std::uint32_t>::failure(
            "Please enter a whole number of days for the interest period");
    }
    return ccsim::Result<std::uint32_t>::success(days);
}

std::string currency(const ccsim::Money& value) {
    return value.to_string();
}

}  // namespace

extern "C" {

CCSIM_KEEPALIVE const char* ccsim_main_result(
    const char* statement_amount,
    const char* annual_rate,
    int payment_mode,
    const char* custom_payment,
    const char* minimum_due_percent,
    const char* interest_period_days) {
    const auto statement = parse_money(statement_amount);
    if (!statement.has_value()) {
        return failure(statement.error());
    }
    const auto apr = parse_rate(annual_rate);
    if (!apr.has_value()) {
        return failure(apr.error());
    }
    const auto period_days = parse_period_days(interest_period_days);
    if (!period_days.has_value()) {
        return failure(period_days.error());
    }
    if (payment_mode < 0 || payment_mode > 2) {
        return failure("Choose a valid payment mode");
    }
    const auto minimum_rate = payment_mode == 0
        ? parse_rate(minimum_due_percent)
        : ccsim::Rate::from_basis_points(500);
    if (!minimum_rate.has_value()) {
        return failure(minimum_rate.error());
    }

    ccsim::MainScenarioInput input{
        statement.value(),
        apr.value(),
        static_cast<ccsim::PaymentMode>(payment_mode),
        std::nullopt,
        minimum_rate.value(),
        period_days.value(),
        365,
        ccsim::Rate::from_basis_points(1800).value()};

    if (input.payment_mode == ccsim::PaymentMode::Custom) {
        const auto payment = parse_money(custom_payment);
        if (!payment.has_value()) {
            return failure(payment.error());
        }
        input.custom_payment = payment.value();
    }

    const auto result = ccsim::MainScenarioCalculator::calculate(input);
    if (!result.has_value()) {
        return failure(result.error());
    }

    response_buffer =
        "{\"ok\":true,\"statement\":\"" + currency(result.value().statement_amount)
        + "\",\"payment\":\"" + currency(result.value().payment_amount)
        + "\",\"remainingPrincipal\":\"" + currency(result.value().remaining_principal)
        + "\",\"interest\":\"" + currency(result.value().interest)
        + "\",\"gstOnInterest\":\"" + currency(result.value().gst_on_interest)
        + "\",\"estimatedNextBalance\":\"" + currency(result.value().estimated_next_balance) + "\"}";
    return response_buffer.c_str();
}

CCSIM_KEEPALIVE const char* ccsim_missed_result(
    const char* starting_balance,
    const char* interest_bearing_principal,
    const char* annual_rate,
    const char* late_fee,
    const char* interest_period_days) {
    const auto balance = parse_money(starting_balance);
    if (!balance.has_value()) {
        return failure(balance.error());
    }
    const auto principal = parse_money(interest_bearing_principal);
    if (!principal.has_value()) {
        return failure(principal.error());
    }
    const auto apr = parse_rate(annual_rate);
    if (!apr.has_value()) {
        return failure(apr.error());
    }
    const auto period_days = parse_period_days(interest_period_days);
    if (!period_days.has_value()) {
        return failure(period_days.error());
    }
    const auto fee = parse_money(late_fee);
    if (!fee.has_value()) {
        return failure(fee.error());
    }

    const ccsim::MissedPaymentInput input{
        balance.value(),
        principal.value(),
        apr.value(),
        period_days.value(),
        365,
        fee.value(),
        ccsim::Rate::from_basis_points(1800).value(),
        true};
    const auto result = ccsim::MissedPaymentSimulator::calculate(input);
    if (!result.has_value()) {
        return failure(result.error());
    }

    response_buffer =
        "{\"ok\":true,\"startingBalance\":\"" + currency(result.value().starting_balance)
        + "\",\"interest\":\"" + currency(result.value().interest)
        + "\",\"gstOnInterest\":\"" + currency(result.value().gst_on_interest)
        + "\",\"lateFee\":\"" + currency(result.value().late_fee)
        + "\",\"gstOnLateFee\":\"" + currency(result.value().gst_on_late_fee)
        + "\",\"totalAdditionalCharges\":\"" + currency(result.value().total_additional_charges)
        + "\",\"estimatedNewBalance\":\"" + currency(result.value().estimated_new_balance) + "\"}";
    return response_buffer.c_str();
}

}  // extern "C"
