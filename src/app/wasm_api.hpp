#pragma once

extern "C" {

const char* ccsim_main_result(
    const char* statement_amount,
    const char* annual_rate,
    int payment_mode,
    const char* custom_payment,
    const char* minimum_due_percent);

const char* ccsim_missed_result(
    const char* starting_balance,
    const char* interest_bearing_principal,
    const char* annual_rate,
    const char* late_fee);

}
