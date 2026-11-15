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
- Default, all predefined, and custom late-fee amounts.
- Main-to-optional result handoff, optional recalculation, and state independence.
- The C API contract used by WebAssembly.

## Browser checks

The browser app is built with Emscripten from the same C++ library. UI checks should confirm:

- The optional card starts collapsed.
- Minimum/Full/Custom inputs recalculate the main result.
- Late-fee changes recalculate only Scenario B.
- Invalid inputs display errors and clear stale result values.
- A 390px mobile viewport has no horizontal overflow.

The current implementation has been exercised in the integrated browser at a 390px viewport. Cross-browser and full accessibility audits remain release tasks.