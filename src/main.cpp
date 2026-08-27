#include <Arduino.h>
#include <BleKeyboard.h>

// Teclado Bluetooth (BLE HID) de 3 teclas para os prompts de permissao
// do Claude Code. Cada tecla tem um lado ligado ao GND e o outro ao
// pino GPIO (INPUT_PULLUP).
//
// Pinos (ajuste aqui se soldou em GPIOs diferentes):
constexpr uint8_t PIN_SIM        = 4; // "1" - Sim
constexpr uint8_t PIN_SIM_SEMPRE = 5; // "2" - Sim, sempre / nao perguntar novamente
constexpr uint8_t PIN_NAO        = 6; // "3" - Nao

constexpr unsigned long DEBOUNCE_MS = 80;

BleKeyboard bleKeyboard("Claude Code Keyboard", "HBR", 100);

struct Button {
  uint8_t pin;
  char key;
  bool lastRead;
  bool stableState;
  unsigned long lastChangeMs;
};

Button buttons[] = {
  {PIN_SIM,        '1', HIGH, HIGH, 0},
  {PIN_SIM_SEMPRE, '2', HIGH, HIGH, 0},
  {PIN_NAO,        '3', HIGH, HIGH, 0},
};

void setup() {
  Serial.begin(115200);

  for (auto &b : buttons) {
    pinMode(b.pin, INPUT_PULLUP);
  }

  bleKeyboard.begin();
}

void loop() {
  static unsigned long lastStatusMs = 0;
  const unsigned long now = millis();

  if (now - lastStatusMs > 2000) {
    lastStatusMs = now;
    Serial.printf("[status] BLE conectado: %s | GPIO4=%d GPIO5=%d GPIO6=%d\n",
                  bleKeyboard.isConnected() ? "sim" : "nao",
                  digitalRead(PIN_SIM), digitalRead(PIN_SIM_SEMPRE), digitalRead(PIN_NAO));
  }

  for (auto &b : buttons) {
    const bool reading = digitalRead(b.pin);

    if (reading != b.lastRead) {
      b.lastChangeMs = now;
      b.lastRead = reading;
    }

    if ((now - b.lastChangeMs) > DEBOUNCE_MS && reading != b.stableState) {
      b.stableState = reading;

      if (b.stableState == LOW) { // tecla pressionada
        Serial.printf("[tecla] GPIO%d pressionado -> '%c'\n", b.pin, b.key);
        if (bleKeyboard.isConnected()) {
          bleKeyboard.write(b.key);
        } else {
          Serial.println("[aviso] BLE nao conectado, tecla nao enviada");
        }
      }
    }
  }
}
