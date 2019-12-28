/*
  ObleServer.cpp
  Created by Peter Czibula, February 18, 2019.
*/
#include "ObleServer.h"

// the following variables can be used in BleServer and all the classes defined in BleClient.cpp
static BLEServer *pServer;
static BLEService *pService;
static BLECharacteristic *pTxCharacteristic;
static BLECharacteristic *pRxCharacteristic;
static bool _clientConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
public:
  MyServerCallbacks()
  {
    Serial.println("MyServerCallbacks init");
  };

  void onConnect(BLEServer *pServer)
  {
    Serial.println("Client connected!");
    _clientConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("Client disconnected!");
    _clientConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
  // when we receive a value un our RX buffer
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0)
    {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
      {
        Serial.print(rxValue[i]);
      }
      Serial.println();
      Serial.println("*********");
    }
  }
};

ObleServer::ObleServer(void){};

void ObleServer::startBleService(void)
{
  // Create the BLE Device
  BLEDevice::init("ESP32 Waasseruergel");
  // BLEDevice::setPower(ESP_PWR_LVL_P9);

  // Create the BLE Server
  Serial.println(" - Creating server");
  pServer = BLEDevice::createServer();
  // callback when client connects or disconnects
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  Serial.println(" - Creating service");
  pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic for TX
  // client can be notified and can read
  pTxCharacteristic = pService->createCharacteristic(
      CHARACT_UUID_ANALYSER,
      BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_READ);

  // only for notifications
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  pTxCharacteristic->addDescriptor(new BLE2902());

  // Create a BLE Characteristic for RX
  // client can read & write to it
  pRxCharacteristic = pService->createCharacteristic(
      CHARACT_UUID_CONTROLLER,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  // callback when we receive data from our client
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();
};

void ObleServer::startBleAdvertising(void)
{
  // this still is working for backward compatibility
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
};

bool ObleServer::clientConnected(void)
{
  return _clientConnected;
}

// **********************************
// TX IS ANALYSER DATA

std::string ObleServer::getTxValue()
{
  return pTxCharacteristic->getValue();
};

void ObleServer::setTxValue(std::string value)
{
  pTxCharacteristic->setValue(value);
};

void ObleServer::setTxValue(uint8_t *pData, size_t length)
{
  pTxCharacteristic->setValue((uint8_t *)&pData, length);
};

void ObleServer::notifyTx(void)
{
  pTxCharacteristic->notify();
};

// **********************************
// RX IS CONTROLLER DATA

std::string ObleServer::getRxValue()
{
  return pRxCharacteristic->getValue();
};

void ObleServer::setRxValue(std::string value)
{
  pRxCharacteristic->setValue(value);
};

void ObleServer::setRxValue(uint8_t *pData, size_t length)
{
  pRxCharacteristic->setValue((uint8_t *)&pData, length);
};

void ObleServer::notifyRx(void)
{
  pRxCharacteristic->notify();
};
