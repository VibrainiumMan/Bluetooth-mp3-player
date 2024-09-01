#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer; //the dfplayer variable

int bluetooth; //data receiving
int sensorValue = A0;
int oldValue = 0;
long volumeDelay = 0;

boolean isPlaying = false; //is the dfplayer playing

void setup() {

    mySoftwareSerial.begin(9600);
    Serial.begin(9600);

    delay(1000);

    Serial.println();
    Serial.println("DFPlayer Mini Demo");
    Serial.println("Initializing DFPlayer...");

    if (!myDFPlayer.begin(mySoftwareSerial)) {
        Serial.println("Unable to begin:");
        Serial.println("1.Please recheck the connection!");
        Serial.println("2.Please insert the SD card!");
        while (true);
    }
    Serial.println(F("DFPlayer Mini online."));

    myDFPlayer.setTimeOut(500); //timeout delay

    //----Set different EQ----
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

    myDFPlayer.play(1); //Play the first song
    isPlaying = true; //isPlaying is true
    Serial.println("Playing..");

    pinMode(sensorValue, INPUT);
}

void loop() {
  while(Serial.available()>0){ //while there is serial communication
    bluetooth = Serial.read(); //data is being red
   }

   sensorValue = analogRead(sensorValue);
   
   int newValue = map (sensorValue, 0, 1023, 0, 30); //New value of the potentiometer
    if(oldValue != newValue && volumeDelay + 500 <= millis()){
      oldValue = newValue; //updates vol
      Serial.println(newValue);
      myDFPlayer.volume(newValue);
      volumeDelay = millis();
    }
    
    //when the data is being recived from the mobile device
    if (bluetooth == '1') { //Pause/play button
        if (isPlaying) { //if it is playing
            myDFPlayer.pause(); //then the dfplayer will pause
            isPlaying = false; //dfplayer is not playing
            Serial.println("Paused..");
            bluetooth = '4';
        } else {
            isPlaying = true; //if it is playing
            myDFPlayer.start(); //then it will start to play
            Serial.println("Playing..");
            bluetooth ='4';
        }
        delay(500);
    }
    if (bluetooth == '2') { //Next song button
        if (isPlaying) {
            myDFPlayer.next();
            Serial.println("Next Song..");
            bluetooth = '4';
        }
        delay(500);
    }

    if (bluetooth == '3') { //Previous song button
        if (isPlaying) {
            myDFPlayer.previous();
            Serial.println("Previous Song..");
            bluetooth = '4';
        }
        delay(500);
    }

}
