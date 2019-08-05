// PROJECT ATROX v0.001.0
//***************************************************************************//
// atrox.cpp                                                                 //
//                                                                           //
// Description:                                                              //
//      This is the implementation file for the atrox system class.          //
//                                                                           //
// Written by Antares Husky (antares xavier kiriakov)                        //
// Distributed under the GNU AGPLv3 license                                  //
// xray77n@gmail.com                                                         //
// 04 AUG 2019; Last revision: 05 AUG 2019                                   //
//***************************************************************************//


#include<AccelStepper.h>

#include "atrox.h"


/*  Atrox constructor Atrox(const int[6][6])
    > constructs the system by defining default motor settings
    arges:
      const int[6][6]: an array of motor settings
*/
Atrox::Atrox(const int motorSet[6][6]){
  xMotor.stepPerRev = motorSet[0][0];
  xMotor.microstepFactor = motorSet[0][1];
  xMotor.gboxReductionFactor = motorSet[0][2];
  xMotor.gboxIncreaseFactor = motorSet[0][3];
  xMotor.speedStep = xMotor.maxSpeedStep = motorSet[0][4];
  xMotor.speedStep = xMotor.maxAccelStep = motorSet[0][5];
  xMotor.motorhw.setMaxSpeed(xMotor.maxSpeedStep);
  xMotor.motofhw.setOutputPins(MOTOR_ENABLE_PIN);

  yMotor.stepPerRev = motorSet[1][0];
  yMotor.microstepFactor = motorSet[1][1];
  yMotor.gboxReductionFactor = motorSet[1][2];
  yMotor.gboxIncreaseFactor = motorSet[1][3];
  yMotor.speedStep = yMotor.maxSpeedStep = motorSet[1][4];
  yMotor.speedStep = yMotor.maxAccelStep = motorSet[1][5];
  yMotor.motorhw.setMaxSpeed(yMotor.maxSpeedStep);
  yMotor.motofhw.setOutputPins(MOTOR_ENABLE_PIN);

  zMotor.stepPerRev = motorSet[2][0];
  zMotor.microstepFactor = motorSet[2][1];
  zMotor.gboxReductionFactor = motorSet[2][2];
  zMotor.gboxIncreaseFactor = motorSet[2][3];
  zMotor.speedStep = zMotor.maxSpeedStep = motorSet[2][4];
  zMotor.speedStep = zMotor.maxAccelStep = motorSet[2][5];
  zMotor.motorhw.setMaxSpeed(zMotor.maxSpeedStep);
  zMotor.motofhw.setOutputPins(MOTOR_ENABLE_PIN);

  wMotor.stepPerRev = motorSet[3][0];
  wMotor.microstepFactor = motorSet[3][1];
  wMotor.gboxReductionFactor = motorSet[3][2];
  wMotor.gboxIncreaseFactor = motorSet[3][3];
  wMotor.speedStep = wMotor.maxSpeedStep = motorSet[3][4];
  wMotor.speedStep = wMotor.maxAccelStep = motorSet[3][5];
  wMotor.motorhw.setMaxSpeed(wMotor.maxSpeedStep);
  wMotor.motofhw.setOutputPins(MOTOR_ENABLE_PIN);

  pMotor.stepPerRev = motorSet[4][0];
  pMotor.microstepFactor = motorSet[4][1];
  pMotor.gboxReductionFactor = motorSet[4][2];
  pMotor.gboxIncreaseFactor = motorSet[4][3];
  pMotor.speedStep = pMotor.maxSpeedStep = motorSet[5][4];
  pMotor.speedStep = pMotor.maxAccelStep = motorSet[5][5];
  pMotor.motorhw.setMaxSpeed(pMotor.maxSpeedStep);
  pMotor.motofhw.setOutputPins(MOTOR_ENABLE_PIN);

  rMotor.stepPerRev = motorSet[5][0];
  rMotor.microstepFactor = motorSet[5][1];
  rMotor.gboxReductionFactor = motorSet[5][2];
  rMotor.gboxIncreaseFactor = motorSet[5][3];
  rMotor.speedStep = rMotor.maxSpeedStep = motorSet[6][4];
  rMotor.speedStep = rMotor.maxAccelStep = motorSet[6][5];
  rMotor.motorhw.setMaxSpeed(rMotor.maxSpeedStep);
  rMotor.motofhw.setOutputPins(MOTOR_ENABLE_PIN);

}


/*  void Atrox::releaseSteppers()
    > disables all stepper motors
    no args
    returns nothing
*/
void Atrox::releaseSteppers(){
  xMotor.motorhw.disableOutputs();
  yMotor.motorhw.disableOutputs();
  zMotor.motorhw.disableOutputs();
  wMotor.motorhw.disableOutputs();
  pMotor.motorhw.disableOutputs();
  rMotor.motorhw.disableOutputs();
  return;
} //end Atrox::releaseSteppers()


/*  void Atrox::engageSteppers()
    > enables all stepper motors
    no args
    returns nothing
*/
void Atrox::engageSteppers(){
  xMotor.motorhw.enableOutputs();
  yMotor.motorhw.enableOutputs();
  zMotor.motorhw.enableOutputs();
  wMotor.motorhw.enableOutputs();
  pMotor.motorhw.enableOutputs();
  rMotor.motorhw.enableOutputs();
  return;
} //end Atrox::engageSteppers()


/*  void Atrox::moveAxis(const Axis axis, const int val, const dynamicsData cmdDynamics)
    > moves a single axis by an amount
    > will call moveAxisStep() or moveAxisDegree() depending on the unit stored in Atrox::angUnit
    args:
      const Axis axis: the axis to move
      const int val: movement value
      const dynamicsData cmdDynamics: the dynamics behaviour of the movement
    returns nothing
*/
void Atrox::moveAxis(const Axis axis, const int val, const dynamicsData cmdDynamics){
  switch(angUnit){
    case STEP:
      moveAxisStep(axis, val, cmdDynamics);
      break;
    case DEGREE:
      moveAxisDegree(axis, (float)val, cmdDynamics);
      break;
  }
  return;
} // end Atrox::moveAxis(Axis, int)


/*  void Atrox::moveAxis(const Axis axis, const float val, const dynamicsData cmdDynamics)
    > overloaded to take float as the movement value
      moves a single axis by an amount
      will call moveAxisStep() or moveAxisDegree() depending on the unit stored in Atrox::angUnit
    args:
      const Axis axis: the axis to move
      const float val: movement value
      const dynamicsData cmdDynamics: the dynamics behaviour of the movement
    returns nothing
*/
void Atrox::moveAxis(const Axis axis, const float val, const dynamicsData cmdDynamics){
  switch(angUnit){
    case STEP:
      moveAxisStep(axis, (int)val, cmdDynamics);
      break;
    case DEGREE:
      moveAxisDegree(axis, val, cmdDynamics);
      break;
  }
  return;
} // end Atrox::moveAxis(Axis, float)


/*  protected void Atrox::moveAxisStep(const Axis axis, const int step, const dynamicsData cmdDynamics)
    > moves a single axis to target position using a step command
      this is a blocking function
    args:
      const Axis axis: the axis to be moved
      const int step: the amount to move, in step
      const dynamicsData cmdDynamics: the dynamics data of the movement
    returns nothing
*/
void Atrox::moveAxisStep(const Axis axis, const int step, const dynamicsData cmdDynamics){
  motorData* motorDataPtr;
  switch(axis){
    case X_AXIS:
      motorDataPtr = &xMotor;
      break;
    case Y_AXIS:
      motorDataPtr = &yMotor;
      break;
    case Z_AXIS:
      motorDataPtr = &zMotor;
      break;
    case W_AXIS:
      motorDataPtr = &wMotor;
      break;
    case P_AXIS:
      motorDataPtr = &pMotor;
      break;
    case R_AXIS:
      motorDataPtr = &rMotor;
      break;
  }

  AccelStepper& motor = motorDataPtr->motorhw;

  //run motor
  bool hasAccl{true};
  motor.move(step);
  if(abs(cmdDynamics.angAccel) < 1.0){ //arbitrary 1step/s/s minimum
    hasAccl = false;
  }else{
    motor.setAcceleration(cmdDynamics.angAccel);
  }
  if(abs(cmdDynamics.angSpeed < 0.00027)){ //1step/hr minimum
    motor.setSpeed(motorDataPtr->speedStep);
  }else{
    motor.setSpeed(cmdDynamics.angSpeed);
  }
  if(hasAccl){
    while(motor.distanceToGo() != 0){
      motor.run();
    }
  }else{
    while(motor.distanceToGo() != 0){
      motor.runSpeedToPosition();
    }
  }
  return;
} // end Atrox::moveAxisStep(const Axis, const int, const dynamicsData)


/*  protected void Atrox::moveAxis(const Axis axis, const int degree, const dynamicsData cmdDynamics)
    > moves a single axis to target position by a certain degree
      will be rounded to the next valid step
      this is a blocking function
    args:
      const Axis axis: the axis to be moved
      const float degree: the amount to move, in degrees
      const dynamicsData cmdDynamics: the dynamics data of the movement
    returns nothing
*/
void Atrox::moveAxisDegree(const Axis axis, const float degree, const dynamicsData cmdDynamics){
  //TODO: convert degree to step
  //TODO: call function to move axis
  return;
} // end Atrox::moveAxis(Axis axis)