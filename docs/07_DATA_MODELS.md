
# Data Models

All monetary values must use integer paise or an equally safe decimal representation.

## 1. MainScenarioInput

Fields:

- statement_amount_paise
- annual_interest_rate
- payment_mode
- custom_payment_paise
- minimum_due_assumption
- period_days
- day_count_basis
- gst_rate

Payment modes:

- Minimum
- Full
- Custom

## 2. MainScenarioResult

Fields:

- payment_amount_paise
- remaining_principal_paise
- interest_paise
- gst_on_interest_paise
- other_charges_paise
- estimated_next_balance_paise
- assumptions

## 3. MissedPaymentInput

Fields:

- starting_balance_paise
- annual_interest_rate
- period_days
- day_count_basis
- late_fee_paise
- gst_rate
- late_fee_applicable

The starting balance must come from MainScenarioResult.

## 4. MissedPaymentResult

Fields:

- starting_balance_paise
- interest_paise
- gst_on_interest_paise
- late_fee_paise
- gst_on_late_fee_paise
- total_additional_charges_paise
- estimated_new_balance_paise
- assumptions

## 5. Validation

Reject:

- Negative monetary amounts.
- Invalid interest rates.
- Invalid payment amounts.
- Invalid day counts.
- Invalid tax rates.
- Arithmetic overflow.
- Unsupported monetary precision.

Do not silently correct invalid inputs.
