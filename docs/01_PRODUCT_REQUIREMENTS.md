
# Product Requirements Document (PRD)

## 1. Objective

Create a credit-card simulator that demonstrates the financial impact of paying only the minimum due and subsequently missing a payment.

## 2. Main Calculator

### Inputs

1. Statement Amount
2. Annual Interest Rate (default illustrative rate: 45% p.a.)
3. Payment Mode:
   - Minimum
   - Full
   - Custom

The minimum payment calculation must be configurable and must not assume every bank uses 5%.

For the approved simple MVP estimate, show an editable illustrative percentage with a 5% default, applied to the entered statement amount and capped at that amount. This is a user-adjustable estimate, not the issuer's MAD formula. No rupee minimum/floor is assumed.

### Outputs

- Statement amount.
- Payment made.
- Remaining balance.
- Interest.
- GST on interest.
- Estimated next statement balance.

### Warnings

1. Banks may calculate MAD differently. The percentage used is an estimate.
2. Paying the minimum amount does not generally stop interest.
3. Missing a payment can introduce additional charges and reporting consequences.

The main estimate uses an editable illustrative period, defaulting to 45 days, and a 365-day basis; no statement or due date is required. The selected period is a simplified approximation, not a universal bank rule or the actual duration for every transaction. It treats the entered balance as eligible principal and does not model previously accrued or unbilled charges.

## 3. Optional Missed-Payment Scenario

Trigger:

"What if I miss next month's payment?"

The scenario is collapsed by default.

When expanded:

- Starting balance comes from the main calculator.
- Assumed payment missed completely.
- Default late fee: ₹750.
- Editable late fee.
- Additional interest calculation.
- GST on interest.
- GST on late fee, where applicable.
- Final estimated balance.

### Late Fee Options

₹500, ₹750, ₹1,200, ₹1,300 and Custom.

The default is ₹750.

Display:

"Late fee depends on your bank and outstanding amount. ₹750 is an illustrative estimate, not a universal bank charge."

## 4. User Experience Requirements

- No mandatory bank selection.
- No mandatory date selection.
- No additional mandatory fields for the missed-payment scenario.
- The main calculator must remain unchanged when the optional scenario is opened.
- Changing the late fee must update only the optional scenario.
- The optional scenario must use the main scenario's output.

## 5. Financial Display Requirements

All monetary values must display in INR.

Use two decimal places.

Display separate line items for:

- Interest.
- GST on interest.
- Late fee.
- GST on late fee.
- Total additional charges.
- New estimated balance.

## 6. Disclaimer

This simulator provides educational estimates based on stated assumptions.

Actual charges depend on the card issuer's terms, transaction history, payment dates, billing cycle and applicable regulations.
