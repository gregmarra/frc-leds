
char serialInputBuffer[64];         // a String to hold incoming data
boolean serialInputComplete = false;  // whether the string is complete
int serialInputBufferIndex = 0;

#define MAXSERIALINPUTPARAMS 4
int serialInputParams[MAXSERIALINPUTPARAMS];

void setup() {
  // initialize serial:
  Serial.begin(9600);
}

void loop() {
  // print the string when a newline arrives:
  if (serialInputComplete) {
    handleSerialInput(); 
  }
}

void handleSerialInput() {
  Serial.println(serialInputBuffer);
  splitCommand();

  memset(serialInputBuffer, 0, sizeof serialInputBuffer); // resets input buffer to 0's

  serialInputComplete = false;
}

void splitCommand() {
  char* bufferPointer;
  bufferPointer = strtok(serialInputBuffer, ",");
  int serialInputParamsIndex = 0;
  
  while (bufferPointer != NULL)
  {
    
    Serial.println(atoi(bufferPointer));
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
    serialInputBuffer[serialInputBufferIndex] = Serial.read();;
    if (serialInputBuffer[serialInputBufferIndex] == '\n') {
      serialInputComplete = true;
      serialInputBufferIndex = 0;
    } else {
      serialInputBufferIndex++;
    }
  }
}

