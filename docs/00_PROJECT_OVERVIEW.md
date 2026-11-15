
# Credit Card Interest Simulator
## Project Overview

### Product identity

Product: Credit Card Interest Simulator

Publisher: Daily Finance India

Market: India

Currency: INR (₹)

Category: Consumer financial education

Product type: Interactive calculator and financial simulator.

## 1. Problem Statement

Many credit-card users believe that paying only the Minimum Amount Due (MAD) solves their immediate payment problem.

However, paying only the minimum generally leaves an unpaid balance on which finance charges may continue.

Missing a subsequent payment can introduce additional consequences:

- Finance charges
- Late-payment fees
- Applicable GST
- Potential credit-bureau reporting

Consumers need a simple way to understand how these decisions affect their outstanding balance.

## 2. Product Objective

Build an educational simulator that explains two situations:

1. What happens when a user pays only the minimum amount due on time?
2. What happens when the user misses the following month's payment?

The product must explain the financial impact using transparent calculations.

## 3. Core Product Philosophy

- Simple by default.
- Additional complexity must remain optional.
- Financial calculations must be transparent.
- Every assumption must be disclosed.
- Avoid misleading or exaggerated claims.
- Distinguish estimates from actual bank charges.
- Prioritize educational value over unnecessary features.

## 4. MVP Scope

### Main Calculator

Inputs:

- Credit-card statement amount.
- Annual interest rate.
- Payment selection: Minimum, Full or Custom.

Outputs:

- Payment amount.
- Remaining balance.
- Interest.
- GST on interest, where applicable.
- Estimated next balance.

### Optional Missed-Payment Simulator

A collapsed card:

"Explore a missed-payment scenario"

When expanded:

- Uses the main calculator's resulting balance.
- Assumes the following payment is completely missed.
- Calculates additional illustrative interest.
- Adds an editable late-payment fee.
- Calculates applicable GST separately.
- Displays the resulting balance.

Default late fee: ₹750.

Quick selections:

- ₹500
- ₹750
- ₹1,200
- ₹1,300
- Custom amount

## 5. Explicitly Out of Scope

Do not implement these in the initial MVP:

- Bank-specific calculation profiles.
- User accounts.
- Backend infrastructure.
- Cloud database.
- Payment processing.
- Instagram automation.
- Cloudflare integration.
- Personal financial account connections.
- Transaction-level statement reconstruction.

## 6. Product Principles

The main screen must remain simple.

The optional missed-payment simulator must not introduce additional mandatory fields.

The application must not suggest that its estimate is an exact representation of every bank's calculation methodology.

## 7. Target Audience

Indian credit-card users who want to understand:

- Minimum Amount Due.
- Credit-card interest.
- GST on finance charges.
- Late-payment fees.
- The consequences of missing payments.

## 8. Success Criteria

- A user can complete the main calculation without assistance.
- The missed-payment scenario is accessible with one tap.
- Every result has a transparent calculation breakdown.
- The same inputs always produce the same outputs.
- Financial calculations have automated tests.
- The application clearly communicates its limitations.
