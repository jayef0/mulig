// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

/* ---------------------------------------------------------------- */

// INCLUDES

// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

#include <Particle.h>

/* ---------------------------------------------------------------- */

// CONSTANTS

// FOR CALCULATIONS

#define KG_TO_LBS   2.20462
#define LBS_TO_KG   0.453592

#define CALIBRATION_FACTOR -900.0

// FOR WIRING MANAGEMENT

#define LOAD_CELL_AMPLIFIER_DOUT  3  // D3
#define LOAD_CELL_AMPLIFIER_CLK   2  // D2

/* ---------------------------------------------------------------- */

// STATE MACHINE

enum SystemState {
    INITIALIZATION  = 0,
    MEASURE_VALUES  = 1,
    SEND_VALUES     = 2,
    SLEEP           = 3
};
typedef enum SystemState SystemState_t ;

/* ---------------------------------------------------------------- */

// GLOBAL VARIABLES

SystemState_t CurrentState = INITIALIZATION;

bool isHorizontal = true;
float baseWeight = 0.0;
float currentWeight = 0.0;
char szBuf[100];
/* ---------------------------------------------------------------- */

// CONFIGURE SYSTEM COMPONENTS (PIN MANAGEMENT, ...)

//IoTEcoSys_LSM9DS1_Acc_Gyr_Mag lsm(true);
HX711ADC scale(LOAD_CELL_AMPLIFIER_DOUT, LOAD_CELL_AMPLIFIER_CLK);

/* ---------------------------------------------------------------- */

void setup() {
    // Set initializatian state
    CurrentState = INITIALIZATION;
}

/* ---------------------------------------------------------------- */

// STATE MACHINE METHODS

// STATE: Initialization
void initializationState() {
    // Set calibration for load cell amplifier
    scale.set_scale(CALIBRATION_FACTOR);
    scale.tare();
}

// STATE: Measure values
void measureValuesState() {

    // Load current weight
    // IMPORTANT: get_units() return weight in lbs instead of kg
    currentWeight = scale.get_units() * LBS_TO_KG;

}

// STATE: Send values
void sendValuesState() {

    sprintf(szBuf,"{\"isHorizontal\": %s, \"baseWeight\": %.3f, \"completeWeight\": %.3f}",isHorizontal ? "true" : "false",baseWeight,currentWeight);
    Particle.publish("message",String(szBuf));
}

// STATE: Hibernate device
void sleepState() {
    // delay of 5 minutes
    delay(3000);
}

void loop() {
    switch (CurrentState)
    {
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