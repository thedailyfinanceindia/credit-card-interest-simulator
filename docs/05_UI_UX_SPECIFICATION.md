
# UI/UX Specification

## Design Objective

Create a simple, mobile-friendly financial calculator.

The main calculator must remain uncluttered.

Additional complexity must appear only when the user requests it.

## Main Screen

Title:

Credit Card Minimum Due Calculator

### Input Section

- Statement Amount
- Annual Interest Rate
- Minimum / Full / Custom payment selection
- An editable illustrative minimum-payment percentage (5% default), visible when Minimum is selected

### Result Section

Display:

- Payment Amount
- Interest
- GST
- Remaining Balance
- Estimated New Balance

The new balance should be the most prominent result.

### Optional Section

Collapsed by default.

Button:

"Explore a missed-payment scenario"

### Expanded Card

Title:

"How a missed payment could affect the balance"

Display:

- Starting balance
- Assumed interest rate
- Interest
- GST on interest
- Late fee
- GST on late fee
- Total additional charges
- Estimated new balance

### Late Fee Selector

Default: ₹750

Options:

- ₹500
- ₹750
- ₹1,200
- ₹1,300
- Custom

### Warnings

Keep the main minimum-payment warning and missed-payment warning visible and concise. Put longer interest-free-period, APR, compounding, RBI past-due timing, issuer-fee, and illustrative-assumption explanations in collapsed expandable disclosures.

State that the 5% minimum payment and listed late-fee choices are illustrative, not universal RBI rules. Identify the 18% GST rate as an illustrative assumption.

Provide a compact expandable list of charges excluded from the estimate. The final disclaimer must state that actual billing depends on transaction dates, payment dates, payment allocation, refunds, reversals, and issuer-specific terms.

## Visual Principles

- Mobile-first layout.
- Clear typography.
- High contrast.
- Simple financial breakdown.
- No unnecessary animations.
- Important results should be easy to understand.
- Use warnings without fear-based messaging.
- Describe scenarios without blaming or judging the user's choices.

## Accessibility

- Label all inputs.
- Support keyboard navigation.
- Provide clear validation messages.
- Do not communicate warnings through colour alone.

The main calculator places the editable interest-period control directly after the interest estimate in the result breakdown. It defaults to 45 days, is disabled for Full payment, and includes an information disclosure explaining the illustrative assumption and transaction-date limitation. Editing it recalculates the main result and, when expanded, Scenario B. No dates are requested. The optional missed-payment card remains collapsed by default, uses the same selected period, and receives both the starting total and unpaid principal from the main calculation.
