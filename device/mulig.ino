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

bool isHorizontal = false;
float baseWeight = 0.0;
float currentWeight = 0.0;
str currentTimestamp = "";

/* ---------------------------------------------------------------- */

// CONFIGURE SYSTEM COMPONENTS (PIN MANAGEMENT, ...)

IoTEcoSys_DHT11_Hum_Tem dht(HUMIDITY_TEMPERATURE_PIN);
IoTEcoSys_LSM9DS1_Acc_Gyr_Mag lsm(true);
HX711 scale(LOAD_CELL_AMPLIFIER_DOUT, LOAD_CELL_AMPLIFIER_CLK);

/* ---------------------------------------------------------------- */

void setup() {
    // Set initializatian state
    CurrentState = INITIALIZATION;
}

/* ---------------------------------------------------------------- */

// STATE MACHINE METHODS

// STATE: Initialization
void initializationState() {
    // Initialize Humidity and Temperature sensor
    dht.init();
    // Initialize GyroMagnoAcc sensor
    lsm.init();

    // TODO: Disable some parts of the sensors to save energy

    // Set calibration for load cell amplifier
    scale.set_scale(CALIBRATION_FACTOR);
    // Initialize time settings
    Time.zone(10);
}

// STATE: Measure values
void measureValuesState() {
    // Generate timestamp
    // TODO - Serial.println(Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL))

    // Load current weight
    // IMPORTANT: get_units() return weight in lbs instead of kg
    currentWeight = scale.get_units() * LBS_TO_KG;

    // Measure values of AccGyroMagno sensor
    lsm.read();
    // Use the values of the AccGyroMagno sensor to measure the orientation
    isHorizontal = getIsHorizontal(lsm.getAccRaw_X(), lsm.getAccRaw_Y(), lsm.getAccRaw_Z(),
      -lsm.getMagRaw_Y(),-lsm.getMagRaw_X(), lsm.getMagRaw_Z());
}

// STATE: Send values
void sendValuesState() {
    //TODO: Take timestamp





    sprintf(publishString,"{\"isHorizontal\": %s, \"baseWeight\": %.3f, \"completeWeight\": %.3f, \"time\": %s}",isHorizontal ? "true" : "false",baseWeight,currentWeight,xyz);
    Particle.publish("message",publishString);
}

// STATE: Hibernate device
void hibernateState() {
}

// STATE: Hibernate device
void sleepState() {
    // delay of 5 minutes
    delay(300000);
}

void loop() {
    switch (CurrentState)
    ​{
        case INITIALIZATION:
            initializationState();
            CurrentState = MEASURE_VALUES;
            break;
        case MEASURE_VALUES:
            measureValuesState();
            CurrentState = SEND_VALUES;
            break;
        case SEND_VALUES:
            sendValuesState();
            CurrentState = SLEEP;
            break;
        case SLEEP:
            sleepState();
            CurrentState = MEASURE_VALUES;
            break;
    }
}

/* ---------------------------------------------------------------- */

bool getIsHorizontal(float ax, float ay, float az, float mx, float my, float mz)
{
    float roll = atan2(ay, az);
    float pitch = atan2(-ax, sqrt(ay * ay + az * az));

    // Convert everything from radians to degrees:
    //heading *= 180.0 / PI;
    pitch *= 180.0 / PI;
    roll  *= 180.0 / PI;

    //Particle.publish(String("Pitch"), String(pitch));
    //Particle.publish(String("Roll"), String(roll));

    // TODO: do check
}
