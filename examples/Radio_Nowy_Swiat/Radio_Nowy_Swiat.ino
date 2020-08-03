
//#include "time.h"
#define DEBUG 1
#include "Arduino.h"
#include "SPIFFS.h"

#include "WiFi.h"
#include <HTTPClient.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#include "Audio.h"
#include "Stations.h"

// Digital I/O used
#define I2S_DOUT      27
#define I2S_BCLK      26
#define I2S_LRC       25
#define volumeStart    9

Audio audio;
 
unsigned long plusMillis  = 0;
unsigned long currentMillis  = 0;
unsigned long previousMillis = 0;
unsigned long intervalMillis = 166;
//unsigned long zegar = 0;

uint8_t cur_station   = 0;

int cur_volume        = 10;

const byte ADCsuw = 7;
const int ADCpodzial = 24;
const int ADCminimum = 150;
const int deltaADC   = 100;

#define divADC      15000 // 16000 im większy tym głośniej

int    ADC           = 0;
int    lastADC       = 0;
int    potValue      = 0;
bool   ADCzero       = false;
String streamInfo    = " ";
//String timeFromServer = "";

#define  potPin        34  // pudelko=32 // kuchnia,roza =34
#define  ledPIN        33
#define  pwnPIN        32
#define  podzial       4

String CoJestGrane  = "";



void alarm(int czas){
   digitalWrite(ledPIN, LOW);
   delay(czas);
   digitalWrite(ledPIN, HIGH); 
}

void alarmStop(){
  for (int i = 0; i <= 5; i++) {
    alarm(100+(i*100));
  }  
}

void setup() {
  #ifdef DEBUG 
    Serial.begin(115200); 
    Serial.println("");  
    Serial.println("Radio dla rozy"); 
    Serial.println("-------------");
  #endif  

  //analogReadResolution(9);                   // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits
  //analogSetWidth(9);                         // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits

                  pinMode(ledPIN, OUTPUT);
                  pinMode(pwnPIN,OUTPUT);
                  ledcSetup(0, 10000, 8);
                  ledcAttachPin(pwnPIN, 0);
  
                /*SPIFFS - musi inicjować system plików */               
                if (!SPIFFS.begin(true)) {
                  Serial.println("An Error has occurred while mounting SPIFFS");
                }
                File root = SPIFFS.open("/"); 
                /*File file = root.openNextFile();
                while(file){
                    Serial.print("FILE: ");
                    Serial.println(file.name());
                    file = root.openNextFile();
                }*/    
              /*SPIFFS*/

    wifiMulti.addAP("", "");
    wifiMulti.addAP("", "");
    wifiMulti.addAP("", "");
    wifiMulti.addAP("", "");

    if(wifiMulti.run() == WL_CONNECTED) {
      IPAddress ip   = WiFi.localIP();
      String    rssi = String(WiFi.RSSI()); 
      String    ssid = WiFi.SSID();
        #ifdef DEBUG
          Serial.println("\nWiFi connected...................................");
            Serial.print("IP: ");    Serial.println(ip);
            Serial.print("RSSI: ");  Serial.println(rssi);
            Serial.print("SSID: ");  Serial.println(ssid);
            Serial.println("...................................");
         #endif
         alarm(300);

          audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
          delay(333);
          digitalWrite(ledPIN, LOW);
          ADC = analogRead(potPin); 
          setVolumeGain(ADC);
          lastADC = ADC;             
          audio.connecttoFS (SPIFFS, "r.mp3");
          delay(111);
          setStation();
    }
} // EOF setup

byte licznikADC = 0;
unsigned long licznikZmian = 0;
char incomingByte;


void loop(){
    audio.loop();
    
    currentMillis = millis();
    if(currentMillis - previousMillis > intervalMillis + plusMillis){
          licznikADC++;
          /* ADC * ADC * ADC * ADC * ADC * ADC * ADC * ADC * */
          ADC = (analogRead(potPin) + lastADC) >> 1;          
          //Serial.printf("ADC=%d lastADC=%d delta=%d", ADC, lastADC, ADC-lastADC); Serial.println("");   
      if (licznikADC % podzial == 0){
              previousMillis = currentMillis;
              plusMillis = 0; 
                       
                               /******/
                               
                              if (Serial.available() > 0) {
                                  cur_station = Serial.parseInt(); // czytamy
                                  incomingByte = Serial.read();    // tracimy zbędny bajt
                                  Serial.println("\n================");
                                  Serial.print("cur_station=");Serial.print(cur_station);Serial.print(", incomingByte=");Serial.println(incomingByte);                             
                                  if(cur_station >= 100) {
                                    cur_volume = cur_station-100;
                                    audio.setVolume(cur_volume);
                                  Serial.print("volume="); Serial.println(cur_station-100);
                                  }
                                  
                                  if(cur_station < max_stations) setStation();
                                } 
                                         
           
          if (abs(ADC-lastADC) > deltaADC){ 
              potValue = (ADC >> 1 << 1) >> ADCsuw; // 7=512 //1024
              //potValue = (ADC || 1) >> ADCsuw; // 7=512 //1024
              #ifdef DEBUG
                   Serial.printf("#154 ADC=%d lastADC=%d delta=%d potValue=%d", ADC, lastADC, ADC-lastADC,potValue); Serial.println("");   
              #endif
              if (potValue <= ADCpodzial){
                setVolumeGain(ADC);
                   if (ADCzero==true){
                    Serial.println("ADCzero");
                      ADCzero=false;
                      setStation();
                  }
              }
              if (potValue > ADCpodzial) {
                cur_station++;
                if(cur_station < max_stations) {
                    licznikADC=0;   
                } else {
                  cur_station = 0; 
                }
              #ifdef DEBUG
                  Serial.print("#367 cur_station=");Serial.println(cur_station);
              #endif 
                audio.stopSong();
                alarm(300);  
                 plusMillis = 2500;  
                setStation();
                 licznikZmian++;   
              } 
              if (potValue < 2) { 
                cur_station = 0;
                audio.stopSong();
                Serial.print("#378 audio.stopSong()=");Serial.println(cur_station);
                alarm(300);
                ADCzero=true;
                #ifdef DEBUG 
                    Serial.print("#383 STACJA=");Serial.println(cur_station);
                #endif
                audio.setVolume(volumeStart-2);
                audio.connecttoFS (SPIFFS, "1.mp3");
              }    
             lastADC = ADC;         
          }  // eoif potValue
          
                         
      } // EOF 
    
   } // EOF intervalMillis
  
}

void liczZmiany(){
  Serial.print("#214 licznikZmian="); Serial.println(licznikZmian);
}

void setStation(){
    Serial.print("#203__NEW_cur_station="); Serial.println(cur_station);
    CoJestGrane = String(stacje[cur_station].info)+".mp3";
    Serial.print("#205 CoJestGrane=");Serial.println(CoJestGrane);
    audio.connecttoFS (SPIFFS, "r.mp3");
    liczZmiany();
}
 
void setVolumeGain(int adc){
    //stacje[cur_station].ampli = 100; //???????????????????????
    cur_volume = (adc) / (divADC / stacje[cur_station].ampli); 
    Serial.print("#212__NEW___cur_volume="); Serial.println(cur_volume);
    if (cur_volume > 21) cur_volume = 21;
    audio.setVolume(cur_volume); 
    liczZmiany();
}

/***********************************/


 void koniecGrania(){
      liczZmiany();
      //char *tempInfo = stacje[cur_station].info;
      //stacje[cur_station].info = "Cisza...";
      audio.stopSong();
      audio.setVolume(0);
      Serial.print("#225 KONIEC idziemy spac");
      //stacje[cur_station].info = tempInfo;
      alarm(100);
      digitalWrite(ledPIN, HIGH); 
      if (licznikZmian>10){
          delay(1000*10);
          audio.setVolume(volumeStart-3);
          audio.connecttoFS (SPIFFS, "x.mp3");
      }
 }



void audio_eof_mp3(const char *info){ 
    Serial.print("#232 info=");Serial.println(info);
    Serial.print("#233 cur_station=");Serial.print(cur_station); 
    
      Serial.print("#235 EOF_MP3 ****************** ");  Serial.println(info);
      
      if (String(info) == "x.mp3"){
          audio.stopSong();
          audio.setVolume(0);        
        return;
      }
      if (String(info) == "1.mp3"){
        koniecGrania();
        return;
      }
      if (String(info) != "r.mp3"){
          plusMillis = 2500;
          audio.connecttohost(stacje[cur_station].stream);
          audio.setVolume(cur_volume);          
      } 
      else {
          plusMillis = 2500;
          if (!CoJestGrane) CoJestGrane = "a.mp3";
          audio.connecttoFS (SPIFFS, CoJestGrane); 
          CoJestGrane=""; 
      }
}

//void audio_info(const char *info){            Serial.print("##261 info        "); Serial.println(info);}
//void audio_id3data(const char *info){         Serial.print("##261 id3data     "); Serial.println(info);}
//void audio_showstation(const char *info){ Serial.print("\n   #430 INFO="); Serial.println(info);alarm(100);}
//void audio_showstreaminfo(const char *info){  Serial.print("#470 streaminfo  "); Serial.println(info);}  
//void audio_showstreamtitle(const char *info){  if (cur_station!=2) Serial.print("#450 audio_showstreamtitle  "); Serial.println(info);    }    
//void audio_bitrate(const char *info){         Serial.print("bitrate     "); Serial.println(info);}
//void audio_commercial(const char *info){      Serial.print("commercial  "); Serial.println(info);}
//void audio_icyurl(const char *info){          Serial.print("#479 icyurl      "); Serial.println(info);}
//void audio_lasthost(const char *info){        Serial.print("#480 lasthost    "); Serial.println(info);}
//void audio_eof_speech(const char *info){      Serial.print("eof_speech  "); Serial.println(info);}
