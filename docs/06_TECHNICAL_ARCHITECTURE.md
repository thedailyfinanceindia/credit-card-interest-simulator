
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

## 3. Implemented Directory Structure

src/
  domain/
    result.hpp
    money.hpp / money.cpp
    rate.hpp / rate.cpp

  calculation/
    interest_calculator.hpp / .cpp
    percentage_calculator.hpp / .cpp
    tax_calculator.hpp / .cpp
    main_scenario_calculator.hpp / .cpp
    missed_payment_simulator.hpp / .cpp

  validation/
    input_validator.hpp / .cpp

  app/
    wasm_api.hpp / wasm_api.cpp

web/
  index.html
  styles.css
  app.js

tests/
  unit/simulator_tests.cpp
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
- The JavaScript layer never computes money, rates, tax, or interest.
- No duplicated calculation logic.
- No hidden financial assumptions.
- No unnecessary global state.
- No network dependency for calculations.
- Use deterministic calculation functions.
- Keep the engine modular.

The approved illustrative Month 1 model is documented in the financial specification and decisions log. It remains distinct from issuer-specific billing methodology.

## 6. Technology Decisions

Implementation decisions:

- C++20 for the calculation library.
- CMake for reproducible native builds and CTest integration.
- A small in-repository test runner instead of a third-party test dependency.
- The financial library has no UI or network dependencies.
- Static HTML/CSS and minimal JavaScript for responsive presentation; C++ calculations are compiled to WebAssembly using Emscripten.
- No backend, account, database, or network calculation service.
- CTest covers the native C++ library and API wrapper; WebAssembly is built as the static browser application.

JavaScript only collects control values, calls the C++ WebAssembly API, and renders returned results. No financial or business calculation is implemented in browser JavaScript.

## 7. Non-Functional Requirements

- Deterministic results.
- Reliable monetary precision.
- Fast calculations.
- Maintainable code.
- Automated tests.
- Clear separation of responsibilities.
