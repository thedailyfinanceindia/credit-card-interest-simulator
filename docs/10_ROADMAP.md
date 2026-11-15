
# Project Roadmap

Statuses: NOT STARTED / IN PROGRESS / BLOCKED / DONE.

## Stage 0: Documentation

- [x] Add repository documentation.
- [x] Review product requirements.
- [x] Finalize the illustrative Month 1 model; issuer methodology remains out of scope.
- [ ] Verify GST applicability and taxable bases against current official sources.
- [x] Review RBI credit-card direction and issuer disclosures; repeat before release.

## Stage 1: Project Foundation

- [x] Select target platform (static browser application).
- [x] Select UI framework (HTML/CSS/JavaScript presentation with C++ WebAssembly engine).
- [x] Finalize C++ standard (C++20).
- [x] Configure CMake.
- [x] Configure dependency-free CTest test runner.
- [ ] Establish CI build.
- [x] Implement monetary representation.

## Stage 2: Calculation Engine

- [x] Implement Money.
- [x] Implement Rate.
- [x] Implement InterestCalculator.
- [x] Implement TaxCalculator.
- [x] Implement MainScenarioCalculator.
- [x] Implement MissedPaymentSimulator.
- [x] Implement InputValidator.
- [x] Add unit tests for the implemented financial functions.
- [x] Pass the missed-payment golden test case.

## Stage 3: User Interface

- [x] Build main calculator.
- [x] Add Minimum / Full / Custom selection.
- [x] Display itemized results.
- [x] Add optional missed-payment card.
- [x] Add editable late fee.
- [x] Add warnings and disclaimers.
- [x] Test responsive layout (390px viewport; no horizontal overflow).

## Stage 4: Quality Assurance

- [ ] Financial calculation review.
- [ ] Regulatory review.
- [ ] Accessibility testing.
- [x] Edge-case testing for implemented calculations.
- [ ] Cross-browser testing, if applicable.
- [ ] Release readiness review.

The MVP implementation is complete. CI, final financial/regulatory review, full accessibility/cross-browser validation, and release readiness remain incomplete; the roadmap is not marked done.

## Out of Scope

- Bank-specific profiles.
- Backend.
- User accounts.
- Cloud database.
- Cloudflare.
- Instagram automation.
- Payment integrations.
