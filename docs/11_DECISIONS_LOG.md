
# Decisions Log

Record all important product and technical decisions here.

## D-001: Optional Missed-Payment Scenario

Status: APPROVED

Decision:

The missed-payment scenario is collapsed by default and accessible through one tap.

Reason:

Keep the main calculator simple.

## D-002: No Bank Selection

Status: APPROVED

Decision:

No mandatory bank selection in MVP.

Reason:

Avoid unnecessary complexity and false bank-specific precision.

## D-003: Default Late Fee

Status: APPROVED

Decision:

Default illustrative late fee is ₹750.

Available selections:

₹500, ₹750, ₹1,200, ₹1,300 and Custom.

## D-004: Separate Charges

Status: APPROVED

Decision:

Display interest, GST on interest, late fee and GST on late fee separately where applicable.

## D-005: Monetary Precision

Status: APPROVED

Decision:

Display INR values to two decimal places.

Use integer paise or equivalent safe decimal arithmetic internally.

## D-006: No Mandatory Dates

Status: APPROVED

Decision:

Do not ask for due date or number of days late in MVP.

Assume the payment remains unpaid beyond the relevant applicable period.

## D-007: Illustrative Starting Balance

Status: PROVISIONAL

Decision:

Use ₹99,146.17 as the starting balance in the missed-payment example.

The Month 1 formula still needs validation.

## D-008: Corrected Missed-Payment Example

Status: APPROVED FOR STATED ASSUMPTIONS

Starting balance: ₹99,146.17

Interest: ₹3,667.05

GST on interest: ₹660.07

Late fee: ₹750.00

GST on late fee: ₹135.00

Total charges: ₹5,212.12

Final balance: ₹1,04,358.29

## D-009: Educational Simulator

Status: APPROVED

Decision:

The MVP provides illustrative estimates, not exact bank-specific statements.

## OPEN DECISIONS

The following must be resolved before the calculation engine is finalized:

1. Exact Minimum Amount Due formula.
2. Main scenario interest-bearing principal.
3. Payment allocation methodology.
4. Full-payment behaviour.
5. Interest-free-period treatment.
6. Exact rounding sequence.
7. GST applicability and calculation basis.
8. Verified regulatory wording.
9. UI framework.
10. Target platform.
11. C++ standard.
12. Build and testing tools.
