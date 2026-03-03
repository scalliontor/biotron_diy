# Biotron DIY Project

## 1. ESP32 Firmware Setup (`biotron_esp32_osc.ino`)

This ESP32 program reads capacitive Touch on GPIO 4 and Light intensity via an LDR on GPIO 34. The data is smoothed using an Exponential Moving Average (EMA) and sent over WiFi via UDP OSC packets to a host PC.

### Requirements
- Arduino IDE with ESP32 board support.
- Library: **OSC** by CNMAT (Install via Arduino Library Manager or download from [GitHub](https://github.com/CNMAT/OSC)).

### Configuration
Update the network credentials and target IP address at the top of the `.ino` file:
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
IPAddress outIp(192, 168, 1, 100); // IP of the computer running the Python script
```

### Wiring
- **Touch**: Connect an alligator clip to `GPIO 4` (Pin D4 / T0) and the other end to the plant leaf via aluminum foil or an ECG pad.
- **Light (LDR)**: Build a voltage divider. Connect one end of the LDR to 3.3V, the other end to `GPIO 34`. Also, connect an resistor (e.g. 10k) from `GPIO 34` to GND.
- **Grounding**: Run a wire from a `GND` pin on the ESP32 into the moist soil of the plant to reduce 50Hz/60Hz EMI.

---

## 2. Python OSC Receiver (`biotron_receiver.py`)

This Python script listens for OSC data on port 8000 and displays the smoothed incoming values.

### Installation
Make sure you have Python 3.7+ installed.
Install the required package `python-osc`:
```bash
pip install python-osc
```

### Running the Receiver
Run the script in the terminal:
```bash
python biotron_receiver.py --ip 0.0.0.0 --port 8000
```
This will listen on all interfaces for OSC messages. When the ESP32 connects and sends data, you will see a text-based "bar chart" visualization of the touch and light sensors.

## Integration with TouchDesigner
In TouchDesigner:
1. Create an `OSC In CHOP`.
2. Set the port to match the one configured in the ESP32 (e.g. `8000`).
3. You will receive two channels: `/biotron/touch` and `/biotron/light`. Map them as described in the project documentation!
