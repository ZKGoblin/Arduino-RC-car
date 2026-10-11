#include <Wire.h>
#include <Adafruit_PN532.h>
#include <DFRobotDFPlayerMini.h>

// PN532 NFC reader using I2C
Adafruit_PN532 nfc(-1, -1);

// DFPlayer Mini
DFRobotDFPlayerMini player;


// timing variables
unsigned long lastScan = 0;
unsigned long lastReaderCheck = 0;

// check reader every 3 seconds
const unsigned long readerCheckInterval = 3000;

// prevent same tag being detected repeatedly
const unsigned long scanCooldown = 1500;


// reset I2C bus
void resetI2C()
{
  Serial.println("Resetting I2C...");

  Wire.end();

  delay(50);

  Wire.begin();

  delay(50);
}


// attempt to reconnect PN532
bool reconnectReader()
{
  Serial.println("Trying to reconnect reader...");

  // reset the I2C bus
  resetI2C();

  // reinitialise PN532
  nfc.begin();

  delay(250);

  // ask PN532 for firmware information
  uint32_t version = nfc.getFirmwareVersion();

  if (version)
  {
    // reconfigure PN532 for normal operation
    nfc.SAMConfig();

    Serial.println("Reader reconnected!");

    return true;
  }

  Serial.println("Reader still unavailable.");

  return false;
}


// setup
void setup()
{
  Serial.begin(9600);

  Serial.println("Starting system...");


  // NFC setup
  Wire.begin();

  nfc.begin();

  delay(100);

  uint32_t versiondata = nfc.getFirmwareVersion();


  // keep trying until PN532 responds
  while (!versiondata)
  {
    Serial.println("Waiting for reader...");

    delay(1000);

    reconnectReader();

    versiondata = nfc.getFirmwareVersion();
  }


  // configure PN532
  nfc.SAMConfig();

  Serial.println("Reader ready!");


  // DFPlayer setup
  Serial1.begin(9600);

  delay(1000);


  while (!player.begin(Serial1))
  {
    Serial.println("Waiting for player...");

    delay(1000);
  }


  // maximum DFPlayer volume
  player.volume(30);

  Serial.println("Player ready!");
  Serial.println("Scanning...");
}


// main loop
void loop()
{
  // NFC tag storage
  uint8_t uid[7];
  uint8_t uidLength;


  // scan for NFC tag
  bool detected = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength,
    500
  );


  // tag detected
  if (detected)
  {
    // make sure cooldown has expired
    if (millis() - lastScan > scanCooldown)
    {
      lastScan = millis();

      Serial.println("Tag detected");

      Serial.print("UID: ");

      for (uint8_t i = 0; i < uidLength; i++)
      {
        Serial.print(uid[i], HEX);
        Serial.print(" ");
      }

      Serial.println();


      // play MP3/0001.mp3
      player.playMp3Folder(1);

      Serial.println("Playing sound");
    }
  }


  // check PN532 connection
  if (!detected &&
      millis() - lastReaderCheck > readerCheckInterval)
  {
    lastReaderCheck = millis();


    uint32_t version = nfc.getFirmwareVersion();


    // PN532 has disappeared
    if (!version)
    {
      Serial.println();
      Serial.println("********************************");
      Serial.println("READER LOST!");
      Serial.println("********************************");


      // keep trying until reader returns
      while (!reconnectReader())
      {
        Serial.println("Waiting for reader to return...");

        delay(1000);
      }


      Serial.println("Reader is back!");
      Serial.println("Scanning...");
      Serial.println();
    }
  }
}
