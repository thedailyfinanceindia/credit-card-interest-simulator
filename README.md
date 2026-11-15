# Credit Card Interest Simulator

An educational calculator for Indian credit-card users, published by Daily Finance India. It demonstrates an illustrative payment scenario and an optional missed-next-payment scenario. Results are not issuer statements or financial advice.

## What it calculates

### Main calculator

- Enter a statement/outstanding balance and annual interest rate.
- Choose Minimum, Full, or Custom payment.
- Minimum uses an editable 5% illustrative assumption by default; it is not a universal bank rule.
- Review payment, remaining principal, interest, assumed GST on interest, and estimated next balance.

### Optional missed-payment scenario

- The card is collapsed by default and starts from the main calculator's estimated next balance.
- Choose ₹500, ₹750, ₹1,200, ₹1,300, or a custom late fee.
- Review interest, assumed GST on interest and fee, total additional charges, and the final estimate.
- Changing the fee does not modify the main scenario.

### Disclosed calculation assumptions

The simplified main scenario treats the entered balance as eligible principal, subtracts the selected payment, and estimates interest on the unpaid principal for 30 days using a 365-day basis. Each interest/tax line is rounded half up to paise. The model does not reconstruct issuer billing, transaction dates, payment allocation, interest-free-period rules, or prior accrued/unbilled interest. The 18% GST rate is an illustrative input from the supplied test scenario, not a universal tax conclusion.

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

## Project status

The core illustrative calculation engine and responsive browser MVP are implemented. Automated tests cover arithmetic, validation, the fixture, both scenarios, fee options, and scenario independence. Regulatory/GST review, cross-browser/accessibility review, and release readiness remain outstanding; see [docs/10_ROADMAP.md](docs/10_ROADMAP.md).
