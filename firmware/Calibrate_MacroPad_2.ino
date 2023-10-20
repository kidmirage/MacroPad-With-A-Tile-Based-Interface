#include <debounce.h>

// Analog input for the hall effect sensor selected by the multiplexer chip.
static constexpr int sensorPin = A1;  // Read the analog value of the sensor selected by the multiplexer chip.

// Output pins used to select one of the sixteen hall effect sensors via the multiplexer chip.
static constexpr int S0 = 15;
static constexpr int S1 = 14;
static constexpr int S2 = 10;
static constexpr int S3 = 16;

// Input pins used to read the eight key switches.
static constexpr int BT1 = 2;
static constexpr int BT2 = 3;
static constexpr int BT3 = 4;
static constexpr int BT4 = 5;
static constexpr int BT5 = 6;
static constexpr int BT6 = 7;
static constexpr int BT7 = 8;
static constexpr int BT8 = 9;

// Sensor initial values.
static int midValues[16];

// Individual sensor calibration values.
static int calValues[16][10] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Fetch the sensor value here.
int sensorValue = 0;

// Define the tile value that is being calibrated.
int valuePos = -1;

// Given a sensor number, set the S0-S3 pins to select it for input.
void setSensorForInput(int sensorNumber) {
  if (sensorNumber & 1) {
    digitalWrite(S0, HIGH);
  } else {
    digitalWrite(S0, LOW);
  }
  if (sensorNumber & 2) {
    digitalWrite(S1, HIGH);
  } else {
    digitalWrite(S1, LOW);
  }
  if (sensorNumber & 4) {
    digitalWrite(S2, HIGH);
  } else {
    digitalWrite(S2, LOW);
  }
  if (sensorNumber & 8) {
    digitalWrite(S3, HIGH);
  } else {
    digitalWrite(S3, LOW);
  }
}

// Button presses that have been debounced will be handled here.
static void buttonHandler(uint8_t btnId, uint8_t btnState) {
  int leftSensor, rightSensor;
  if (btnState == BTN_PRESSED) {
    switch (btnId) {
      case BT1:
        leftSensor = 0;
        rightSensor = 1;
        break;
      case BT2:
        leftSensor = 8;
        rightSensor = 9;
        break;
      case BT3:
        leftSensor = 2;
        rightSensor = 3;
        break;
      case BT4:
        leftSensor = 10;
        rightSensor = 11;
        break;
      case BT5:
        leftSensor = 4;
        rightSensor = 5;
        break;
      case BT6:
        leftSensor = 12;
        rightSensor = 13;
        break;
      case BT7:
        leftSensor = 6;
        rightSensor = 7;
        break;
      case BT8:
        leftSensor = 14;
        rightSensor = 15;
        break;
      default:
        break;
    }
    
    // Determine the sensor values.
    setSensorForInput(leftSensor);
    delay(10);
    sensorValue = analogRead(sensorPin) - midValues[leftSensor];
    calValues[leftSensor][valuePos] = sensorValue;
    Serial.print(leftSensor);
    Serial.print(" ");
    Serial.print(sensorValue);
    Serial.print(" ");
    
    setSensorForInput(rightSensor);
    delay(10);
    sensorValue = analogRead(sensorPin) - midValues[rightSensor];
    calValues[rightSensor][valuePos] = sensorValue;
    Serial.print(rightSensor);
    Serial.print(" ");
    Serial.println(sensorValue);
  } 
}

// Define the buttons for debounce.
static Button Button1(BT1, buttonHandler);
static Button Button2(BT2, buttonHandler);
static Button Button3(BT3, buttonHandler);
static Button Button4(BT4, buttonHandler);
static Button Button5(BT5, buttonHandler);
static Button Button6(BT6, buttonHandler);
static Button Button7(BT7, buttonHandler);
static Button Button8(BT8, buttonHandler);

// Check for button changes on each loop.
static void pollButtons() {
  // update() will call buttonHandler() if the pin transitions to a new state and stays there
  // for multiple reads over 25+ ms.
  Button1.update(digitalRead(BT1));
  Button2.update(digitalRead(BT2));
  Button3.update(digitalRead(BT3));
  Button4.update(digitalRead(BT4));
  Button5.update(digitalRead(BT5));
  Button6.update(digitalRead(BT6));
  Button7.update(digitalRead(BT7));
  Button8.update(digitalRead(BT8));
}

// Check for button presses.
void loop() {
}

void setup() {
  // Setup serial output.
  Serial.begin(115200);
  while (!Serial) {
    delay(1000);
  }
  Serial.println("Calibrate Tile Based MacroPad!");

  // Initialize pins.
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
  pinMode(BT4, INPUT_PULLUP);
  pinMode(BT5, INPUT_PULLUP);
  pinMode(BT6, INPUT_PULLUP);
  pinMode(BT7, INPUT_PULLUP);
  pinMode(BT8, INPUT_PULLUP);

  // List the initial sensor values. Only run with no tiles in.
  Serial.println();
  Serial.print("static int midValues[] = {");
  for (int i = 0; i < 16; i++) {
    setSensorForInput(i);
    delay(100);
    sensorValue = analogRead(sensorPin);
    Serial.print(sensorValue);
    midValues[i] = sensorValue;
    if (i != 15) {
      Serial.print(",");
    }
  }
  Serial.println("};");
  Serial.println();

  // Poll the user to insert calibration tiles until the whole calValues table is filled.
  for (valuePos = 0; valuePos < 10; valuePos++) {
    Serial.println();
    Serial.print("Put the ");
    Serial.print(valuePos);
    Serial.println(" tile into each of the eight slots and press the corresponding button.");
    Serial.println();

    // Wait till all 10 values are collected for this sensor.
    int count = 0;
    while(count < 16) {
      count = 0;
      for (int i = 0; i < 16; i++) {
        if (calValues[i][valuePos] != 0) {
          count++;
        }
      }
      pollButtons();
    }
  }

  // Dump the calibration value table.
  Serial.println();
  Serial.println("static int calValues[16][10] = {");
  for (int sensor = 0; sensor < 16; sensor++) {
    Serial.print("  {");
    for (int i = 0; i < 10; i++) {
      Serial.print(calValues[sensor][i]);
      if (i < 9) {
        Serial.print(",");
      }
    }
    Serial.print("}");
    if (sensor < 15) {
      Serial.println(",");
    }
  }
  Serial.println();
  Serial.println("};");
}
