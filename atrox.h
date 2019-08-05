// PROJECT ATROX v0.001.1
//***************************************************************************//
// atrox.h                                                                   //
//                                                                           //
// Description:                                                              //
//      This is the header file for the atrox system class.                  //
//                                                                           //
// Written by Antares Husky (antares xavier kiriakov)                        //
// Distributed under the GNU AGPLv3 license                                  //
// xray77n@gmail.com                                                         //
// 04 AUG 2019; Last revision: 05 AUG 2019                                   //
//***************************************************************************//


#ifndef _ATROX_H
#define _ATROX_H

#include <AccelStepper.h>

enum PosMode {ABSOLUTE_POS, RELATIVE_POS};
enum LinUnit {IN, MM};
enum AngUnit {STEP, DEGREE};
enum Axis {X_AXIS, Y_AXIS, Z_AXIS, W_AXIS, P_AXIS, R_AXIS};

//motor settings
//in the future these will be stored in the EEPROM
const int MOTOR_ENABLE_PIN = 8;

const int X_AXIS_STEP_PIN = 0;
const int X_AXIS_DIRN_PIN = 0;
const int Y_AXIS_STEP_PIN = 0;
const int Y_AXIS_DIRN_PIN = 0;
const int Z_AXIS_STEP_PIN = 0;
const int Z_AXIS_DIRN_PIN = 0;
const int W_AXIS_STEP_PIN = 0;
const int W_AXIS_DIRN_PIN = 0;
const int P_AXIS_STEP_PIN = 3;
const int P_AXIS_DIRN_PIN = 6;
const int R_AXIS_STEP_PIN = 0;
const int R_AXIS_DIRN_PIN = 0;

/*  struct motorData
    > contains information about motor
    > upon construction, step and direction pins are initialized
*/
struct motorData{
  AccelStepper motorhw;

  int stepPerRev{};
  int microstepFactor{};
  int gboxReductionFactor{};
  int gboxIncreaseFactor{};

  float maxSpeedStep{};
  float maxAccelStep{};

  float speedStep{};
  float AccelStep{};

  motorData(const int stepPin, const int dirnPin){
    motorhw = AccelStepper(AccelStepper::DRIVER, stepPin, dirnPin);
  }
};

/*  struct staticsData
    > contains statics data
*/
struct staticsData{
  float A{}; //rotational x-axis
  float B{}; //           y-axis
  float C{}; //           z-axis
  float X{}; //linear x-axis
  float Y{}; //       y-axis
  float Z{}; //       z-axis
  float W{}; //yaw axis rotation
  float P{}; //pitch axis rotation
  float R{}; //roll axis rotation
};

/*  struct dynamicsData
    > contains dynamics data
*/
struct dynamicsData{
  float linSpeed{};
  float linAccel{};
  float angSpeed{};
  float angAccel{};
};

/*  class Atrox
    > system container
    public members:
      PosMode posMode  stores the positioning mode
                       can be either ABSOLUTE_POS or RELATIVE_POS
      LinUnit linUnit  stores the linear unit
                       can be either MM or IN
      AngUnit angUnit  stores the angular unit
                       can be either STEP or DEGREE
      motor xMotor,    stores information about the x axis motor
            yMotor,    stores information about the y axis motor
            zMotor,    stores information about the z axis motor
            wMotor,    stores information about the yaw axis motor
            pMotor,    stores information about the pitch axis motor
            rMotor;    stores information about the roll axis motor
    public methods:
      void releaseSteppers(): disables all steppers
      void engageSteppers(): enables all steppers
      void moveAxis(const Axis, const int step, const dynamicsData): moves the specified motor a specified amount of steps
      void moveAxis(const Axis, const float degree, const dynamicsData): moves the specified motor a specified amount of degrees
    usage:
      Atrox(const int[6][6]): initializes a system giving in the motors' settings
*/
class Atrox{

  public:
    PosMode posMode{RELATIVE_POS};
    LinUnit linUnit{MM};
    AngUnit angUnit{STEP};

    motorData xMotor = motorData(X_AXIS_STEP_PIN, X_AXIS_DIRN_PIN);
    motorData yMotor = motorData(Y_AXIS_STEP_PIN, Y_AXIS_DIRN_PIN);
    motorData zMotor = motorData(Z_AXIS_STEP_PIN, Z_AXIS_DIRN_PIN);
    motorData wMotor = motorData(W_AXIS_STEP_PIN, W_AXIS_DIRN_PIN);
    motorData pMotor = motorData(P_AXIS_STEP_PIN, P_AXIS_DIRN_PIN);
    motorData rMotor = motorData(R_AXIS_STEP_PIN, R_AXIS_DIRN_PIN);

    Atrox(const int motorSet[6][6]);

    void releaseSteppers();
    void engageSteppers();
    void moveAxis(const Axis axis, const int val, const dynamicsData cmdDynamics);
    void moveAxis(const Axis axis, const float val, const dynamicsData cmdDynamics);
  protected:
    void moveAxisStep(const Axis axis, const int step, const dynamicsData cmdDynamics);
    void moveAxisDegree(const Axis axis, const float degree, const dynamicsData cmdDynamics);
};

#endif //_ATROX_H