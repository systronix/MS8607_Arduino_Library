#include <ms8607.h>

static ms8607 m_ms8607;

boolean OK;

void setup() 
{
  Serial.begin(115200);     // use max baud rate
  // Teensy3 doesn't reset with Serial Monitor as do Teensy2/++2, or wait for Serial Monitor window
  // Wait here for 10 seconds to see if we will use Serial Monitor, so output is not lost
  while((!Serial) && (millis()<10000));    // wait until serial monitor is open or timeout

  Serial.printf("MS8607 Demo with MUX control for TE Arduino Weathershield\n");
 

  // WeatherShield MUX control pins
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  // MUX select MS8607
  Serial.printf("Select MS8607\n");
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
  delay(5);

  enum ms8607_status status;
  status = ms8607_status_ok;

  m_ms8607.begin();

  // TODO cleanup and improve
  // never resets if this connect fails!
  // which means calibration PROM may never get loaded! See data sheet 09/2015 Page 13
  if (m_ms8607.is_connected() == true) 
  {
    status = m_ms8607.reset(); 
    Serial.printf("OK: MS8607 status is %i\n", status);
  }
  else
  {
    Serial.printf("ERROR: MS8607 connect failed\n");
  }
   Serial.printf(" Secs,   \xC2\xB0\x43,   hPa,  inhg,  %%RH\n");
}

void loop() {
  float temperature;
  float pressure;
  float humidity;
  boolean connected;

  connected = m_ms8607.is_connected();
  if (connected == true) 
  {
    // Serial.println("MS8607 connected in loop");
    m_ms8607.read_temperature_pressure_humidity(&temperature, &pressure, &humidity);

    // Print outputs in a format easily read into spreadsheet
    // Serial.printf("Temp= %4.2f \xC2\xB0\x43,", temperature);   // UTF-8 degree symbol and C
    // Serial.printf(" Press= %5.2f hPa,", pressure);
    // Serial.printf(" Hum= %4.2f %%RH\n", humidity);
    Serial.printf("%5u, ", millis()/1000);
    Serial.printf("%3.1f, ", temperature);   // UTF-8 degree symbol and C
    Serial.printf("%4.1f, ", pressure);
    Serial.printf("%2.2f, ", pressure*.02953);
    Serial.printf("%2.1f\n", humidity);    
  } 
  else 
  {
    Serial.printf("MS8607 not connecting in loop\n");
  }
  delay(10000);
}
