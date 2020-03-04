#include "Arduino.h"
#include "SPI.h"
#include "WiFi.h"
#include "Audio.h"

#define I2S_DOUT      22
#define I2S_BCLK      26
#define I2S_LRC       25

Audio audio;

String ssid =     ".......";
String password = ".......";

void setup() 
{
    Serial.begin(115200);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("wifi...");
    }
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21);    
    audio.connecttohost("0n-80s.radionetz.de:8000/0n-70s.mp3");
}

void loop()
{
    audio.loop();
}

//optional
void audio_info(const char *info){            Serial.print("info        "); Serial.println(info);}
void audio_id3data(const char *info){         Serial.print("id3data     "); Serial.println(info);}
void audio_eof_mp3(const char *info){         Serial.print("eof_mp3     "); Serial.println(info);}
void audio_showstation(const char *info){     Serial.print("station     "); Serial.println(info);}
void audio_showstreaminfo(const char *info){  Serial.print("streaminfo  "); Serial.println(info);}
void audio_showstreamtitle(const char *info){ Serial.print("streamtitle "); Serial.println(info);}
void audio_bitrate(const char *info){         Serial.print("bitrate     "); Serial.println(info);}
void audio_commercial(const char *info){      Serial.print("commercial  "); Serial.println(info);}
void audio_icyurl(const char *info){          Serial.print("icyurl      "); Serial.println(info);}
void audio_lasthost(const char *info){        Serial.print("lasthost    "); Serial.println(info);}
void audio_eof_speech(const char *info){      Serial.print("eof_speech  "); Serial.println(info);}
