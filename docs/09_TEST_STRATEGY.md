# Test Strategy

## Calculation tests

The project uses CTest with a small dependency-free C++ test runner. No third-party test framework is required.

On Windows with Visual Studio 2022:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

The test groups cover:

- Paise parsing, precision, formatting, arithmetic and overflow.
- Rate validation, interest and percentage rounding.
- GST line rounding and zero rates/balances.
- Minimum (editable illustrative default), Full, and Custom main payments.
- Invalid/missing custom payment, overpayment, negative/invalid inputs and zero periods.
- The approved main illustrative example and the standalone missed-payment golden fixture.
- The integrated Minimum-to-missed-payment flow with independent expectations: ₹99,146.17 main balance; ₹3,513.70 additional interest; ₹632.47 GST on interest; ₹750.00 fee; ₹135.00 GST on fee; ₹5,031.17 charges; ₹1,04,177.34 final estimate.
- Default, all predefined, and custom late-fee amounts.
- Main-to-optional result handoff, optional recalculation, and state independence.
- The C API contract used by WebAssembly.

## Browser checks

The browser app is built with Emscripten from the same C++ library. UI checks should confirm:

- The optional card starts collapsed.
- Minimum/Full/Custom inputs recalculate the main result.
- Late-fee changes recalculate only Scenario B.
- Invalid inputs display errors and clear stale result values.
- A 320px, 360px, and 390px viewport has no horizontal overflow.

These browser checks passed in the integrated browser. Cross-browser and full accessibility audits remain release tasks.

## Continuous integration

[`.github/workflows/ci.yml`](../.github/workflows/ci.yml) configures an Ubuntu C++20 CMake build and CTest run for pushes and pull requests targeting `develop`. The workflow is configured locally; its first GitHub-hosted execution remains pending. A local Linux run was unavailable because CMake is not installed in the available WSL distribution.

Record the hosted run result here after the workflow first executes.
