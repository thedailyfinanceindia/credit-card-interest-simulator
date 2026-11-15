
# Technical Architecture

## 1. Architecture Objective

Keep the financial calculation engine independent of the user interface.

The same calculation functions must be reusable and independently testable.

## 2. Core Modules

### Money

Responsible for:
- INR monetary representation.
- Paise precision.
- Addition and subtraction.
- Rounding.

### InterestCalculator

Responsible for:
- Interest calculations.
- Annual-to-period rate conversion.
- Day-count assumptions.

### TaxCalculator

Responsible for:
- GST calculations.
- Tax rounding.

### MainScenarioCalculator

Responsible for:
- Minimum payment scenario.
- Full payment scenario.
- Custom payment scenario.
- Main balance calculation.

### MissedPaymentSimulator

Responsible for:
- Consuming the main scenario result.
- Calculating additional interest.
- Calculating assumed late fee.
- Calculating GST.
- Producing the final balance.

### InputValidator

Responsible for:
- Validating monetary inputs.
- Validating interest rates.
- Validating payment amounts.
- Preventing invalid calculations.

## 3. Suggested C++ Directory Structure

src/
  domain/
    money.h
    rate.h
    scenario_types.h

  calculation/
    interest_calculator.h
    interest_calculator.cpp
    tax_calculator.h
    tax_calculator.cpp
    main_scenario_calculator.h
    main_scenario_calculator.cpp
    missed_payment_simulator.h
    missed_payment_simulator.cpp

  validation/
    input_validator.h
    input_validator.cpp

  app/
    main.cpp

tests/
  unit/
  integration/
  test-data/

## 4. Data Flow

Main Calculator Input
        |
        v
Input Validation
        |
        v
Main Scenario Calculator
        |
        v
Main Scenario Result
        |
        +--------------------+
        |                    |
        v                    v
Main UI Result       Optional Missed Payment
                             |
                             v
                     Missed Payment Simulator
                             |
                             v
                     Additional Charges
                             |
                             v
                     Final Estimated Balance

## 5. Architecture Rules

- No financial formulas inside UI components.
- No duplicated calculation logic.
- No hidden financial assumptions.
- No unnecessary global state.
- No network dependency for calculations.
- Use deterministic calculation functions.
- Keep the engine modular.

## 6. Technology Decisions

To be finalized:

- Target platform.
- UI framework.
- C++ standard.
- Build system.
- Unit testing framework.
- Deployment approach.

CMake and GoogleTest are candidates, not approved decisions.

Do not select technologies without documenting the decision.

## 7. Non-Functional Requirements

- Deterministic results.
- Reliable monetary precision.
- Fast calculations.
- Maintainable code.
- Automated tests.
- Clear separation of responsibilities.
