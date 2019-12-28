/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    
   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.
   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second. 
*/

#include "main.h"
#include "struct.h"

// prototypes
void generateRandomtxAnalyserData(void);

ObleServer BtServer;
ANALYSER_DATA_STRUCTURE txAnalyserData;
CONTROLLER_DATA_STRUCTURE rxControllerData;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE Server application...");

  BtServer.startBleService();

  // pRxCharacteristic->setValue("Hello World says Waasseruergel");

  Serial.println("Starting advertising...");
  BtServer.startBleAdvertising();

  Serial.println("Waiting for a client connection to notify...");
}

void loop()
{
  delay(2000);
  static long txValue = 0;

  generateRandomtxAnalyserData();

  if (BtServer.clientConnected())
  {
    // Serial.print("Server reads RxValue ");
    // Serial.println((char *)BtServer.getRxValue().c_str());

    // change rx value, which can be read by the client
    // String setRxValue = "Set RxValue to " + String(txValue);
    // Serial.print("Write ");
    // Serial.println(setRxValue);
    // BtServer.setRxValue((char*) setRxValue.c_str());
    
    // BtServer.setRxValue((uint8_t *)&rxControllerData, sizeof(rxControllerData));

    // notify changed value
    String setTxValue = "Server sets TxValue to " + String(txValue);
    Serial.print("Write ");
    Serial.println(setTxValue);
    BtServer.setTxValue((char *)setTxValue.c_str());
    //BtServer.setTxValue((uint8_t *)&txAnalyserData, sizeof(txAnalyserData));

    BtServer.notifyTx();

    txValue++;
    // bluetooth stack will go into congestion, if too many packets are sent,
    // in 6 hours test i was able to go as low as 3ms
    delay(10);
  }
}

void generateRandomtxAnalyserData(void)
{
  // generate random data to test
  txAnalyserData.ampFactor = rxControllerData.ampFactor;
  txAnalyserData.waterjetAttack = rxControllerData.waterjetAttack;
  txAnalyserData.waterjetDecay = rxControllerData.waterjetDecay;
  // Spectrum analyzer read values.
  uint32_t sumvol = 0;
  for (uint16_t i = 0; i < SPECTRUMBAND_COUNT; i++)
  {
    txAnalyserData.Spectrum[i] += (i + 1);
    sumvol += txAnalyserData.Spectrum[i];
  }
  // Current volume value (sum of all bands / 6)
  txAnalyserData.Volume = sumvol / SPECTRUMBAND_COUNT;
  // average BPM for the last 10 seconds
  txAnalyserData.BPM += 5;
  txAnalyserData.EnergyAvg += 2;
  txAnalyserData.EnergyVar += 3;
  txAnalyserData.EnergyStdDev += 1;
  // Energybeat every 800 ms
  static uint32_t EnergyBeatMillis = 0;
  if (millis() > EnergyBeatMillis)
  {
    txAnalyserData.EnergyBeatDetected = true;
    EnergyBeatMillis = millis() + 800;
  }
  else
  {
    txAnalyserData.EnergyBeatDetected = false;
  }
  // Bassbeat every 450 ms
  static uint32_t BassBeatMillis = 0;
  if (millis() > BassBeatMillis)
  {
    txAnalyserData.BassBeatDetected = true;
    BassBeatMillis = millis() + 450;
  }
  else
  {
    txAnalyserData.BassBeatDetected = false;
  }
  // Volumebeat every 230 ms
  static uint32_t VolBeatMillis = 0;
  if (millis() > VolBeatMillis)
  {
    txAnalyserData.VolumeBeatDetected = true;
    VolBeatMillis = millis() + 230;
  }
  else
  {
    txAnalyserData.VolumeBeatDetected = false;
  }
  // Current led rgb values
  for (uint16_t i = 0; i < LED_COUNT; i++)
  {
    txAnalyserData.leds[i].r += (i + 1) * 1;
    txAnalyserData.leds[i].g += (i + 1) * 2;
    txAnalyserData.leds[i].b += (i + 1) * 3;
  }
  // Current waterjet height
  for (uint16_t i = 0; i < WATERJET_COUNT; i++)
  {
    txAnalyserData.waterjetCurrentHeight[i] += (i + 1);
  }
}
