

static void read_temp(){
   float t = dht.readTemperature();// Read temperature as Celsius (the default)
   Serial.print("Temperature = ");
   Serial.println(t);
   return t;
}

static void   read_humid(){
   float h = dht.readHumidity();
   Serial.print("Humidity = ");
   Serial.println(h);
   return h;
}



static void read_sensors(){

   float h = dht.readHumidity();
   float t = dht.readTemperature();// Read temperature as Celsius (the default)
}
  
   
