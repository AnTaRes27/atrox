// PROJECT ATROX v0.001.0
//***************************************************************************//
// command.h                                                                 //
//                                                                           //
// Description:                                                              //
//      This is the header file for the command class.                       //
//                                                                           //
// Written by Antares Husky (antares xavier kiriakov)                        //
// Distributed under the GNU AGPLv3 license                                  //
// xray77n@gmail.com                                                         //
// 04 AUG 2019; Last revision: 05 AUG 2019                                   //
//***************************************************************************//


#ifndef _COMMAND_H
#define _COMMAND_H

#include "atrox.h"

/*  class Command
    > command container
    members:
      Atrox* atroxPtr  stores the address to the atrox system
      char cmdAddr     stores the command's letter address
      int cmdVal       stores the command's address value
      staticsData cmdStatics    stores the command's static data
                                includes X, Y, Z, W, P, R positions
      dynamicsData cmdDynamics  stores the command's dynamic data
                                includes linear and angular positions
                                and accelerations
    public methods:
      int commandInit(char, int): initializes a command
      void commandArgMove(float[]): fill movement information about a move command
      void execute(): executes the stored command. must be initialized with commandInit(char, int) or the Command(Atrox*, char, int) constructor before calling. if not initialized, it will do nothing. calling this repeatedly will invoke the last stored command.
      void execute(char, int): execute the command given in the argument
    usage:
      Command(Atrox*): initializes a command giving in the system that
                       processes the command
      Command(Atrox*, char, int): initializes a command with a
                                  valid command address and value

      available commands;
        G90 - ABSOLUTE POSITIONING
        G91 - RELATIVE POSITIONING
        G200 - ROTATE AXIS //NOT YET
        G220 - ANGULAR UNIT: STEP
        G220 - ANGULAR UNIT: DEGREE
        G291 - JOG ROTATIONAL AXIS, ALWAYS RELATIVE
        M0 - STOP //NOT YET
        M1 - MANUAL STOP //NOT YET
        M17 - ENABLE STEPPERS //NOT YET
        M18 - DISABLE STEPPERS //NOT YET
        M76 - PAUSE //NOT YET
        M112 - EMERGENCY STOP //NOT YET
*/
class Command{
  Atrox* atroxPtr;

  char cmdAddr{};
  int cmdVal{};

  staticsData cmdStatics;
  dynamicsData cmdDynamics;

  public:
    Command(Atrox* ptr);
    Command(Atrox* ptr, char addr, int val);
    int commandInit(char addr, int val);
    void commandArgMove(float arg[]);
    void execute();
    void execute(char addr, int val);
  protected:
    void initStaticsData();
    void initDynamicsData();
};

#endif //_COMMAND_H