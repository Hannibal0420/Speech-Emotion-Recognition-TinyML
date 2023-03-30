
#include <PDM.h>
#include <TinyMLShield.h>

// PDM buffer
short sampleBuffer[256];
volatile int samplesRead;

bool record = false;
bool commandRecv = false;

void setup() {
  Serial.begin(38400);
  while (!Serial);  

  // Initialize the TinyML Shield
  initializeShield();
  pinMode(LED_BUILTIN, OUTPUT);

  PDM.onReceive(onPDMdata);
  // Initialize PDM microphone in mono mode with 16 kHz sample rate
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM");
    while (1);
  }

  Serial.println("Welcome to the data streaming application using the Nano 33 BLE Sense\n");
}

void loop() {
  // see if the button is pressed and turn off or on recording accordingly
  bool clicked = readShieldButton();
  if (clicked){
    record = !record;
  }
  
  
  while (Serial.available()) {
    char incomingByte = Serial.read();

    if (incomingByte == 'x') {
      record = !record;
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
    } else if (incomingByte == 'a'){
      digitalWrite(LEDR,HIGH);
      digitalWrite(LEDG,HIGH);
      digitalWrite(LEDB,LOW);
    } else if (incomingByte == 'b'){
      digitalWrite(LEDR,HIGH);
      digitalWrite(LEDG,LOW);
      digitalWrite(LEDB,HIGH);
    } else if (incomingByte == 'c'){
      digitalWrite(LEDR,LOW);
      digitalWrite(LEDG,HIGH);
      digitalWrite(LEDB,HIGH);
    } else if (incomingByte == 'd'){
      digitalWrite(LEDR,LOW);
      digitalWrite(LEDG,LOW);
      digitalWrite(LEDB,LOW);
    }
  }

  // display the audio if applicable
  if (samplesRead) {
    // print samples to serial plotter
    if (record) {
      for (int i = 0; i < samplesRead; i++) {
        Serial.println(sampleBuffer[i]);
      }
    }
    // clear read count
    samplesRead = 0;
  } 
}

void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read data into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  samplesRead = bytesAvailable / 2;
}
