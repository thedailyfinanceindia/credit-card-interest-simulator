
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
- [x] Add GitHub Actions C++20 build-and-test workflow (first hosted run pending).
- [x] Implement monetary representation.

## Stage 2: Calculation Engine

- [x] Implement Money.
- [x] Implement Rate.
- [x] Implement InterestCalculator.
- [x] Implement TaxCalculator.
- [x] Implement MainScenarioCalculator.
- [x] Implement MissedPaymentSimulator.
- [x] Implement InputValidator.
- [x] Support an editable 45-day-default interest period and test 30-day, default, custom, and invalid periods.
- [x] Add unit tests for the implemented financial functions.
- [x] Pass the missed-payment golden test case.
- [x] Add integrated main-to-missed-payment regression with independent expected values.

## Stage 3: User Interface

- [x] Build main calculator.
- [x] Add editable illustrative interest-period input; Scenario B reuses the selected period.
- [x] Add Minimum / Full / Custom selection.
- [x] Display itemized results.
- [x] Add optional missed-payment card.
- [x] Add editable late fee.
- [x] Add warnings and disclaimers.
- [x] Test responsive layout (390px viewport; no horizontal overflow).
- [x] Verify browser layout at 320px, 360px, and 390px.

## Stage 4: Quality Assurance

- [ ] Financial calculation review.
- [ ] Regulatory review.
- [ ] Accessibility testing.
- [ ] Browser-test the editable interest-period control and Scenario B period recalculation.
- [x] Edge-case testing for implemented calculations.
- [ ] Cross-browser testing, if applicable.
- [ ] Release readiness review.

## Stage 5: Repository Quality and Automation

- [x] Add least-permission GitHub Actions workflow for C++20 configure, build, and CTest; no deployment.
- [x] Add integrated financial regression test without reusing the standalone golden expectation.
- [x] Clean-build WebAssembly and verify generated browser assets load.
- [x] Verify Minimum, Full, Custom, optional scenario, fee changes, invalid inputs, state independence, and mobile widths in-browser.
- [ ] Observe a successful GitHub-hosted workflow run (requires a push or pull request; local WSL lacks CMake).

The MVP implementation is complete. The hosted CI run, final financial/regulatory review, full accessibility/cross-browser validation, and release readiness remain incomplete; the roadmap is not marked done.

## Out of Scope

- Bank-specific profiles.
- Backend.
- User accounts.
- Cloud database.
- Cloudflare.
- Instagram automation.
- Payment integrations.
