
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

"What if I miss next month's payment?"

### Expanded Card

Title:

"What happens if you miss the next payment?"

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

Paying the minimum does not stop interest.

Late fees depend on the card issuer and outstanding amount.

The simulator assumes the payment remains unpaid beyond the applicable period for the missed-payment scenario.

## Visual Principles

- Mobile-first layout.
- Clear typography.
- High contrast.
- Simple financial breakdown.
- No unnecessary animations.
- Important results should be easy to understand.
- Use warnings without fear-based messaging.

## Accessibility

- Label all inputs.
- Support keyboard navigation.
- Provide clear validation messages.
- Do not communicate warnings through colour alone.

The main calculator uses a fixed illustrative 30-day/365-day period and does not request dates. The optional missed-payment card remains collapsed by default and receives both the starting total and unpaid principal from the main calculation.
