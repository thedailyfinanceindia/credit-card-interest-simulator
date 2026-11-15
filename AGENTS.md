
# AI Coding Agent Instructions

You are working on the Credit Card Interest Simulator, an educational financial calculator for Indian credit-card users, published under Daily Finance India.

The GitHub repository is the single source of truth. Do not depend on previous ChatGPT conversations for project requirements.

## Mandatory reading

Before making changes, read:

1. README.md
2. docs/00_PROJECT_OVERVIEW.md
3. docs/01_PRODUCT_REQUIREMENTS.md
4. docs/02_USER_FLOWS.md
5. docs/03_FINANCIAL_CALCULATION_SPEC.md
6. docs/04_REGULATORY_RULES.md
7. docs/05_UI_UX_SPECIFICATION.md
8. docs/06_TECHNICAL_ARCHITECTURE.md
9. docs/07_DATA_MODELS.md
10. docs/08_EDGE_CASES.md
11. docs/09_TEST_STRATEGY.md
12. docs/10_ROADMAP.md
13. docs/11_DECISIONS_LOG.md

## Language and engineering

- All calculation and business logic must be implemented in C++.
- Follow the C++ standard configured by the project.
- Keep financial calculations independent of the UI.
- Use integer paise or a rigorously specified decimal representation for money.
- Never use double as the authoritative representation of monetary values.
- Make rounding rules explicit.
- Avoid unnecessary dependencies.
- Keep interfaces small, modular and testable.
- Do not introduce features outside the approved MVP.

## Product constraints

- The main calculator is the default screen.
- The missed-next-month scenario is optional and collapsed by default.
- No mandatory bank selection.
- No mandatory due-date or days-late input.
- Default illustrative late fee is ₹750.
- User can select ₹500, ₹750, ₹1,200, ₹1,300 or enter a custom amount.
- Opening or changing the optional scenario must not modify the main calculation.
- The optional scenario must consume the main calculator's result.

## Financial correctness

- Treat every output as an estimate unless a validated issuer-specific model is implemented.
- Do not claim that one formula represents every Indian bank.
- Separate principal, interest, GST and late-payment fee.
- Do not silently charge interest on GST or fees.
- Validate formulas against documented test cases.
- Never invent regulatory requirements.
- Verify RBI and GST claims against official sources before release.
- Preserve the assumptions behind every calculation.

## Development workflow

Before coding:

1. Inspect the repository and current branch.
2. Read the relevant specifications.
3. Identify unresolved decisions.
4. Present an implementation plan.
5. Do not make assumptions where financial correctness is affected.

During development:

1. Implement small changes.
2. Add unit tests.
3. Run the build and tests.
4. Report actual results.
5. Update documentation when behaviour changes.

Never overwrite unrelated user changes.

## Documentation rules

If a product or technical decision changes:

- Update the relevant specification.
- Add an entry to docs/11_DECISIONS_LOG.md.
- Update docs/10_ROADMAP.md if implementation status changes.

## Definition of done

A feature is complete only when:

- Requirements are satisfied.
- Financial calculations are tested.
- Edge cases are considered.
- Documentation is updated.
- Build succeeds.
- Relevant tests pass.
