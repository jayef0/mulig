/* ---------------------------------------------------------------- */

// INCLUDES

// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

// This #include statement was automatically added by the Particle IDE.
#include <SparkFunLSM9DS1.h>

// This #include statement was automatically added by the Particle IDE.
#include "IOT-ECOSYS_LSM9DS1_Acc_Gyr_Mag.h"

// This #include statement was automatically added by the Particle IDE.
#include "IOT-ECOSYS_DHT11_Hum_Tem.h"

/* ---------------------------------------------------------------- */

// CONSTANTS

// FOR CALCULATIONS

// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.

#define PI          3.1415926535897932384626433832795
#define HALF_PI     1.5707963267948966192313216916398
#define TWO_PI      6.283185307179586476925286766559
#define DEG_TO_RAD  0.017453292519943295769236907684886
#define RAD_TO_DEG  57.295779513082320876798154814105

#define KG_TO_LBS   2.20462
#define LBS_TO_KG   0.453592

#define CALIBRATION_FACTOR -7050.0

// FOR WIRING MANAGEMENT

#define LOAD_CELL_AMPLIFIER_DOUT  3  // D3
#define LOAD_CELL_AMPLIFIER_CLK   2  // D2
#define HUMIDITY_TEMPERATURE_PIN  2  // D2

/* ---------------------------------------------------------------- */

// STATE MACHINE

enum SystemState {
    INITIALIZATION  = 0,  // xyz
    MEASURE_VALUES  = 1,  // xyz
    SEND_VALUES     = 2,  // xyz
    SLEEP           = 3,  // xyz
    HIBERNATE       = 4   // xyz
};
typedef enum SystemState SystemState_t ;

/* ---------------------------------------------------------------- */

// GLOBAL VARIABLES

SystemState_t CurrentState = INITIALIZATION;

/* ---------------------------------------------------------------- */

// CONFIGURE SYSTEM COMPONENTS (PIN MANAGEMENT, ...)

IoTEcoSys_DHT11_Hum_Tem dht(HUMIDITY_TEMPERATURE_PIN);
IoTEcoSys_LSM9DS1_Acc_Gyr_Mag lsm(true);
HX711 scale(LOAD_CELL_AMPLIFIER_DOUT, LOAD_CELL_AMPLIFIER_CLK);

/* ---------------------------------------------------------------- */

void setup() {
    //TODO: Setup and init the state machine
}

/*
scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
Serial.print(scale.get_units(), 1); //scale.get_units() returns a float - in lbs
*/
/*
Serial.println(Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL));
*/

/* ---------------------------------------------------------------- */

// STATE MACHINE METHODS

// STATE: Initialization
void initializationState() {
    //
    dht.init();
    lsm.init();
    scale.set_scale(calibration_factor);

    //
    Time.zone(10);
}

// STATE: Measure values
void measureValuesState() {
  //dht.read();
  //Particle.publish(String("Humidity %:"), String(dht.getHumVal(), DEC));
  //Particle.publish(String("Temp C:"), String(dht.getTemVal(), DEC));
  //TODO: save these values and compare them to the last values; if they differ then publish them
}

// STATE: Send values
void sendValuesState() {
  //TODO: Take timestamp
  //TODO: Publish dataset with timestamp
}

// STATE: Hibernate device
void hibernateState() {
}

// STATE: Hibernate device
void sleepState() {
  //wait to measure again
}

void loop() {
    switch (CurrentState)
    ​{
        case INITIALIZATION:
            initializationState();
            break;
        case XYA:
            measureValuesState();
            break;
        case XYB:
            sendValuesState();
            break;
        default:
            // code to be executed if n doesn't match any constant
    }

    lsm.read();
    printAttitude(lsm.getAccRaw_X(), lsm.getAccRaw_Y(), lsm.getAccRaw_Z(),
      -lsm.getMagRaw_Y(),-lsm.getMagRaw_X(), lsm.getMagRaw_Z());

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
