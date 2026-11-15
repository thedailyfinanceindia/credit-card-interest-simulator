
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

Status: SUPERSEDED BY D-012

The originally provisional ₹99,146.17 example was replaced by the approved illustrative Month 1 assumptions in D-012. The amount remains the standalone missed-payment fixture's starting balance under D-008.

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

## D-010: C++ Build and Test Foundation

Status: APPROVED

Decision:

- Use C++20 and CMake for the calculation library.
- Use CTest with a small in-repository test runner to avoid introducing an external testing dependency before it is needed.
- Represent money as integer paise and percentages as integer basis points.
- Round each interest and GST line half up to the nearest paise.

Reason:

These choices meet the precision and testability requirements while keeping the financial core dependency-free.

## D-011: RBI Research and Scope

Status: RESEARCHED; REGULATORY REVIEW REQUIRED BEFORE RELEASE

Decision:

Use the RBI Master Direction - Credit Card and Debit Card - Issuance and Conduct Directions, 2022, updated on 7 March 2024, as the primary current reference reviewed for the MVP. RBI defines the broad constraints but does not publish one universal MAD percentage or issuer billing algorithm. The missed-payment example remains an illustrative scenario only.

Sources:

- RBI Master Direction: https://www.rbi.org.in/Scripts/BS_ViewMasDirections.aspx?id=12300
- SBI Card MITC example: https://www.sbicard.com/en/most-important-terms-and-conditions.page

Current GST applicability and taxable values must still be verified against CBIC/GST Council primary sources before release. The fixture's 18% is treated as an input assumption, not a universal legal conclusion.

## D-012: Approved Illustrative Month 1 Model

Status: APPROVED - ILLUSTRATIVE MVP ONLY

Decision:

For the simple MVP, the user types a statement/outstanding balance. Minimum payment is an editable percentage of that amount, default 5%, capped at the amount; no extra minimum-rupee floor is assumed. The calculation uses the remaining balance as illustrative principal for 30 days at a 365-day basis. GST on interest is a separate 18% scenario assumption. Full payment produces no new modeled interest, while previously accrued/unbilled amounts are disclosed as excluded.

The optional scenario consumes the main estimated next balance as its starting total and uses the main remaining principal as the new-interest principal. This prevents charging new interest on GST from Scenario A.

Reason:

Keep the MVP simple, allow an editable assumption rather than claiming one MAD method is universal, and preserve the separation of principal, interest, GST, and fees.

The standalone missed-payment golden fixture remains a separate case: its stated assumption treats the entire ₹99,146.17 starting balance as interest-bearing. It is not the integrated Scenario B interest base.

## D-013: Browser UI and C++ WebAssembly

Status: APPROVED

Decision:

Use static HTML/CSS and minimal JavaScript for accessible responsive presentation. Compile the C++ calculation library to WebAssembly with Emscripten. JavaScript performs no financial calculation. Use no backend service or third-party UI framework.

Reason:

This keeps business and financial logic in C++, permits a mobile-first browser UI, and avoids a server or runtime package dependency.

## OPEN DECISIONS

1. Verify current GST applicability/rates/taxable bases against CBIC/GST Council primary sources before release.
2. Obtain final compliance review of regulatory wording and confirm any RBI amendments after the 7 March 2024 consolidation before release.
3. Implement CI, accessibility audit, and cross-browser/release testing.
