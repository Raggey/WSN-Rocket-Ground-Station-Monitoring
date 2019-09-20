#include <SPI.h>
#include <RH_RF95.h>
#include <DHT.h>

#define DHT11_PIN 5
#define DHTTYPE DHT11  

DHT dht(DHT11_PIN, DHTTYPE);
RH_RF95 rf95; // Create radio object

struct SensorData {
  
  uint8_t group_id;
  uint8_t temperature;
  uint8_t humidity;
  
};
SensorData sensor_rec;
uint8_t * rec_pointer;

SensorData sensor_data_payload;
uint8_t * payload_pointer = 0;

float frequency = 924; // This frequency is for Group 1, you will need to change your frequency according to the provided table
void setup() {

  /**************************************
   * Set-up payload and payload pointer
   *************************************/
  sensor_data_payload.group_id = 9;
  payload_pointer = (uint8_t*) &sensor_data_payload;
  rec_pointer = (uint8_t*) &sensor_rec;
  
  Serial.begin(9600);
  Serial.println("Start LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  
  rf95.setFrequency(frequency); 
  
  // Setup Power,dBm
  rf95.setTxPower(20); // Do not increase the transmit power as you do not need to operate over a long range
  
  rf95.setSpreadingFactor(7); // Setup Spreading Factor (6 ~ 12) //7
  
  // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000
  //Lower BandWidth for longer distance.
  rf95.setSignalBandwidth(125000);
  
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  rf95.setCodingRate4(5); //5

  dht.begin();
}

void loop(){
  
  /**************************************
   * Collect data in struct
   *************************************/
   Serial.println("Sending message to LoRa Server");
   Serial.println();
  
   uint8_t grpMessage[] = "Hello World!";
   uint8_t grpPointer = &grpMessage;
   

   uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
   uint8_t len = sizeof(buf);
 
   rf95.send(grpMessage, sizeof(grpMessage));  
   rf95.waitPacketSent();
  
   // Now wait for a reply
  
//   if (rf95.waitAvailableTimeout(3000)){ 
//     // Should be a reply message for us now   
//     if (rf95.recv(buf, &len)) {
//       Serial.println((char*)buf);
//       Serial.println();
//     }
//     else{
//       Serial.println("recv failed");
//     }
//   }
//  else{
//    Serial.println("No reply, is LoRa server running?");
//    Serial.println();
//  }
  delay(300);
}
