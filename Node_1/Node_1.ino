#include <SPI.h>
#include <RH_RF95.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define DHT11_PIN 5
#define DHTTYPE DHT11  

DHT dht(DHT11_PIN, DHTTYPE);
RH_RF95 rf95; // Create radio object

struct SensorData {
  
  uint8_t carbon_monoxide; //%
  uint8_t hydrogen; //% 
  uint8_t oxygen; //%
  uint8_t temperature; // 12.3 C
  uint8_t humidity; // 12 %
  uint8_t wind_dir; // 0-360 degrees
  uint8_t GPS_lat; // lat +123.12
  uint8_t GPS_long; //long -123.12
};

  SensorData sensor_rec;
  uint8_t * rec_pointer;
  
  SensorData sensor_data_payload;
  uint8_t * payload_pointer = 0;
  
  float frequency = 924; // This frequency is for Group 1, you will need to change your frequency according to the provided table
  TinyGPS gps;
  SoftwareSerial ss(3, 4); // Arduino TX, RX , 
  
  static void smartdelay(unsigned long ms);
  static void print_float(float val, float invalid, int len, int prec);
  static void print_int(unsigned long val, unsigned long invalid, int len);
  static void print_date(TinyGPS &gps);
  static void print_str(const char *str, int len);


  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;


void setup() {

  /**************************************
   * Set-up payload and payload pointer
   *************************************/
  payload_pointer = (uint8_t*) &sensor_data_payload;
  rec_pointer = (uint8_t*) &sensor_rec;
  
  Serial.begin(9600);
  Serial.println("Start LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  ss.begin(9600); // SoftSerial port to get GPS data. 
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
   
//   float C0 = analogRead(5); //   Read hydrogen (Pass 00 
//   float hydrogen = analogRead(6) ; //   Read hydrogen (Pass 00 
   float C0 = 0.1; //   Read hydrogen (Pass 00 
   float hydrogen = 0.1; //   Read hydrogen (Pass 00 
   float o2 = 00; //   Read Oxygen (Pass 00 
   float t = dht.readTemperature();// Read temperature as Celsius (the default)
   float h = dht.readHumidity();
   float wind = 0.0; //   Read Wind Direction (Send 000)
   float latitude =  123.123;
   float longitude = 123.123;

   Serial.println("Sending message to LoRa Server");
   Serial.println(); 

   //Initialise rec message
   uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
   uint8_t len = sizeof(buf);

   //Sensor Reads here
   sensor_data_payload.carbon_monoxide = C0; 
   sensor_data_payload.hydrogen = hydrogen;
   sensor_data_payload.oxygen = o2;
   sensor_data_payload.temperature = t;// 12.3 C
   sensor_data_payload.humidity = h; // 12 %
   sensor_data_payload.wind_dir = wind; // 0-360 degrees
   sensor_data_payload.GPS_lat = latitude; // lat +123.12
   sensor_data_payload.GPS_long = longitude; //long -123.12

   getGPS();
Serial.println(sizeof(&sensor_data_payload));
   

//  rf95.send(payload_pointer, sizeof(sensor_data_payload));  
//   rf95.waitPacketSent();
//  
//   // Now wait for a reply
//  
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


static void send_lora(){

   

}
