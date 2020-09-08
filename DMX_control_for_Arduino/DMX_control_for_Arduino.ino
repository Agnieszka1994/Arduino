


#include <DmxSimple.h>
#include <ArduinoJson.h>


//CONSTANTS
// The pin on which Arduino will transmit DMXmessages
const byte dmxTransmitPin = 4;
//Used to switch between Master/Slave modes
const byte modeSelectPin = 2;
//Buttons
const byte blueButton = A0;
const byte redButton = A1;


void setup() {
  // Serial connection used to receive DMX request as JSON payload from Node-Red:
  Serial.begin(9600);

  //Help text - examples payload expected
  Serial.println("Send DMX channel/values as JSON payload in the following format:");
  Serial.println("{\"channels\":[");
  Serial.println("   {\"channel\":1,\"value\":255},");
  Serial.println("   {\"channel\":3,\"value\":128}");
  Serial.println("]}");

  //DMX shield - set to "DMX Master" mode
  pinMode(modeSelectPin, OUTPUT);
  digitalWrite(modeSelectPin, HIGH);

  //the pin that the Arduino will use to communicate the the DMX shield
  DmxSimple.usePin(dmxTransmitPin);
  //define all channels - some devices require the whole 512byte message to be sent
  DmxSimple.maxChannel(512);

  //Initialise the buttons pins:
  pinMode(blueButton, INPUT_PULLUP);
  pinMode(redButton, INPUT_PULLUP);


}

void loop() {


  if (Serial.available()) {
    //create a new JSON doc
    DynamicJsonDocument doc(512);
    //parse any incoming data received over the serial connection
    DeserializationError error = deserializeJson(doc, Serial);
    //chack if the data is valid
    if (error) {
      Serial.println(F("deserializeJson() failed: ")); //save to Flash memory - F();
      Serial.println(error.c_str());
      return; 
    }
    //Parse the array of channel-values from the received string
    JsonArray array = doc["channels"].as<JsonArray>();
    for (JsonVariant v : array) {
      Serial.println(v["channel"].as<int>() );
      Serial.println(v["value"].as<int>() );
      // write each value to the correct channel
      DmxSimple.write(v["channel"].as<int>(), v["value"].as<int>());
    }
Serial.println("OK");

  }

}
