#include <HID-Project.h>
#include <debounce.h>

// Analog input for the hall effect sensor selected by the multiplexer chip.
static constexpr int sensorPin = A1;  // Read the analog value of the sensor selected by the multiplexer chip.

// Output pins used to select one of the sixteen hall effect sensors via the multiplexer chip.
static constexpr int S0 = 15;
static constexpr int S1 = 14;
static constexpr int S2 = 16;
static constexpr int S3 = 10;

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
static int midValues[] = { 526, 524, 527, 524, 528, 526, 525, 527, 526, 526, 528, 527, 524, 526, 527, 525 };

// Individual sensor calibration values.
static int calValues[16][8] = {
  { 136, 72, 54, 37, -123, -87, -56, -37 },
  { 136, 75, 57, 38, -128, -91, -58, -39 },
  { 135, 74, 55, 37, -125, -90, -57, -38 },
  { 135, 69, 53, 35, -119, -85, -53, -36 },
  { 137, 75, 56, 38, -127, -86, -57, -38 },
  { 134, 71, 54, 36, -120, -83, -54, -36 },
  { 130, 75, 57, 37, -128, -83, -58, -40 },
  { 136, 72, 55, 37, -124, -83, -55, -37 },
  { 139, 74, 56, 38, -127, -85, -58, -39 },
  { 135, 73, 55, 37, -125, -85, -56, -38 },
  { 139, 74, 56, 38, -127, -89, -58, -39 },
  { 135, 73, 55, 37, -125, -86, -56, -38 },
  { 137, 73, 55, 37, -123, -85, -56, -36 },
  { 133, 72, 55, 37, -124, -85, -56, -37 },
  { 135, 74, 56, 37, -127, -89, -61, -38 },
  { 136, 73, 55, 37, -124, -85, -60, -38 }
};

// Fetch the sensor value here.
int sensorValue = 0;

// Define the delimiter character for the macro definitions.
static constexpr char *delimiter = ",";

// Add any
String keyboardKeyNames[] = {
  "KEY_LEFT_CTRL",
  "KEY_UP_ARROW",
  "KEY_DOWN_ARROW",
  "KEY_LEFT_ARROW",
  "KEY_RIGHT_ARROW"
};

KeyboardKeycode keyboardKeyCodes[]{
  KEY_LEFT_CTRL,
  KEY_UP_ARROW,
  KEY_DOWN_ARROW,
  KEY_LEFT_ARROW,
  KEY_RIGHT_ARROW
};

String consumerKeyNames[] = {
  "MEDIA_PLAY_PAUSE",
  "MEDIA_VOL_MUTE",
  "MEDIA_VOLUME_UP",
  "MEDIA_VOLUME_DOWN"
};

ConsumerKeycode consumerKeyCodes[]{
  MEDIA_PLAY_PAUSE,
  MEDIA_VOL_MUTE,
  MEDIA_VOLUME_UP,
  MEDIA_VOLUME_DOWN
};

// Define the macros here.
String macros[] = {
  "PRESS,KEY_LEFT_CTRL,a,RELEASE_ALL",  // select
  "PRESS,KEY_LEFT_CTRL,c,RELEASE_ALL",  // copy
  "PRESS,KEY_LEFT_CTRL,x,RELEASE_ALL",  // cut
  "PRESS,KEY_LEFT_CTRL,v,RELEASE_ALL",  // paste
  "KEY_UP_ARROW",                       // up
  "KEY_LEFT_ARROW",                     // left
  "KEY_RIGHT_ARROW",                    // right
  "KEY_DOWN_ARROW",                     // down
  "",                                   // Skip for now.
  "",                                   // Skip for now.
  "MEDIA_VOLUME_DOWN",                  // louder 
  "MEDIA_VOLUME_UP",                    // softer
  "MEDIA_VOL_MUTE",                     // mute
  "MEDIA_PLAY_PAUSE",                   // pause/play
  ""
};

// Given a sensor and a value find the numeric code.
static int findCode(int sensor, int value) {
  for (int i = 0; i < 8; i++) {
    if (abs(value - calValues[sensor][i]) < 13) {
      return i;
    }
  }
  return -1;
}

// Fetch the macro string based on the index passed, parse the macro string sending the key codes.
static void sendMacro(int macroNumber) {
  // Keep track of whether we are in PRESS or WRITE mode.
  bool pressMode = false;

  // Keep track of whether a RELEASE tag was encountered.
  bool releaseMode = false;

  // Get the macro string and convert it to a character array.
  String macro = macros[macroNumber];
  int length = macro.length() + 1;
  char buffer[length];
  macro.toCharArray(buffer, length);

  // Parse the string for tokens.
  char *ptr = NULL;
  ptr = strtok(buffer, delimiter);  // First token.
  while (ptr != NULL) {
    String token = String(ptr);
    int toklen = token.length();
    if (token == "PRESS") {
      // Initiate Press mode. Send all subsequent characters as presses.
      pressMode = true;
    } else if (token == "RELEASE") {
      // Initiate Release mode. Next character will be sent as a release.
      releaseMode = true;
    } else if (token == "RELEASE_ALL") {
      // Send a releaseAll.
      Keyboard.releaseAll();
      pressMode = false;
    } else if (token.startsWith("KEY")) {
      // Send one of the special standard key codes found in Keyboard.h.
      for (int i = 0; i < sizeof(keyboardKeyNames); i++) {
        if (token == keyboardKeyNames[i]) {
          KeyboardKeycode code = keyboardKeyCodes[i];
          if (pressMode) {
            Keyboard.press(code);
          } else {
            Keyboard.write(code);
          }
        }
      }
    } else if (token.startsWith("MEDIA")) {
      // Send one of the special media key codes found in Keyboard.h.
      for (int i = 0; i < sizeof(consumerKeyNames); i++) {
        if (token == consumerKeyNames[i]) {
          ConsumerKeycode code = consumerKeyCodes[i];
          if (pressMode) {
            Consumer.press(code);
          } else {
            Consumer.write(code);
          }
        }
      }
    } else if (toklen == 4 && token.startsWith("0x")) {
      // Send a character expressed as a hex number (0xnn).
      char keyArray[5];
      token.toCharArray(keyArray, length);
      int keychar = strtol(keyArray, NULL, 16);
      if (pressMode) {
        Keyboard.press(keychar);
      } else {
        Keyboard.write(keychar);
      }
    } else {
      // Assume a character or a string.
      if (toklen > 1) {
        // Print the string to the keyboard.
        Keyboard.print(token);
      } else {
        // Send the single character.
        if (releaseMode) {
          // Character was immediately after a RELEASE tag.
          Keyboard.release(token[0]);
          releaseMode = false;
        } else if (pressMode) {
          Keyboard.press(token[0]);
        } else {
          Keyboard.write(token[0]);
        }
      }
    }
    // Next token.
    ptr = strtok(NULL, delimiter);  // Next token.
  }
}

// Button presses that have been debounced will be handled here.
static void buttonHandler(uint8_t btnId, uint8_t btnState) {
  int leftSensor, rightSensor;
  if (btnState == BTN_PRESSED) {
    switch (btnId) {
      case BT1:
        leftSensor = 10;
        rightSensor = 11;
        break;
      case BT2:
        leftSensor = 5;
        rightSensor = 4;
        break;
      case BT3:
        leftSensor = 8;
        rightSensor = 9;
        break;
      case BT4:
        leftSensor = 7;
        rightSensor = 6;
        break;
      case BT5:
        leftSensor = 2;
        rightSensor = 3;
        break;
      case BT6:
        leftSensor = 13;
        rightSensor = 12;
        break;
      case BT7:
        leftSensor = 0;
        rightSensor = 1;
        break;
      case BT8:
        leftSensor = 15;
        rightSensor = 14;
        break;
      default:
        break;
    }
    // Determine what macro to send.
    setSensorForInput(leftSensor);
    sensorValue = analogRead(sensorPin) - midValues[leftSensor];
    int macro = findCode(leftSensor, sensorValue) * 10;
    setSensorForInput(rightSensor);
    sensorValue = analogRead(sensorPin) - midValues[rightSensor];
    macro = macro + findCode(rightSensor, sensorValue);

    // Send the macro.
    sendMacro(macro);
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

void setup() {
  // Setup serial output.
  Serial.begin(115200);
  while (!Serial) {
    // Wait for Serial to initialize.
    delay(10);
  }
  Serial.println("Tile Based MacroPad!");

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

  /****
  // List the initial sensor values.
  for (int i = 0; i < 16; i++) {
    sensorValue = analogRead(sensorPin);
    Serial.print(sensorValue);
    Serial.print(" ");
  }
  Serial.println();
  ****/

  // Initialize the keyboard.
  Consumer.begin();  // For media keys.
}

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

void loop() {
  pollButtons();  // Poll your buttons every loop.
  delay(10);
}

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
