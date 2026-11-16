
# Financial Calculation Specification

Status: Illustrative MVP model approved 15 November 2026. Issuer-specific and regulatory validation remains required before public release.

## 1. Monetary Precision

- Currency: INR.
- Internal monetary representation: integer paise.
- Display precision: two decimal places.
- Do not use double as the authoritative monetary representation.
- Define rounding explicitly.
- Use round-half-up to the nearest paise for the documented example.

## 2. Interest Calculation

For the simplified educational model, the user selects a positive whole-number period in days. The UI defaults to 45 days; this is an illustrative assumption, not an RBI-prescribed period or universal issuer practice. No transaction or payment dates are requested.

Interest = Principal × Annual Interest Rate × Number of Days / Day Count Basis

Example:

Principal = ₹99,146.17
Annual Interest Rate = 45%
Number of Days = 30 for this standalone example
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

### Approved MVP formula

The user enters a statement/outstanding amount, annual rate, and payment mode. No statement upload or date input is required.

- Minimum payment = entered statement amount × editable illustrative MAD percentage (default 5%), rounded half up to paise and capped at the statement amount. No minimum rupee floor is assumed.
- Full payment = the entered statement amount.
- Custom payment = the user-entered amount; it cannot exceed the entered statement amount.
- Remaining principal = statement amount − payment.
- Interest = remaining principal × annual rate × selected period days / 365, rounded half up to paise.
- GST on interest = interest × the supplied illustrative GST rate (18% in the app and golden fixture), rounded half up to paise.
- Estimated next balance = remaining principal + interest + GST on interest.

This model assumes the entire entered statement amount is eligible principal and that there are no separately identified taxes, fees, or prior accrued/unbilled interest in it. The model applies interest only to the unpaid principal and does not add tax or fees to the interest base. It is not a universal issuer methodology.

### Approved example

Statement amount = ₹1,00,000; annual rate = 45%; illustrative MAD percentage = 5%; default period = 45 days; day-count basis = 365; GST assumption = 18%.

Payment = ₹5,000; remaining principal = ₹95,000; interest = ₹5,270.55; GST = ₹948.70; estimated next balance = ₹1,01,219.25.

For comparison, selecting 30 days produces interest of ₹3,513.70, GST of ₹632.47, and an estimated next balance of ₹99,146.17. Neither result validates any bank's real MAD or billing methodology.

For Full payment, the model returns zero remaining principal, new interest, GST, and next balance. Previously accrued or unbilled amounts are explicitly outside this simplified scenario and are not represented as waived.

## 5. Optional Missed-Payment Scenario

The following is the standalone 30-day golden fixture; it is separate from the interactive scenario, which inherits the selected period from Scenario A.

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

The standalone golden example assumes:

- Entire starting balance is interest-bearing.
- Interest period is 30 days.
- No payment occurs during the period.
- No additional purchases occur.
- Late fee applies.
- GST assumptions apply.
- No interest is added on GST or fees.

In the interactive flow, Scenario B consumes the MainScenarioResult and uses the same selected period as Scenario A: its starting balance is the main estimated next balance, while its new-interest principal is the main remaining principal. Thus prior main-scenario GST is included in the starting total but excluded from the new interest base. The standalone golden fixture explicitly assumes its entire ₹99,146.17 starting balance is interest-bearing for 30 days, so its ₹3,667.05 interest result is a separate fixture case, not the integrated Scenario B interest base.

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

When the interest-free period is lost, interest may be calculated from transaction dates; actual interest duration varies by transaction and payment date. Repeated balances carried through real billing cycles may accrue interest over time. This simulator instead applies simple interest for one selected period; it does not model transaction-level billing or recurring monthly compounding.

The MVP must call this an illustrative interest estimate. Its default 45-day period is an editable approximation and must not be presented as a regulatory requirement or universal bank practice.

## 8. Regulatory Research (Reviewed 15 November 2026)

The RBI's [Master Direction - Credit Card and Debit Card - Issuance and Conduct Directions, 2022](https://www.rbi.org.in/Scripts/BS_ViewMasDirections.aspx?id=12300) is published as updated on 7 March 2024. The relevant provisions state, in summary:

- Issuers determine rates under their board-approved policy and must disclose APR methodology with examples (paragraph 9(a) and 9(b)(i)).
- Minimum Amount Due terms must avoid negative amortization; the direction does not prescribe one universal MAD percentage (paragraph 9(b)(ii)).
- A card-issuer may report an account as past due or levy penal charges only when it has remained past due for more than three days; late charges are computed from the statement due date and apply only to the post-due outstanding, not the total amount due (paragraph 9(b)(v)).
- Interest is levied only on the outstanding amount, adjusted for payments, refunds and reversed transactions (paragraph 9(b)(vi), and FAQ 6).
- Unpaid taxes, levies and charges must not be capitalized for interest (paragraph 9(b)(ii), and FAQ 7).

The official [SBI Card MITC](https://www.sbicard.com/en/most-important-terms-and-conditions.page) is an issuer-specific example, not a market-wide rule. Its published MAD example includes applicable taxes, EMI, fees/charges, finance charges, over-limit amounts and a percentage of the remaining balance, subject to a stated minimum. It also documents payment allocation and daily finance-charge examples. This demonstrates why those issuer practices cannot be inferred from a single statement amount.

The RBI direction is the regulatory reference used for cautious product wording, not a complete issuer billing model. These sources were checked on 15 November 2026; the direction page identifies its consolidation date as 7 March 2024. Recheck for subsequent amendments and complete compliance review before release.

The 18% GST rate in the supplied missed-payment fixture remains an explicit illustrative input. It is not, by itself, a claim that GST applies to every issuer charge or taxable amount. Current GST applicability and the taxable value for each charge remain a release-verification item against CBIC/GST Council primary sources.

## 9. Future Improvements

Possible future enhancements:

- Transaction-level daily balance calculation.
- Bank-specific calculation profiles.
- Actual statement reconstruction.
- Multiple billing-cycle simulation.

These are outside the initial MVP.
