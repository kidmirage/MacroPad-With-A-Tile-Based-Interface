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


/*****
* Standard Key Codes.
******
KEY_RESERVED               KEY_ENTER                   KEY_PAGE_DOWN                            
KEY_ERROR_ROLLOVER         KEY_RETURN                  KEY_RIGHT_ARROW                          
KEY_POST_FAIL              KEY_ESC                     KEY_LEFT_ARROW                           
KEY_ERROR_UNDEFINED        KEY_BACKSPACE               KEY_DOWN_ARROW                           
KEY_A                      KEY_TAB                     KEY_UP_ARROW                             
KEY_B                      KEY_SPACE                   KEY_RIGHT                                
KEY_C                      KEY_MINUS                   KEY_LEFT                                 
KEY_D                      KEY_EQUAL                   KEY_DOWN                                 
KEY_E                      KEY_LEFT_BRACE              KEY_UP                                   
KEY_F                      KEY_RIGHT_BRACE             KEY_NUM_LOCK                             
KEY_G                      KEY_BACKSLASH               KEYPAD_DIVIDE                            
KEY_H                      KEY_NON_US_NUM              KEYPAD_MULTIPLY                          
KEY_I                      KEY_SEMICOLON               KEYPAD_SUBTRACT                          
KEY_J                      KEY_QUOTE                   KEYPAD_ADD                               
KEY_K                      KEY_TILDE                   KEYPAD_ENTER                             
KEY_L                      KEY_COMMA                   KEYPAD_1                                 
KEY_M                      KEY_PERIOD                  KEYPAD_2                                 
KEY_N                      KEY_SLASH                   KEYPAD_3                                 
KEY_O                      KEY_CAPS_LOCK               KEYPAD_4                                 
KEY_P                      KEY_F1                      KEYPAD_5                                 
KEY_Q                      KEY_F2                      KEYPAD_6                                 
KEY_R                      KEY_F3                      KEYPAD_7                                 
KEY_S                      KEY_F4                      KEYPAD_8                                 
KEY_T                      KEY_F5                      KEYPAD_9                                 
KEY_U                      KEY_F6                      KEYPAD_0                                 
KEY_V                      KEY_F7                      KEYPAD_DOT                               
KEY_W                      KEY_F8                      KEY_NON_US                               
KEY_X                      KEY_F9                      KEY_APPLICATION                          
KEY_Y                      KEY_F10                     KEY_MENU                                 
KEY_Z                      KEY_F11                                            
KEY_1                      KEY_F12                                            
KEY_2                      KEY_PRINT                                          
KEY_3                      KEY_PRINTSCREEN                                    
KEY_4                      KEY_SCROLL_LOCK                                    
KEY_5                      KEY_PAUSE                                          
KEY_6                      KEY_INSERT                                         
KEY_7                      KEY_HOME                                           
KEY_8                      KEY_PAGE_UP                                        
KEY_9                      KEY_DELETE                                         
KEY_0                      KEY_END 

You can add any of these codes to the keyboard lists below for use in macro definitions.
In the keyboardKeyNames table add the code as a string delimited by "".
In the keyboardKeyCodes table add the code as is without quotes.
Don't forget the commas at the end of each entry.
It is IMPORTANT to add the name and code in the same position in the list.
*****/

String keyboardKeyNames[] = {
  "KEY_LEFT_CTRL",
  "KEY_UP_ARROW",
  "KEY_DOWN_ARROW",
  "KEY_LEFT_ARROW",
  "KEY_RIGHT_ARROW"
};

KeyboardKeycode keyboardKeyCodes[] {
  KEY_LEFT_CTRL,
  KEY_UP_ARROW,
  KEY_DOWN_ARROW,
  KEY_LEFT_ARROW,
  KEY_RIGHT_ARROW
};

/******
* Media Key Codes.
*******
MEDIA_RECORD 		   MEDIA_VOLUME_MUTE	
MEDIA_FAST_FORWARD	   MEDIA_VOL_MUTE		
MEDIA_REWIND		   MEDIA_VOLUME_UP		
MEDIA_NEXT		   MEDIA_VOL_UP		
MEDIA_PREVIOUS		   MEDIA_VOLUME_DOWN	
MEDIA_PREV		   MEDIA_VOL_DOWN		
MEDIA_STOP		
MEDIA_PLAY_PAUSE	
MEDIA_PAUSE		

You can add any of these codes to the media lists below for use in macro definitions.
In the consumerKeyNames table add the code as a string delimited by "".
In the consumerKeyCodes table add the code as is without quotes.
Don't forget the commas at the end of each entry.
It is IMPORTANT to add the name and code in the same position in the list.
*****/

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

/*****
* Define the macros here.
*****

The macro string is split into "tokens" at the commas and the tokens are processed from left to right with the following rules:

1) If the token is  "PRESS", a flag is set to to indicate that all subsequent characters (c) will be sent with the
   Keyboard.press(c) function. If the press token is not set (default at start of parsing), characters will be sent with
   Keyboard.write(c).

2) If the token starts with "KEY_", then the token is used to lookup a keycode in the keyboardKeyCodes table. That keycode is
   sent to the PC via the Ketboard press or write functions depending on the press flag.

3) If the token starts with "MEDIA_", then the token is used to lookup a keycode in the consumerKeyCodes table. That keycode is
   sent to the PC via Consumer.press(c) or Consumer.write(c) functions depending on the press flag. 

4) If the token is "RELEASE", then a release flag is set. The next character processed (c) will be sent to the PC with a
   Keyboard.release(c) function and the release flag will be cleared.

5) If the token is "RELEASE_ALL", then the Keyboard.releaseAll() function will be executed and the release flag cleared.

6) If the token is a hex digit of the form "0xnn" exactly where nn is a valid hex number, then the value based on that
   number will be sent to the PC via the Keyboard press or write functions depending on the press flag.

7) If the token is not recognized as one of the "key words" above, then it’s assumed to be an ASCII character or a string.
   If the token is a single character it will  be sent to the PC via the Keyboard press or write functions depending on the 
   press flag. If it's a string (s) it will be sent to the PC with a Keyboard.print(s) call. (NOTE there is no corresponding 
   Keyboard.println(s) call in this code because the user can simply add a \n to the end of the string if that is what they want.) 

It is IMPORTANT to add the macro string into the list at the line number corresponding to Tile number ypu want to map it to.
*****/
String macros[] = {
  // 0 to 9
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
  // 10 to 19
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

// Variables used to manage key auto repeat.
bool repeatEnabled = false;
bool firstRepeat = false;
unsigned long repeatTime = 0L;
int repeatMacro = 0;

// Enable repeat for the current macro.
static void enableRepeat(int macro) {
  repeatEnabled = true;
  firstRepeat = true;
  repeatMacro = macro;
  repeatTime = millis();
}

// See if the latest macro should be sent again.
static void checkRepeat() {
  unsigned long newTime = millis();
  if (repeatEnabled) {
    if (firstRepeat && (millis() - repeatTime) > 1000) {
      sendMacro(repeatMacro);
      firstRepeat = false;
      repeatTime = newTime;
    } else if ((millis() - repeatTime) > 100) {
      sendMacro(repeatMacro);
      repeatTime = newTime;
    }
  }
}

// Stop the key repeat.
static void stopRepeat() {
  repeatEnabled = false;
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
    enableRepeat(macro);
  } else {
    stopRepeat();
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

void setup() {
  // Setup serial output.
  Serial.begin(115200);
  delay(1000);
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
  // List the initial sensor values. Only run with no tiles in.
  for (int i = 0; i < 16; i++) {
    sensorValue = analogRead(sensorPin);
    Serial.print(sensorValue);
    Serial.print(" ");
  }
  Serial.println();
  ****/

  // Initialize the keyboard.
  Consumer.begin();
}

void loop() {
  pollButtons();  // Poll for buttons presses every loop.
  checkRepeat();  // See if the last macro needs repeating.
  delay(10);
}
