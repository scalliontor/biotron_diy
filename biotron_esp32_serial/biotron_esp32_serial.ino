// ----------------------------------------------------------------------------
// Hardware Pins
// ----------------------------------------------------------------------------
// Capacitive Touch Pin - using GPIO4 (T0 on ESP32)
const int TOUCH_PIN = 4;
// LDR Pin (Light sensor) - using GPIO34 (ADC1_CH6)
const int LDR_PIN = 34;

// ----------------------------------------------------------------------------
// DSP Filter: Exponential Moving Average (EMA) Parameters
// ----------------------------------------------------------------------------
// Alpha handles the smoothing factor (0.0 < alpha <= 1.0)
// Higher alpha = less smoothing, more responsive. Lower alpha = more smoothing.
const float TOUCH_ALPHA = 0.2f;
const float LIGHT_ALPHA = 0.1f;

float emaTouchValue = 0.0f;
float emaLightValue = 0.0f;

// Timers for non-blocking loop
unsigned long lastProcessTime = 0;
const int processIntervalMs = 20; // 50 Hz update rate (Every 20ms)

void setup() {
  Serial.begin(115200);

  // Setup ADC pin
  pinMode(LDR_PIN, INPUT);

  // Increase touch sensitivity
  // touchSetCycles(0x1000, 0x1000) will set measurement cycles and sleep cycles higher
  touchSetCycles(0x1000, 0x1000);

  // Initial read to set baseline for EMA
  emaTouchValue = (float)touchRead(TOUCH_PIN);
  emaLightValue = (float)analogRead(LDR_PIN);

  Serial.println("Biotron Serial Mode Started");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastProcessTime >= processIntervalMs) {
    lastProcessTime = currentMillis;

    // 1. Read Raw Sensors
    uint16_t rawTouch = touchRead(TOUCH_PIN);
    uint16_t rawLight = analogRead(LDR_PIN);

    // 2. Apply Edge DSP (EMA Filter)
    emaTouchValue = (TOUCH_ALPHA * (float)rawTouch) + ((1.0f - TOUCH_ALPHA) * emaTouchValue);
    emaLightValue = (LIGHT_ALPHA * (float)rawLight) + ((1.0f - LIGHT_ALPHA) * emaLightValue);

    // 3. Print values comma-separated
    // Format: Touch,Light
    Serial.print("T:");
    Serial.print(emaTouchValue);
    Serial.print(",L:");
    Serial.println(emaLightValue);
  }
}
