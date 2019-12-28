/*
  struct.h
  Created by Peter Czibula, February 18, 2019.
*/
#ifndef struct_h
#define struct_h

#include <odroid_go.h>

#define SPECTRUMBAND_COUNT 7
#define LED_COUNT 13
#define WATERJET_COUNT 13

struct AlaColor
{
  union {
    struct
    {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };
    uint8_t raw[3];
  };
};

struct CONTROLLER_DATA_STRUCTURE
{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float ampFactor;
  uint8_t waterjetAttack;
  uint8_t waterjetDecay;
  bool EnergyBeatDetected;
  bool BassBeatDetected;
  bool VolumeBeatDetected;
};

struct ANALYSER_DATA_STRUCTURE
{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float ampFactor;
  uint8_t waterjetAttack;
  uint8_t waterjetDecay;
  // Spectrum analyzer read values.
  byte Spectrum[SPECTRUMBAND_COUNT];
  // average BPM for the last 10 seconds
  int BPM;
  byte EnergyAvg = 0;
  byte EnergyVar = 0;
  byte EnergyStdDev = 0;
  bool EnergyBeatDetected = false;
  bool BassBeatDetected = false;
  bool VolumeBeatDetected = false;
  // Current volume value (sum of all bands / 6)
  byte Volume;
  AlaColor leds[LED_COUNT];
  byte waterjetCurrentHeight[WATERJET_COUNT];
};

#endif
