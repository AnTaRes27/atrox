// PROJECT ATROX v0.001.1
//***************************************************************************//
// command.cpp                                                               //
//                                                                           //
// Description:                                                              //
//      This is the implementation file for the command class.               //
//                                                                           //
// Written by Antares Husky (antares xavier kiriakov)                        //
// Distributed under the GNU AGPLv3 license                                  //
// xray77n@gmail.com                                                         //
// 04 AUG 2019; Last revision: 05 AUG 2019                                   //
//***************************************************************************//


#include <HardwareSerial.h>

#include "command.h"


/*  Command::Command(Atrox* ptr)
    > constructor for a command
    args:
      Atrox* ptr: address of system
*/
Command::Command(Atrox* ptr){
  atroxPtr = ptr;
  commandInit('O', 27);
} //end Command::Command(Atrox*)


/*  Command::Command(Atrox* ptr, char addr, int val)
    > constructor for a command, giving in valid address and value
    args:
      Atrox* ptr: address of system
      char addr: letter address of command
      int val: address value of command
*/
Command::Command(Atrox* ptr, char addr, int val){
  atroxPtr = ptr;
  commandInit(addr, val);
} //end Command::Command(Atrox*, char, int)


/*  int Command::commandInit(char addr, int val)
    > prepare command for arguments
    args:
      char addr: letter address of command
      int val: address value of command
    returns:
      int of command status
        status -1 indicates failure
        status 2 indicates a need for WPR values
        status 8 indicates a complete command
*/
int Command::commandInit(char addr, int val){
  int status{-1};
  cmdAddr = toupper(addr);
  cmdVal = val;
  switch(cmdAddr){
    case 'G': 
      switch(cmdVal){
        case 90:
          //G90 - ABSOLUTE POSITIONING
          status = 8; //complete
          break;
        case 91:
          //G91 - RELATIVE POSITIONING
          status = 8; //complete
          break;
        case 200:
          //G200 - ROTATE AXIS
          //       DEFAULT CLOCKWISE
          initStaticsData();
          initDynamicsData();
          status = 2; //need WPR
          break;
        case 220:
          //G220 - ANGULAR UNIT: STEP
          status = 8;
          break;
        case 221:
          //G220 - ANGULAR UNIT: DEGREE
          status = 8;
          break;
        case 291:
          //G291 - JOG ROTATIONAL AXIS, ALWAYS RELATIVE
          //       DEFAULT CLOCKWISE
          initStaticsData();
          initDynamicsData();
          status = 2; //need PRY
          break;
      } //end switch(cmdVal) for G
      break; //end G
    case 'M':
      switch(cmdVal){
        case 0:
          //M0 - STOP
          status = 8; //complete
          break;
        case 1:
          //M1 - MANUAL STOP
          status = 8; //complete
          break;
        case 17:
          //M17 - ENABLE STEPPERS
          status = 8; //complete
          break;
        case 18:
          //M18 - DISABLE STEPPERS
          status = 8; //complete
          break;
        case 76:
          //M76 - PAUSE
          status = 8; //complete
          break;
      } //end switch(cmdVal) for M
    default:
      status = -1;
      break;
  } //end switch(cmdAddr)
  return status;
} //end Command::commandInit(char, int)


/*  void Command::commandArgMove(float arg[])
    > loads move command arguments
    args:
      float arg[]: list of arguments
    returns nothing
*/
void Command::commandArgMove(float arg[]){
  cmdStatics.X = arg[0];
  cmdStatics.Y = arg[1];
  cmdStatics.Z = arg[2];
  cmdStatics.W = arg[3];
  cmdStatics.P = arg[4];
  cmdStatics.R = arg[5];
  cmdDynamics.linSpeed = arg[6];
  cmdDynamics.linAccel = arg[7];
  cmdDynamics.angSpeed = arg[8];
  cmdDynamics.angAccel = arg[9];

  return;
} //end Command::commandArgMove(float[])


/*  void Command::execute()
    > executes loaded command with stored arguments
    no args
    returns nothing
*/
void Command::execute(){
  Serial.print(cmdAddr);
  Serial.print(cmdVal);
  Serial.print(' ');
  Serial.print('W');
  Serial.print(cmdStatics.W);
  Serial.print('|');
  Serial.print('P');
  Serial.print(cmdStatics.P);
  Serial.print('|');
  Serial.print('R');
  Serial.print(cmdStatics.R);
  Serial.print(' ');
  Serial.println(cmdDynamics.angSpeed);

  switch(cmdAddr){
    case 'G': 
      switch(cmdVal){
        case 90:
          //G90 - ABSOLUTE POSITIONING
          atroxPtr->posMode = ABSOLUTE_POS;
          break;
        case 91:
          //G91 - RELATIVE POSITIONING
          atroxPtr->posMode = RELATIVE_POS;
          break;
        case 200:
          //G200 - ROTATE AXIS
          //       DEFAULT CLOCKWISE
          break;
        case 220:
          //G220 - ANGULAR UNIT: STEP
          atroxPtr->angUnit = STEP;
          break;
        case 221:
          //G220 - ANGULAR UNIT: DEGREE
          atroxPtr->angUnit = DEGREE;
          break;
        case 291:
          //G291 - JOG ROTATIONAL AXIS, ALWAYS RELATIVE
          //       DEFAULT CLOCKWISE
          atroxPtr->moveAxis(P_AXIS, cmdStatics.P, cmdDynamics);
          break;
      } //end switch(cmdVal) for G
      break;
    case 'M':
      switch(cmdVal){
        case 0:
          //M0 - STOP
          //TODO
          break;
        case 1:
          //M1 - MANUAL STOP
          //TODO
          break;
        case 17:
          //M17 - ENABLE STEPPERS
          atroxPtr->engageSteppers();
          break;
        case 18:
          //M18 - DISABLE STEPPERS
          atroxPtr->releaseSteppers();
          break;
        case 76:
          //M76 - PAUSE
          //TODO
          break;
      } //end switch(cmdVal) for M
      break;
  } //end switch(cmdAddr)
  return;
} //end Command::execute()


/*  void Command::execute(char addr, int val)
    > executes command given by valid arguments
    args:
      char addr: letter address
      int val: address value
    returns nothing
*/
void Command::execute(char addr, int val){
  this->commandInit(addr, val);
  this->execute();
} //end Command::execute(char, int)


/*  void Command::initStaticsData()
    > initialises the command's statics data
    no args
    returns nothing
*/
void Command::initStaticsData(){
  cmdStatics.X = 0;
  cmdStatics.Y = 0;
  cmdStatics.Z = 0;
  cmdStatics.W = 0;
  cmdStatics.P = 0;
  cmdStatics.R = 0;
  return;
} //end initStaticsData()


/*  void Command::initDynamicsData()
    > initialises the command's dynamics data
    no args
    returns nothing
*/
void Command::initDynamicsData(){
  cmdDynamics.linSpeed = 0;
  cmdDynamics.linAccel = 0;
  cmdDynamics.angSpeed = 0;
  cmdDynamics.angAccel = 0;
  return;
} //end initDynamicsData()