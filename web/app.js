"use strict";

const mainForm = document.querySelector("#main-form");
const statementAmount = document.querySelector("#statement-amount");
const annualRate = document.querySelector("#annual-rate");
const minimumRate = document.querySelector("#minimum-rate");
const customPayment = document.querySelector("#custom-payment");
const minimumRateField = document.querySelector("#minimum-rate-field");
const customPaymentField = document.querySelector("#custom-payment-field");
const mainError = document.querySelector("#main-error");
const missedScenario = document.querySelector("#missed-scenario");
const lateFee = document.querySelector("#late-fee");
const customFee = document.querySelector("#custom-fee");
const customFeeField = document.querySelector("#custom-fee-field");
const missedError = document.querySelector("#missed-error");
const engineStatus = document.querySelector("#engine-status");

let engine;
let currentMainResult;

function selectedPaymentMode() {
  return Number(mainForm.querySelector('input[name="payment-mode"]:checked').value);
}

function setCurrency(id, amount) {
  document.querySelector(`#${id}`).textContent = `₹${amount}`;
}

function showError(element, message) {
  element.textContent = message;
  element.hidden = !message;
}

function invokeEngine(functionName, values) {
  const allocatedPointers = [];
  const argumentsForEngine = values.map(value => {
    if (typeof value === "number") {
      return value;
    }
    const text = value ?? "";
    const length = engine.lengthBytesUTF8(text) + 1;
    const pointer = engine._malloc(length);
    engine.stringToUTF8(text, pointer, length);
    allocatedPointers.push(pointer);
    return pointer;
  });
  try {
    const resultPointer = engine[`_${functionName}`](...argumentsForEngine);
    return JSON.parse(engine.UTF8ToString(resultPointer));
  } finally {
    allocatedPointers.forEach(pointer => engine._free(pointer));
  }
}

function readMainResult() {
  return invokeEngine("ccsim_main_result", [
    statementAmount.value,
    annualRate.value,
    selectedPaymentMode(),
    customPayment.value,
    minimumRate.value
  ]);
}

function renderMain(result) {
  currentMainResult = result.ok ? result : null;
  if (!result.ok) {
    showError(mainError, result.error);
    ["main-next-balance", "main-payment", "main-remaining", "main-interest", "main-gst"]
      .forEach(id => document.querySelector(`#${id}`).textContent = "—");
    renderMissed();
    return;
  }

  showError(mainError, "");
  setCurrency("main-next-balance", result.estimatedNextBalance);
  setCurrency("main-payment", result.payment);
  setCurrency("main-remaining", result.remainingPrincipal);
  setCurrency("main-interest", result.interest);
  setCurrency("main-gst", result.gstOnInterest);
  if (missedScenario.open) {
    renderMissed();
  }
}

function updateMain() {
  if (!engine) {
    return;
  }
  renderMain(readMainResult());
}

function selectedLateFee() {
  if (lateFee.value !== "custom") {
    return lateFee.value;
  }
  return customFee.value;
}

function renderMissed() {
  if (!missedScenario.open || !engine) {
    return;
  }
  if (!currentMainResult) {
    showError(missedError, "Enter valid main-calculator inputs to estimate this scenario.");
    ["missed-final-balance", "missed-starting-balance", "missed-interest",
      "missed-gst-interest", "missed-late-fee", "missed-gst-fee", "missed-total"]
      .forEach(id => document.querySelector(`#${id}`).textContent = "—");
    return;
  }

  const result = invokeEngine("ccsim_missed_result", [
    currentMainResult.estimatedNextBalance,
    currentMainResult.remainingPrincipal,
    annualRate.value,
    selectedLateFee()
  ]);
  if (!result.ok) {
    showError(missedError, result.error);
    ["missed-final-balance", "missed-starting-balance", "missed-interest",
      "missed-gst-interest", "missed-late-fee", "missed-gst-fee", "missed-total"]
      .forEach(id => document.querySelector(`#${id}`).textContent = "—");
    return;
  }

  showError(missedError, "");
  setCurrency("missed-final-balance", result.estimatedNewBalance);
  setCurrency("missed-starting-balance", result.startingBalance);
  setCurrency("missed-interest", result.interest);
  setCurrency("missed-gst-interest", result.gstOnInterest);
  setCurrency("missed-late-fee", result.lateFee);
  setCurrency("missed-gst-fee", result.gstOnLateFee);
  setCurrency("missed-total", result.totalAdditionalCharges);
}

function updatePaymentFields() {
  const mode = selectedPaymentMode();
  minimumRateField.hidden = mode !== 0;
  customPaymentField.hidden = mode !== 2;
  customPayment.required = mode === 2;
  updateMain();
}

mainForm.addEventListener("input", updateMain);
mainForm.addEventListener("change", event => {
  if (event.target.name === "payment-mode") {
    updatePaymentFields();
  } else {
    updateMain();
  }
});
mainForm.addEventListener("submit", event => event.preventDefault());

missedScenario.addEventListener("toggle", () => {
  if (missedScenario.open) {
    renderMissed();
  }
});

lateFee.addEventListener("change", () => {
  customFeeField.hidden = lateFee.value !== "custom";
  showError(missedError, "");
  renderMissed();
});
customFee.addEventListener("input", renderMissed);

if (typeof createSimulatorModule !== "function") {
  engineStatus.textContent = "Calculation engine is unavailable. Build the WebAssembly app before opening this page.";
  showError(mainError, "The calculation engine did not load. Follow the build instructions in the README.");
} else {
  createSimulatorModule()
    .then(module => {
      engine = module;
      engineStatus.textContent = "Calculation engine ready.";
      updatePaymentFields();
    })
    .catch(error => {
      engineStatus.textContent = "Calculation engine failed to load.";
      showError(mainError, "The calculation engine failed to load. Rebuild the WebAssembly app and try again.");
      console.error(error);
    });
}
