#include <TFT_eSPI.h> // Graphics library for ST7789 display
#include <SPI.h>      // SPI communication for the display

// Initialize the display object
TFT_eSPI tft = TFT_eSPI();

// ==== BUTTON AND POTENTIOMETER CONFIGURATION ====
const int nBotones = 12;            // Total number of digital trigger buttons
boolean notaActiva[nBotones];       // State tracking: True if currently pressed
boolean estadoAnterior[nBotones];   // Debounce tracking: Stores previous state to detect edge triggers

// MIDI Note numbers (C2 to B2)
byte notas[] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};

// Human-readable labels for the IPS display
String nombreNotas[] = {
  "C2", "C#2", "D2", "D#2", "E2", "F2",
  "F#2", "G2", "G#2", "A2", "A#2", "B2"
};

const int nPots = 4;                // Total number of analog potentiometers (CC controls)
int lecturas[nPots];                // Current analog reading (mapped 0-255)
int lecturasAnteriores[nPots];      // Previous reading to detect significant changes (jitter filtering)

// ==== SETUP: INITIALIZATION ====
void setup() {
  // Initialize Display
  tft.init();
  tft.setRotation(1);               // Landscape orientation
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Ready to Play");

  // MIDI Serial Communication
  // Note: 115200 baud rate is required for Hairless MIDI <-> Serial bridge
  Serial.begin(115200); 

  // Pin Configuration
  for (int i = 0; i < nBotones; i++) {
    // Buttons are connected to D30 through D41 using internal pull-up resistors
    pinMode(i + 30, INPUT_PULLUP);  
    notaActiva[i] = false;
    estadoAnterior[i] = HIGH;       // HIGH means unpressed due to INPUT_PULLUP logic
  }
}

// ==== MAIN LOOP: SCANNING AND PROCESSING ====
void loop() {
  bool updateDisplay = false;

  // 1. SCAN DIGITAL BUTTONS
  for (int i = 0; i < nBotones; i++) {
    bool estado = digitalRead(i + 30);  // Read current physical state

    // Detect Falling Edge: Button Just Pressed
    if (estado == LOW && estadoAnterior[i] == HIGH) {
      midi(0x90, notas[i], 100);       // Send Note ON (Channel 1, Velocity 100)
      notaActiva[i] = true;
      updateDisplay = true;
    }

    // Detect Rising Edge: Button Just Released
    if (estado == HIGH && estadoAnterior[i] == LOW) {
      midi(0x80, notas[i], 0);         // Send Note OFF (Channel 1)
      notaActiva[i] = false;
      updateDisplay = true;
    }

    estadoAnterior[i] = estado;        // Store state for next iteration
  }

  // 2. UPDATE UI (Visual Feedback)
  // Rendering logic: Only update screen if a change occurred to prevent loop lag
  if (updateDisplay) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Active Notes:");

    int y = 40;
    for (int i = 0; i < nBotones; i++) {
      if (notaActiva[i]) {
        tft.setCursor(10, y);
        tft.setTextColor(TFT_CYAN, TFT_BLACK);
        tft.println(nombreNotas[i]);
        y += 25;                       // Increment Y position for next note label
      }
    }
  }

  // 3. SCAN ANALOG POTENTIOMETERS (Control Change)
  for (int k = 0; k < nPots; k++) {
    // Map 10-bit ADC (0-1023) to an 8-bit range for processing
    lecturas[k] = map(analogRead(k), 0, 1023, 0, 255);
    
    // Threshold filtering: Only send MIDI if change is > 1 to avoid signal noise
    if (abs(lecturas[k] - lecturasAnteriores[k]) > 1) {
      // 0xB0 = Control Change, Channel 1. CC range is 0-127 (hence /2)
      midi(0xB0, k + 30, lecturas[k] / 2);  
      lecturasAnteriores[k] = lecturas[k];
    }
  }
}

// ==== HELPER FUNCTION: SEND MIDI OVER SERIAL ====
// Standard MIDI message structure: [Status Byte, Data1, Data2]
void midi(byte cmd, byte data1, byte data2) {
  Serial.write(cmd);   // MIDI Status Byte (Command + Channel)
  Serial.write(data1); // Data 1 (Note Number or CC Number)
  Serial.write(data2); // Data 2 (Velocity or CC Value)
}
