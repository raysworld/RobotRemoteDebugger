#ifndef ROBOTCLIENT_H
#define ROBOTCLIENT_H

#include "../stdafx.h"

enum MOTOR_DIR {MOTOR_FORWARD, MOTOR_BACKWARD};
enum MOTOR_STATUS {MOTOR_ENALBED, MOTOR_DISABLED};
enum HAND_STATUS {HAND_OPENED=0, HAND_CLOSED=1};
enum GRIPPER_STATUS {GRIPPER_OPENED=0, GRIPPER_CLOSED=1};

void sendCmd(std::string cmd, char *respMsg);

/************************************************************************/
/* Functions                                                            */
/************************************************************************/
    /* functions for hand ***********************************************/
    std::string ControlHand(HAND_STATUS status);

    /* functions for gripper ********************************************/
    std::string ControlGripper(GRIPPER_STATUS status);

    /* functions for step motor *****************************************/
    std::string ControlStepMotor(MOTOR_DIR dir);
    std::string ControlStepMotor_Stop(MOTOR_STATUS status);
    std::string ControlStepMotor_Move(MOTOR_DIR dir, short cm, short mm);
    std::string ControlStepMotor_Speed(short speed);

    /* functions for robot **********************************************/
    std::string ControlRobot_SetSpeed(short speed);
//    std::string ControlRobot_SetJoint(float *joints);
//    std::string ControlRobot_SetXyzwpr(float *xyzwpr, short *config);
    std::string ControlRobot_SetJoint(float *joints, bool isSoftFloat = false, short sfIdx = 0);
    std::string ControlRobot_SetXyzwpr(float *xyzwpr, short *config, bool isSoftFloat = false, short sfIdx = 0);
    std::string ControlRobot_GetCoord();
    void Cmd2Coord(std::string cmd, float *xyzwpr, short *config, float *joints);


#endif // ROBOTCLIENT_H
