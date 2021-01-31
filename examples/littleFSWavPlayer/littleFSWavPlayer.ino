// Simple WAV file player example
// 
// Three types of output may be used, by configuring the code below.
//
//   1: Digital I2S - Normally used with the audio shield:
//         http://www.pjrc.com/store/teensy3_audio.html
//
//   2: Digital S/PDIF - Connect pin 22 to a S/PDIF transmitter
//         https://www.oshpark.com/shared_projects/KcDBKHta
//
//   3: Analog DAC - Connect the DAC pin to an amplified speaker
//         http://www.pjrc.com/teensy/gui/?info=AudioOutputAnalog
//
// To configure the output type, first uncomment one of the three
// output objects.  If not using the audio shield, comment out
// the sgtl5000_1 lines in setup(), so it does not wait forever
// trying to configure the SGTL5000 codec chip.
//
// The SD card may connect to different pins, depending on the
// hardware you are using.  Uncomment or configure the SD card
// pins to match your hardware.
//
// Data files to put on your SD card can be downloaded here:
//   http://www.pjrc.com/teensy/td_libs_AudioDataFiles.html
//
// Modified for testing with:
//  LittleFS (only tested with LittleFS_SPINAND, chipSelect 4)
// This example code is in the public domain.

#include <Audio.h>
#include <SPI.h>
#include "play_littleFS_wav.h"
#define MY_VOL_TO_PART

//LittleFS_QSPIFlash myfs;
//LittleFS_Program myfs;
//LittleFS_SPIFlash myfs;
//LittleFS_SPIFram myfs;
LittleFS_SPINAND myfs;   // Has to be declared extern in play_litleFS_wav.cpp.
                         // Does not work the same as using SD.h.
//LittleFS_QPINAND myfs;
#define chipSelect 4

AudioPlayLFSWav           playWav1;
// Use one of these 3 output types: Digital I2S, Digital S/PDIF, or Analog DAC
AudioOutputI2S           audioOutput;
//AudioOutputSPDIF       audioOutput;
//AudioOutputAnalog      audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(8);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);
  
  //SPI.setMOSI(7);
  //SPI.setSCK(14);
  
  // Initialize Flash Memory
  Serial.print("Initializing SPI NAND...");
//  if (!myfs.begin()) {
  if (!myfs.begin(chipSelect)) {
    Serial.println("Initializing spidisk failed:");
  }
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);

  // Simply wait for the file to finish playing.
  while (playWav1.isPlaying()) {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    float vol = analogRead(15);
    vol = vol / 1024;
    sgtl5000_1.volume(vol);
  }
}


void loop() {
  playFile("SDTEST1.WAV");
  delay(500);
  playFile("SDTEST2.WAV");
  delay(500);
  playFile("SDTEST3.WAV");
  delay(500);
  playFile("SDTEST4.WAV");
  delay(1500);
}

