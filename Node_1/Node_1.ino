#include <SPI.h>
#include <RH_RF95.h>
#include <DHT.h>

#define DHT11_PIN 5
#define DHTTYPE DHT11  

DHT dht(DHT11_PIN, DHTTYPE);
RH_RF95 rf95; // Create radio object

struct SensorData {

  uint8_t packet_number; // increment
  uint8_t node_id; // 1 or 2
  uint8_t air_pressure; //kPa
  uint8_t altitude; // Meters above sea level (4 digits?)
  uint8_t temperature; // 12.3 C
  uint8_t humidity; // 12 %
  uint8_t co2_level; //CO2 %
  uint8_t flame; // t/f boolean
  uint8_t GPS_lat; // lat +123.12
  uint8_t GPS_long; //long -123.12
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
  sensor_data_payload.node_id = 1;
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
  // Lower BandWidth for longer distance.
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

   //Initialise rec message
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
   uint8_t len = sizeof(buf);

   //Sensor Reads here
   sensor_data_payload.packet_number = 1; // increment
   sensor_data_payload.node_id = 2; // 1 or 2
   sensor_data_payload.air_pressure = 3; //kPa
   sensor_data_payload.altitude = 4; // Meters above sea level (4 digits?)
   sensor_data_payload.temperature = 5;// 12.3 C
   sensor_data_payload.humidity = 6; // 12 %
   sensor_data_payload.co2_level = 7; //CO2 %
   sensor_data_payload.flame = 8; // t/f boolean
   sensor_data_payload.GPS_lat = 9; // lat +123.12
   sensor_data_payload.GPS_long = 10; //long -123.12

   

   rf95.send(payload_pointer, sizeof(sensor_data_payload));  
   rf95.waitPacketSent();
  
   // Now wait for a reply
  
   if (rf95.waitAvailableTimeout(3000)){ 
     // Should be a reply message for us now   
     if (rf95.recv(buf, &len)) {
       Serial.println((char*)buf);
       Serial.println();
     }
     else{
       Serial.println("recv failed");
     }
   }
  else{
    Serial.println("No reply, is LoRa server running?");
    Serial.println();
  }
  delay(300);
}
