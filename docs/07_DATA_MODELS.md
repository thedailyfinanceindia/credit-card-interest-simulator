
# Data Models

All monetary values must use integer paise or an equally safe decimal representation.

## 1. MainScenarioInput

Fields:

- statement_amount_paise
- annual_interest_rate
- payment_mode
- custom_payment_paise
- minimum_due_rate (editable percentage; 5% default in the UI)
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
- estimated_next_balance_paise
- calculation assumptions (30 days, 365-day basis, entered amount treated as eligible principal)

## 3. MissedPaymentInput

Fields:

- starting_balance_paise
- interest_bearing_principal_paise
- interest_bearing_principal_paise
- annual_interest_rate
- period_days
- day_count_basis
- late_fee_paise
- gst_rate
- late_fee_applicable

The starting balance must come from MainScenarioResult. In the interactive flow, the interest-bearing principal is the main result's remaining principal; the starting total also includes the main result's interest and GST. In the interactive flow, the interest-bearing principal is the main result's remaining principal; the starting total also includes the main result's interest and GST.

## 4. MissedPaymentResult

Fields:

- starting_balance_paise
- interest_paise
- gst_on_interest_paise
- late_fee_paise
- gst_on_late_fee_paise
- total_additional_charges_paise
- estimated_new_balance_paise
- calculation assumptions (30 days, 365-day basis, GST and late-fee applicability)

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

## 6. Implemented Numerical Types

- `Money` stores a non-negative signed 64-bit integer number of paise. Parsing accepts decimal rupees with at most two fractional digits and rejects negative values, excess precision and overflow.
- `Rate` stores an integer number of basis points, where 100 basis points equals one percentage point.
- Percentage calculations round half up to the nearest paise, independently for each interest or tax line.
- Arithmetic overflow and invalid inputs produce explicit errors; they do not produce a partial result.

The main scenario's minimum payment assumption is an editable rate (5% default), applied to the entered statement amount and capped at that amount. Its interest-bearing principal is the post-payment remainder for the documented illustrative calculation only.
