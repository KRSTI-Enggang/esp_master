// Contoh ini membuat jembatan antara Bluetooth Serial dan Bluetooth Klasik (SPP)
// Ini adalah perpanjangan dari contoh SerialToSerialBT oleh Evandro Copercini - 2018
// JANGAN mencoba menyambungkan ke ponsel atau laptop - mereka adalah master
// perangkat, sama seperti ESP menggunakan kode ini - TIDAK akan berfungsi!
// Anda dapat mencoba mem-flash ESP32 kedua dengan contoh SerialToSerialBT - seharusnya
// secara otomatis berpasangan dengan ESP32 yang menjalankan kode ini
//Ubah PIN dan MAC addres,juga ubah nama master dan slave,gunakan salah satu antara nama atau MAC.

#include "BluetoothSerial.h"

//#define USE_NAME // Jika MAC,Comment saja.
const char *pin = "1234"; // ubah PIN dan samakan

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#ifdef USE_NAME
  String slaveName = "Enggang-SLAVE"; // ID
#else
  String MACadd = "AA:BB:CC:11:22:33"; // Debug
  uint8_t address[6]  = {0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33}; // ubah MAC addres
#endif

String myName = "Enggang-MASTER";

void setup() {
  bool connected;
  Serial.begin(115200);

  SerialBT.begin(myName, true);
  Serial.printf("Perangkat ini \"%s\" dalam master mode,pastikan slave aktif!\n", myName.c_str());

  #ifndef USE_NAME
    SerialBT.setPin(pin);
    Serial.println("Mengunakan PIN");
  #endif

  #ifdef USE_NAME
    connected = SerialBT.connect(slaveName);
    Serial.printf("Koneksi ke  slave BT device dengan Nama \"%s\"\n", slaveName.c_str());
  #else
    connected = SerialBT.connect(address);
    Serial.print("Koneksi ke  slave BT device dengan MAC "); Serial.println(MACadd);
  #endif

  if(connected) {
    Serial.println("Sukses terhubung!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Koneksi gagal,dekatkan perangkat.");
    }
  }

  if (SerialBT.disconnect()) {
    Serial.println("Sukses Terputus!");
  }

  SerialBT.connect();
  if(connected) {
    Serial.println("Sukses menghubungkan ulang!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Koneksi gagal,dekatkan perangkat!.");
    }
  }
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}
