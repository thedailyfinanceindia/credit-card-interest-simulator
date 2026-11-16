# Credit Card Interest Simulator

An educational calculator for Indian credit-card users, published by Daily Finance India. It demonstrates an illustrative payment scenario and an optional missed-next-payment scenario. Results are not issuer statements or financial advice.

## What it calculates

### Main calculator

- Enter a statement/outstanding balance and annual interest rate.
- Choose Minimum, Full, or Custom payment.
- Minimum uses an editable 5% illustrative assumption by default; it is not a universal bank rule.
- The illustrative interest period defaults to 45 days and can be changed.
- Review payment, remaining principal, interest, assumed GST on interest, and estimated next balance.

### Optional missed-payment scenario

- The card is collapsed by default and starts from the main calculator's estimated next balance.
- Choose ₹500, ₹750, ₹1,200, ₹1,300, or a custom late fee.
- Review interest, assumed GST on interest and fee, total additional charges, and the final estimate.
- Changing the fee does not modify the main scenario.

### Disclosed calculation assumptions

The simplified main scenario treats the entered balance as eligible principal, subtracts the selected payment, and applies simple interest to the unpaid principal for the selected period (45 days by default) using a 365-day basis. Each interest/tax line is rounded half up to paise. This is not a transaction-date billing model: actual interest duration and balances can depend on transaction dates, payment dates and allocations, and issuer terms. Repeated carried balances may accrue interest over multiple billing cycles; this simulator does not model recurring monthly compounding. The model also excludes prior accrued/unbilled interest. The 18% GST rate is an illustrative assumption, not a universal tax conclusion.

The missed-payment golden fixture assumes the entire supplied starting balance is interest-bearing. In the interactive flow, Scenario B instead uses Scenario A's unpaid principal as the interest-bearing amount and its estimated next balance as the starting total, so it does not charge new interest on the prior GST line.

See [docs/03_FINANCIAL_CALCULATION_SPEC.md](docs/03_FINANCIAL_CALCULATION_SPEC.md), [docs/04_REGULATORY_RULES.md](docs/04_REGULATORY_RULES.md), and [docs/11_DECISIONS_LOG.md](docs/11_DECISIONS_LOG.md) for formulas, regulatory caveats, and decisions.

## Build and test the C++ engine

Requirements: CMake 3.20+ and a C++20 compiler. On Windows with Visual Studio 2022:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The calculation library has no third-party dependencies. Tests use CTest and a small in-repository test runner.

## CI and verified results

The GitHub Actions workflow runs the C++20 CMake configure, build, and CTest suite on pushes to `develop` and pull requests targeting `develop`. It has read-only repository contents permission and does not deploy. A GitHub-hosted run succeeded on `develop` on 16 November 2026.

Local verification on the current implementation:

- Native Visual Studio Debug build succeeded; CTest passed (1/1 target, 15/15 test groups).
- The 45-day integrated regression expects ₹5,270.55 interest, ₹948.70 GST and a ₹1,01,219.25 main balance. Scenario B reuses its ₹95,000.00 unpaid principal and selected 45-day period; its expected interest is ₹5,270.55, GST on interest ₹948.70, fee ₹750.00, GST on fee ₹135.00, additional charges ₹7,104.25, and final balance ₹1,08,323.50. These values are independently asserted and separate from the 30-day standalone golden fixture.
- The WebAssembly `simulator_web` build succeeded. `simulator.js` and `simulator.wasm` were produced in `build-wasm/site/` and copied to `web/`; generated copies are Git-ignored. The new editable period control has not yet been browser-tested.
- Earlier browser checks covered Minimum, Full, and Custom payments; the initially collapsed optional card; fee updates without changing the main result; invalid-input behavior; and mobile widths. Those checks predate the new period control.

## Build and run the browser app

The presentation is static HTML/CSS/JavaScript; all financial calculations and validation run in the C++ WebAssembly module. Install CMake, Ninja, Python, and the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html). If Emscripten is installed under `%USERPROFILE%\emsdk`, build the browser app:

```powershell
cmake -S . -B build-wasm -G Ninja `
  -DCMAKE_TOOLCHAIN_FILE="$env:USERPROFILE\emsdk\upstream\emscripten\cmake\Modules\Platform\Emscripten.cmake" `
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-wasm --target simulator_web
```

The build places the generated `simulator.js` and `simulator.wasm` beside the page in `web/` (they are ignored by Git). In VS Code, open `web/index.html` and click **Go Live**. The workspace Live Server root is configured to `web/`. Serve the page over HTTP; opening `index.html` directly does not load WebAssembly reliably.

Alternatively, run `python -m http.server 8000 --directory web` and open `http://localhost:8000`.

## GitHub Pages deployment

The Pages workflow builds the browser application from `web/` and uploads only `build-wasm/site/`; it deploys on pushes to `main`. The C++ CI workflow remains independent and runs on pushes and pull requests targeting `develop`. In the repository's **Settings → Pages → Build and deployment**, set **Source** to **GitHub Actions**. The legacy branch-root source can publish `README.md` and can race with the workflow deployment. The repository setting must be updated before the new `main` deployment can be verified.

## Project status

The core illustrative calculation engine and responsive browser MVP are implemented. Automated tests cover arithmetic, validation, the fixture, both scenarios, fee options, and scenario independence. Regulatory/GST review, cross-browser/accessibility review, and release readiness remain outstanding; see [docs/10_ROADMAP.md](docs/10_ROADMAP.md).
