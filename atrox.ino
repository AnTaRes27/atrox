// PROJECT ATROX v0.001.0
//***************************************************************************//
// atrox.ino                                                                 //
//                                                                           //
// Description:                                                              //
//      This is a firmware written for a general targeting system.           //
//      It is intended for use with an Arduino Uno R3 with a CNC Shield V3.00//
//                                                                           //
// Written by Antares Husky (antares xavier kiriakov)                        //
// Distributed under the GNU AGPLv3 license                                  //
// xray77n@gmail.com                                                         //
// 04 AUG 2019; Last revision: 05 AUG 2019                                   //
//***************************************************************************//


///////////////////////////////////////////////////////////////////////////////
//
//  SSSSSS  EEEEEE  TTTTTT  U    U  PPPPP
//  S       E         TT    U    U  P    P
//  SSSSSS  EEEE      TT    U    U  PPPPP
//       S  E         TT    U    U  P
//  SSSSSS  EEEEEE    TT     UUUU   P
//
///////////////////////////////////////////////////////////////////////////////

#include "atrox.h"
#include "command.h"

enum OpMode {MD_COMMAND, MD_PROGRAM, MD_JOYSTICK};
OpMode opMode{MD_COMMAND};

enum ReadState {RD_CMDADDR, RD_CMDVAL, INIT_CMD, RD_CMDARG, RD_END};

/*motor settings in order: steps per rev,
                           microstepping factor,
                           gearbox reduction factor,
                           gearbox increase factor,
                           max motor speed in step per sec
                           max motor acceleration in step per sec per sec
*/
const int motorSet[6][6] = {{  0, 1,  1, 1, 1000, 1000},   //x axis
                            {  0, 1,  1, 1, 1000, 1000},   //y axis
                            {  0, 1,  1, 1, 1000, 1000},   //z axis
                            {200, 1,  1, 1, 1000, 1000},   //w axis
                            {200, 1, 50, 1, 1000, 1000},   //p axis
                            {200, 1,  1, 1, 1000, 1000}};  //r axis

Atrox atrox(motorSet);
Command command(&atrox);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("hello world"));

  //TODO: homing
}


///////////////////////////////////////////////////////////////////////////////
//
//  L        OOOO    OOOO   PPPPP
//  L       O    O  O    O  P    P
//  L       O    O  O    O  PPPPP
//  L       O    O  O    O  P
//  LLLLLL   OOOO    OOOO   P
//
///////////////////////////////////////////////////////////////////////////////

void loop() {
  // put your main code here, to run repeatedly:
  switch(opMode){
    case MD_COMMAND:
        switch(loadCommandFromSerial(&command)){
          case -1:
            Serial.println("ERR");
            break;
          case 1:
            command.execute();
            Serial.println("OK");
            break;
          case 2:
            break;
          case 112:
            //TODO: e-stop handling
            break;
        }
      break;
    case MD_PROGRAM:
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//  L        OOOO     AA    DDDD
//  L       O    O   A  A   D   D
//  L       O    O  A    A  D    D
//  L       O    O  AAAAAA  D   D
//  LLLLLL   OOOO   A    A  DDDD
//
//  SSSSSS  EEEEEE  RRRRR   IIIIII    AA    L
//  S       E       R    R    II     A  A   L
//  SSSSSS  EEE     RRRRR     II    A    A  L
//       S  E       R   R     II    AAAAAA  L
//  SSSSSS  EEEEEE  R    R  IIIIII  A    A  LLLLLL
//
///////////////////////////////////////////////////////////////////////////////


/*  int loadCommandFromSerial(Command* commandPtr)
    > loads command from serial
    args:
      Command* commandPtr: pointer to command object
    returns int of load status
      status -1 indicates failure
      status 1 indicates load successful
      status 2 indicates no available serial command
      status 112 indicates a need to perform emergency stop
*/
int loadCommandFromSerial(Command* commandPtr){
  Command& commandCurrent = *commandPtr;
  ReadState readState{RD_CMDADDR};
  bool isAvailable{};
  bool isValidCommand{false};
  bool isEmergencyStop{false};
  String incomingString{};
  char incoming{};
  char cmdAddr{};
  String cmdValString{};
  int cmdVal{};
  int cmdStatus{};

  int loadStatus{-1};

  //get one line at a time
  if(Serial.available()){
    incomingString = Serial.readStringUntil('\n') + '\n';
  }else{
    loadStatus = 2;
  }

  // while(isAvailable){
  if(incomingString.length() > 1){
    for(int chridx{}; chridx < incomingString.length(); chridx++){
      incoming = incomingString.charAt(chridx);

      switch(readState){
        case RD_CMDADDR:
          //check for alphabet
          //alphabet is start of new command address
          if(isupper(toupper(incoming))){
            cmdAddr = incoming;
            readState = RD_CMDVAL;
          }
          break;
        case RD_CMDVAL:
          if(isdigit(incoming)){
            cmdValString += incoming;
          }
          if(isspace(incoming) || isupper(toupper(incoming))){
            chridx--;
            cmdVal = cmdValString.toInt();
            readState = INIT_CMD;
          }
          break; //end RD_CMDVAL
        case INIT_CMD:
          chridx--; //re-read the previous character
          cmdStatus = commandCurrent.commandInit(cmdAddr, cmdVal);
          switch(cmdStatus){
            case -1:
              readState = RD_END;
              break;
            case 2:  //need PRY
              isValidCommand = true;
              readState = RD_CMDARG;
              break;
            case 8:  //complete
              chridx = incomingString.length()-2;
              isValidCommand = true;
              readState = RD_END;
              break;
            case 112:  //emergency stop
              chridx = incomingString.length()-2;
              isValidCommand = true;
              isEmergencyStop = true;
              readState = RD_END;
              break;
          }
          break; //end INIT_CMD
        case RD_CMDARG:
          if(!isspace(incoming)) chridx--;
          { // process RD_CMDARG
            String cmdArgString{};
            int argStatus{6};
            float cmdArg[10] = {0};
            
            for(; chridx < incomingString.length(); chridx++){
              incoming = incomingString.charAt(chridx);
              switch(cmdStatus){
                case 2:
                  //figure out which move argument it is
                  if(isupper(toupper(incoming))){
                    switch(toupper(incoming)){
                      case 'X': //x direction/axis
                        argStatus = 0;
                        break;
                      case 'Y': //y direction/axis
                        argStatus = 1;
                        break;
                      case 'Z': //z direction/axis
                        argStatus = 2;
                        break;
                      case 'W': //yaw axis
                        argStatus = 3;
                        break;
                      case 'P': //pitch axis
                        argStatus = 4;
                        break;
                      case 'R': //roll axis
                        argStatus = 5;
                        break;
                      case 'F': //linear speed
                        argStatus = 6;
                        break;
                      case 'A': //linear accl
                        argStatus = 7;
                        break;
                      case 'E': //angular speed
                        argStatus = 8;
                        break;
                      case 'B': //angular accl
                        argStatus = 9;
                        break;
                    } //end switch
                  }

                  //accept negative and decimal points (floating pts)
                  if(isdigit(incoming) || incoming == '.' || incoming == '-'){
                    cmdArgString += incoming;
                  }

                  //insert arg to arg array
                  if((isspace(incoming) || isupper(toupper(incoming))) && cmdArgString.length() > 0){
                    cmdArg[argStatus] = cmdArgString.toFloat();
                    cmdArgString = "";
                    if(incoming == '\n'){ //EOL. push argument to command object
                      commandCurrent.commandArgMove(cmdArg);
                    }
                  }

                  break; //end cmdStatus 2
              } //end process RD_CMDARG
            } //end readarg
            chridx -= 2;
            readState = RD_END;
          }
          break; //end RD_CMDARG
        case RD_END: //end reading from serial
          if(isValidCommand){
            if(isEmergencyStop){
              loadStatus = 112; //emergency stop
            }else{
              loadStatus = 1; //success
            }
          }else{
            loadStatus = -1; //error 
          }
          break; //end RD_END
      }//end readstate switch
    } //end readcmd for-loop
  } //end if(incomingString.length() > 1)
  return loadStatus;
} //end loadCommandFromSerial()
