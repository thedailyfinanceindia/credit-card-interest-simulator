
# User Flows

## Flow 1: Minimum Payment

1. User opens the calculator.
2. Enters statement amount.
3. Reviews or changes annual interest rate.
4. Selects Minimum.
5. Reviews or edits the illustrative minimum-payment percentage (5% by default).
6. Reviews estimated payment.
7. Sees interest and GST breakdown.
8. Sees estimated next balance.

## Flow 2: Full Payment

1. User enters statement amount.
2. Selects Full.
3. Calculator displays zero new modeled interest and clearly excludes previously accrued or unbilled amounts.

Do not automatically assume that previously accrued or unbilled interest disappears.

## Flow 3: Custom Payment

1. User enters statement amount.
2. Selects Custom.
3. Enters payment amount.
4. Calculator validates the amount.
5. Displays estimated balance and charges.

## Flow 4: Missed Next Payment

1. User completes the main calculation.
2. User opens "Explore a missed-payment scenario".
3. Optional card expands.
4. Starting balance is taken from the main result.
5. Default late fee is ₹750.
6. User can select another fee or enter a custom amount.
7. Simulator calculates additional charges.
8. Simulator displays the new estimated balance.

## Interaction Requirements

- Optional card is collapsed by default.
- Expanding the card must not reset main inputs.
- Editing the late fee must not change the main result.
- Changing main inputs must recalculate the optional scenario.
- No bank selection.
- No mandatory date input.
- Invalid values must produce clear validation messages.
- The illustrative main calculation defaults to 45 days; users may edit the whole-number period. It uses a 365-day basis and makes clear that transaction-date interest may differ.
- Scenario B uses the same selected interest period as Scenario A.
