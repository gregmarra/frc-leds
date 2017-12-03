char inputString[64];         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int index = 0;
int parts[4];
int partsindex = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);

    splitString();

    Serial.println("done");
    
    // clear the string:
    for (int i=0; i<64; i++) {
      inputString[i] = 0;
    }
    stringComplete = false;
  }
}

void splitString() {
  char* stringPtr;
  stringPtr = strtok(inputString, ",");
  partsindex = 0;
  while (stringPtr != NULL)
  {
    parts[partsindex] = atoi(stringPtr);
    Serial.println(parts[partsindex]);
    partsindex++;
    stringPtr = strtok(NULL, ",");
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    inputString[index] = Serial.read();;
    if (inputString[index] == '\n') {
      stringComplete = true;
    }
    index++;
  }
}

