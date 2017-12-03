/*
 * frc-leds
 * 
 * This sketch enables easy creation of visual lighting effects on FRC robots.
 * 
 * The arduino expects to receive a serial command like "1,177,55,7"
 * The first parameter is the desired mode number
 * The remaining parameters are expected to be arguments to indicate some information visually
 * 
 */

#include <elapsedMillis.h>

#define NUMBEROFMODES 3
typedef enum {MODE_ZERO, MODE_ONE, MODE_TWO} led_mode;
const led_mode mode_numbers[] = {MODE_ZERO, MODE_ONE, MODE_TWO};
int current_mode_index = 0;
led_mode current_mode = mode_numbers[current_mode_index];

#define SERIALINPUTBUFFERSIZE 64
char serialInputBuffer[SERIALINPUTBUFFERSIZE]; // hold incoming data
int serialInputBufferIndex = 0;
boolean serialInputComplete = false; // whether the input is complete

#define MAXSERIALINPUTPARAMS 4
int serialInputParams[MAXSERIALINPUTPARAMS];

elapsedMillis blink_timer;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  blink_timer = 0;
}

void loop() {
  serviceSerialInput();
  serviceMode();
}

void switchMode(int new_mode_index, int arguments[]) {
  if ((new_mode_index < NUMBEROFMODES) && (new_mode_index > 0)) {
    
    Serial.print("Old mode was: ");
    Serial.println(current_mode_index);
    
    current_mode_index = new_mode_index;
    current_mode = mode_numbers[current_mode_index];
    
    Serial.print("Switched to mode: ");
    Serial.println(current_mode_index);
    Serial.print("Arguments: ");
    for (int i = 0; i <= sizeof(arguments); i++) {
      Serial.print(arguments[i]);
      if (i < sizeof(arguments)) {
        Serial.print(", ");
      } else {
        Serial.println();
      }
    }
  
  } else {
    Serial.println("Invalid mode requested.");
  }
}

void serviceMode() {
  switch (current_mode) {
    default:
    case MODE_ONE:
      doModeOne();
      break;
    case MODE_TWO:
      doModeTwo();
      break;
  }
}

void doModeOne() {
  if (blink_timer > 1000) {
    blink_timer = 0;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void doModeTwo() {
  if (blink_timer > 3000) {
    blink_timer = 0;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void serviceSerialInput() {
  if (serialInputComplete) {
    splitCommand();

    int arguments[MAXSERIALINPUTPARAMS - 1];
    for (int i = 0; i < MAXSERIALINPUTPARAMS - 1; i++) {
      arguments[i] = serialInputParams[i+1];
    }
    switchMode(serialInputParams[0], arguments);
  
    memset(serialInputBuffer, 0, sizeof serialInputBuffer); // resets input buffer to 0's
  
    serialInputComplete = false;
  }
}

void splitCommand() {
  char* bufferPointer;
  bufferPointer = strtok(serialInputBuffer, ",");
  int serialInputParamsIndex = 0;
  
  while (bufferPointer != NULL)
  {
    serialInputParams[serialInputParamsIndex++] = atoi(bufferPointer);
    bufferPointer = strtok(NULL, ",");
    
    if (serialInputParamsIndex >= MAXSERIALINPUTPARAMS) {
      break;
    }
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    serialInputBuffer[serialInputBufferIndex] = Serial.read();
    
    if (serialInputBuffer[serialInputBufferIndex] == '\n') {
      serialInputComplete = true;
      serialInputBufferIndex = 0;
    } else {
      serialInputBufferIndex++;
      if (serialInputBufferIndex >= SERIALINPUTBUFFERSIZE) {
        serialInputBufferIndex = SERIALINPUTBUFFERSIZE - 1; // avoid buffer overflows
      }
    }
  }
}



