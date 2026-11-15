
# Financial Calculation Specification

Status: Provisional. Financial model requires validation before public release.

## 1. Monetary Precision

- Currency: INR.
- Internal monetary representation: integer paise.
- Display precision: two decimal places.
- Do not use double as the authoritative monetary representation.
- Define rounding explicitly.
- Use round-half-up to the nearest paise for the documented example.

## 2. Interest Calculation

For the simplified educational model:

Interest = Principal × Annual Interest Rate × Number of Days / Day Count Basis

Example:

Principal = ₹99,146.17
Annual Interest Rate = 45%
Number of Days = 30
Day Count Basis = 365

Interest:

₹99,146.17 × 0.45 × 30 / 365

Expected interest = ₹3,667.05

This is an illustrative formula, not a universal bank calculation.

## 3. GST on Interest

Assumed GST rate = 18%.

GST = Interest × GST Rate

₹3,667.05 × 18%

Expected GST = ₹660.07

GST applicability and calculation basis must be validated against current tax rules.

## 4. Main Scenario

Illustrative inputs:

Statement Amount = ₹1,00,000
Payment Made = ₹5,000

Supplied resulting balance:

₹99,146.17

Important:

This is an illustrative baseline supplied for the missed-payment scenario.

Do not reverse-engineer this figure or claim that the Month 1 calculation is universally correct.

Before implementing the main engine, establish:

- Exact MAD formula.
- Payment allocation.
- Interest-bearing principal.
- Billing-cycle assumptions.
- Interest-free-period treatment.
- Tax and fee treatment.

## 5. Optional Missed-Payment Scenario

Starting Balance = ₹99,146.17

Annual Interest Rate = 45%

Days = 30

Day Count Basis = 365

Late Fee = ₹750

GST = 18%

### Interest

₹99,146.17 × 0.45 × 30 / 365

= ₹3,667.05

### GST on Interest

₹3,667.05 × 18%

= ₹660.07

### Late Fee

= ₹750.00

### GST on Late Fee

₹750 × 18%

= ₹135.00

### Total Additional Charges

₹3,667.05
+ ₹660.07
+ ₹750.00
+ ₹135.00

= ₹5,212.12

### Final Balance

₹99,146.17 + ₹5,212.12

= ₹1,04,358.29

## 6. Important Assumptions

The example assumes:

- Entire starting balance is interest-bearing.
- Interest period is 30 days.
- No payment occurs during the period.
- No additional purchases occur.
- Late fee applies.
- GST assumptions apply.
- No interest is added on GST or fees.

## 7. Real-World Limitations

Actual issuer calculations may depend on:

- Transaction dates.
- Payment credit dates.
- Daily outstanding balances.
- Payment allocation.
- Previous statement balances.
- Refunds and reversals.
- Cash advances.
- Loss of interest-free period.
- Issuer-specific terms.

The MVP must call this an illustrative interest estimate.

## 8. Future Improvements

Possible future enhancements:

- Transaction-level daily balance calculation.
- Bank-specific calculation profiles.
- Actual statement reconstruction.
- Multiple billing-cycle simulation.

These are outside the initial MVP.
