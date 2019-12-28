/*
  ObleServer.h
  Created by Peter Czibula, February 18, 2019.
*/
#ifndef ObleServer_h
#define ObleServer_h

#include <odroid_go.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "26068755-0066-45c1-9923-e395ba9a9977"
#define CHARACT_UUID_CONTROLLER "26068755-0066-45c1-9923-e395ba9a9978"
#define CHARACT_UUID_ANALYSER "26068755-0066-45c1-9923-e395ba9a9979"

class ObleServer
{

public:
  // constructor / Setup
  ObleServer(void);
  void startBleService(void);
  void startBleAdvertising(void);
  bool clientConnected(void);
  std::string getTxValue();
  void setTxValue(std::string value);
  void setTxValue(uint8_t *pData, size_t length);
  void notifyTx(void);
  std::string getRxValue();
  void setRxValue(std::string value);
  void setRxValue(uint8_t *pData, size_t length);
  void notifyRx(void);

private:
};

#endif
