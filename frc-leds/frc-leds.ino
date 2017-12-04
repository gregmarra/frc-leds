/*
 * frc-leds
 * 
 * This sketch enables easy creation of visual lighting effects on FRC robots.
 * 
 * The arduino expects to receive a serial command like "1,177,55,7"
 * The first parameter is the desired mode number
 * The remaining parameters are expected to be arguments to indicate some information visually
 * 
 * Eventually, the code will allow toggling between UART and I2C serial communication.
 * UART: Expected to be used for developing and debugging with a PC
 * I2C: Expected to be used for communications with the RoboRIO
 * 
 */

#include <elapsedMillis.h>
#include <FastLED.h>

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

#define NUM_LEDS 16
#define LED_PIN 6

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  set_max_power_in_volts_and_milliamps(5, 500);
  
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  blink_timer = 0;
}

void loop() {
  serviceSerialInput();
  serviceMode();
  FastLED.show();
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

  // from https://github.com/atuline/FastLED-Demos/blob/master/rainbow_beat/rainbow_beat.ino
  uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);            // Use FastLED's fill_rainbow routine.
  FastLED.setBrightness(serialInputParams[1]);
}

void doModeTwo() {
  if (blink_timer > 3000) {
    blink_timer = 0;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  // from https://github.com/atuline/FastLED-Demos/blob/master/dot_beat/dot_beat.ino
  uint8_t bpm = 30;
  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  leds[middle] = CRGB::Purple;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Aqua;

  nscale8(leds, NUM_LEDS, 224);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);

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



