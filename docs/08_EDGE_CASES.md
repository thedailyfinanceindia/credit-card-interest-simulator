
# Edge Cases

## Input Validation

1. Empty statement amount.
2. Zero statement amount.
3. Negative statement amount.
4. Very large statement amount.
5. More than two decimal places.
6. Negative payment.
7. Custom payment greater than statement amount.
8. Zero interest rate.
9. Negative interest rate.
10. Unusually high interest rate.
11. Invalid GST rate.
12. Negative late fee.
13. Zero-day calculation period.

## Main Calculator

1. Minimum payment.
2. Full payment.
3. Custom payment.
4. Payment equals statement amount.
5. Payment leaves zero balance.
6. Minimum percentage is capped at 100% of the statement amount.
7. Main calculation produces zero interest.

## Missed-Payment Simulator

1. Default late fee of ₹750.
2. Late fee of ₹500.
3. Late fee of ₹1,200.
4. Late fee of ₹1,300.
5. Custom late fee.
6. Zero late fee.
7. Starting balance is zero.
8. Starting balance is very small.
9. Interest rounds to one paise.
10. GST rounds to one paise.
11. Main inputs change while optional card is open.
12. Late fee changes repeatedly.
13. Optional card is opened and closed repeatedly.

## State Independence

- Opening optional scenario must not change main result.
- Changing late fee must not change main result.
- Changing main inputs must update the optional scenario.
- Hiding optional scenario must not destroy main inputs.

The test suite covers the main-result handoff, fee-option recalculation, and invalid-result clearing. Browser checks additionally verify that the optional card is collapsed initially and that a mobile-width viewport has no horizontal overflow.

## Financial Limitations

The MVP does not model:

- Daily transaction-level balances.
- Bank-specific payment allocation.
- Exact statement dates.
- Bank-specific MAD formulas.
- Exact interest-free-period restoration.
- Exact credit-bureau reporting outcomes.

## Error Handling

Invalid calculations must return explicit errors.

Never display a plausible-looking financial result when inputs are invalid.
