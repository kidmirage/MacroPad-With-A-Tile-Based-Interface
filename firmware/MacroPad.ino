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
In the keyboardKeyCodes table ad the code as is without quotes.
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
In the consumerKeyCodes table ad the code as is without quotes.
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
   Keyboard.press(c) function. If the press token is not set (default at start of parsing), characters will ne sent with
   Keyboard.write(c).

2) If the token starts with "KEY_", then the token is used to lookup a keycode in the specialKeys table. That keycode is
   sent to the PC via press or write depending on the press flag.

3) If the token starts with "MEDIA_", then the token is used to lookup a keycode in the specialKeys table. That keycode is
   sent to the PC via special pressRaw(c) or writeRaw(c) functions depending on the press flag. 

4) If the token is "RELEASE", then a release flag is set. The next character processed (c) will be sent to the PC with a
   Keyboard.release(c) function and the release flag will be cleared.

5) If the token is "RELEASE_ALL", then the Keyboard.releaseAll() function will be executed and the release flag cleared.

6) If the token is a hex digit of the form "0xnn" exactly where nn is a valid hex number, then the value based on that
   number will be sent to the PC via press or write depending on the press flag.

7) If the token is not recognized as one of the "key words" above, then it’s assumed to be an ASCII character or a string.
   If the token is a single character it will  be sent to the PC via press or write depending on the press flag. If it's a
   string (s) it will be sent to the PC with a Keyboard.print(s) call. (NOTE there is no corresponding Keyboard.println(s)
   call because the user can simply add a \n to the end of the string if that is what they want.) 

It is IMPORTANT to add the macro string into the list at the line number corresponding to Tile number ypu want to map it to.
*****/
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
