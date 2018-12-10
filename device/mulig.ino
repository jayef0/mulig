// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

// This #include statement was automatically added by the Particle IDE.
#include <SparkFunLSM9DS1.h>

// This #include statement was automatically added by the Particle IDE.
#include "IOT-ECOSYS_LSM9DS1_Acc_Gyr_Mag.h"

// This #include statement was automatically added by the Particle IDE.
#include "IOT-ECOSYS_DHT11_Hum_Tem.h"

// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

// digital pin D2 for humidity & temperature sensor
IoTEcoSys_DHT11_Hum_Tem dht(2);
IoTEcoSys_LSM9DS1_Acc_Gyr_Mag lsm(true);

void setup() {
    dht.init();
    lsm.init();
    
}

void loop() {
    //dht.read();
    //Particle.publish(String("Humidity %:"), String(dht.getHumVal(), DEC));
    //Particle.publish(String("Temp C:"), String(dht.getTemVal(), DEC));

    lsm.read();
    printAttitude(lsm.getAccRaw_X(), lsm.getAccRaw_Y(), lsm.getAccRaw_Z(),-lsm.getMagRaw_Y(),-lsm.getMagRaw_X(), lsm.getMagRaw_Z());
    
    delay(2000);
}

void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
    float roll = atan2(ay, az);
    float pitch = atan2(-ax, sqrt(ay * ay + az * az));

    // Convert everything from radians to degrees:
    //heading *= 180.0 / PI;
    pitch *= 180.0 / PI;
    roll  *= 180.0 / PI;
  
    Particle.publish(String("Pitch"), String(pitch));
    Particle.publish(String("Roll"), String(roll));
}