#include "../stdafx.h"

#include "RobotClient.h"
#include "RobotObject.h"


void sendCmd(std::string cmd, char *respMsg)
{
#ifndef USE_QT
	WORD wVersionRequested;
    WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return;
	}

    // Initialize
	SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = inet_addr("11.0.0.41");
	addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6600);

	std::cout<<"Connecting to Remote..."<<std::endl;
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
    connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	std::cout<<"Connected!"<<std::endl;

    // Receive connection message
	char recvBuf[256];
	while(recv(sockClient, recvBuf, 256, 0) == -1);
	std::cout<<recvBuf<<std::endl;

    // send command
	send(sockClient, cmd.c_str(), cmd.size() + 1, 0);

    // receive return message
	recv(sockClient, respMsg, 256, 0);
	std::cout<<respMsg<<std::endl;

	closesocket(sockClient);
	WSACleanup();
#else
    QTcpSocket socket;
    char recvBuf[256] = "";

    socket.connectToHost("11.0.0.41", 6600);
    std::cout<<"Connecting to Remote..."<<std::endl;
    socket.waitForConnected();

    std::cout<<"Connected!"<<std::endl;

    // Receive connection message
    bool flag = socket.waitForReadyRead();
    if(!flag)
    {
        std::cout<<"Read Error!"<<std::endl;
        return;
    }
    socket.read(recvBuf, 256);
    std::cout<<recvBuf<<std::endl;

    // send command
    socket.write(cmd.c_str());
    socket.waitForBytesWritten();

    // receive return message
    flag = socket.waitForReadyRead();
    if(!flag)
    {
        std::cout<<"Read Error!"<<std::endl;
        return;
    }
    socket.read(respMsg, 256);
    std::cout<<respMsg<<std::endl;
#endif
}

std::string ControlHand(HAND_STATUS status)
{
    // CMD1 - Control Robot Hand
    std::string sCmd = "CMD1";

    switch (status) {
    case HAND_OPENED:
        sCmd += "1EOF";
        break;
    case HAND_CLOSED:
        sCmd += "0EOF";
        break;
    default:
        sCmd = "Hand Status Error!";
        break;
    }

    return sCmd;
}

std::string ControlGripper(GRIPPER_STATUS status)
{
    // CMD2 - Control Gripper
    std::string sCmd = "CMD2";

    switch (status) {
    case GRIPPER_OPENED:
        sCmd += "1EOF";
        break;
    case GRIPPER_CLOSED:
        sCmd += "0EOF";
        break;
    default:
        sCmd = "Gripper Status Error!";
        break;
    }

    return sCmd;
}

std::string ControlStepMotor(MOTOR_DIR dir)
{
    // CMD3 - Control Step Motor
    std::string sCmd = "CMD33"; // Control Step Motor - Run

    switch (dir) {
    case MOTOR_FORWARD:
        sCmd += "0EOF";
        break;
    case MOTOR_BACKWARD:
        sCmd += "1EOF";
        break;
    default:
        sCmd = "Step Motor Instruction Error!";
        break;
    }

    return sCmd;
}

std::string ControlStepMotor_Stop(MOTOR_STATUS status)
{
    // CMD3 - Control Step Motor
    std::string sCmd = "CMD30"; // Control Step Motor - Stop

    switch (status) {
    case MOTOR_ENALBED:
        sCmd += "0EOF";
        break;
    case MOTOR_DISABLED:
        sCmd += "1EOF";
        break;
    default:
        sCmd = "Step Motor Stop Instruction Error!";
        break;
    }

    return sCmd;
}

std::string ControlStepMotor_Move(MOTOR_DIR dir, short cm, short mm)
{
    // CMD3 - Control Step Motor
    std::ostringstream ossCmd;

    ossCmd << "CMD31"; // Control Step Motor - Move

    switch (dir) {
    case MOTOR_FORWARD:
        ossCmd << "0";
        break;
    case MOTOR_BACKWARD:
        ossCmd << "1";
        break;
    default:
        return("Step Motor Stop Instruction Error!");
    }

    ossCmd << std::setfill('0');
    ossCmd << std::setw(2) << cm;
    ossCmd << std::setw(1) << mm;
    ossCmd << "EOF";

    return ossCmd.str();
}

std::string ControlStepMotor_Speed(short speed)
{
    // CMD3 - Control Step Motor
    std::ostringstream ossCmd;

    ossCmd << "CMD32"; // Control Step Motor - Speed

    ossCmd << std::setfill('0');
    ossCmd << std::setw(2) << speed;
    ossCmd << "EOF";

    return ossCmd.str();
}

std::string ControlRobot_SetSpeed(short speed)
{
    // CMD0 - Control Robot
    std::ostringstream ossCmd;

    ossCmd << "CMD012"; // Control Robot - Set - Speed

    ossCmd << std::setfill('0');
    ossCmd << std::setw(3) << speed;
    ossCmd << "EOF";

    return ossCmd.str();
}

std::string ControlRobot_SetJoint(float *joints, bool isSoftFloat, short sfIdx)
{
    // CMD0 - Control Robot
    std::ostringstream ossCmd;

    ossCmd << "CMD01"; // Control Robot - Set

    if(isSoftFloat)
    {
        ossCmd << (3 + sfIdx);// SOFTFLOAT[sfIdx]
    }
    else
    {
        ossCmd << "0";          // Current Position
    }

    ossCmd << "1"; // Joint

    ossCmd << frameInfo.uf; //UF
    ossCmd << frameInfo.ut; //UT

    ossCmd << "J";
    for(int i=0; i<9; i++)
    {
        if (i != 8) ossCmd << joints[i] <<",";
        else ossCmd << joints[i];
    }
    ossCmd << "J";

    ossCmd << "EOF";

    return ossCmd.str();
}

std::string ControlRobot_SetXyzwpr(float *xyzwpr, short *config, bool isSoftFloat, short sfIdx)
{
    // CMD0 - Control Robot
    std::ostringstream ossCmd;

    ossCmd << "CMD01"; // Control Robot - Set

    if(isSoftFloat)
    {
        ossCmd << (3 + sfIdx);// SOFTFLOAT[sfIdx]
    }
    else
    {
        ossCmd << "0";          // Current Position
    }

    ossCmd << "0"; // Xyzwpr

    ossCmd << frameInfo.uf; //UF
    ossCmd << frameInfo.ut; //UT

    ossCmd << "X";
    for(int i=0; i<9; i++)
    {
        if (i != 8) ossCmd << xyzwpr[i] <<",";
        else ossCmd << xyzwpr[i];
    }
    ossCmd << "X";

    ossCmd << "G";
    for(int i=0; i<7; i++)
    {
        if (i != 6) ossCmd << config[i] <<",";
        else ossCmd << config[i];
    }
    ossCmd << "G";

    ossCmd << "EOF";

    return ossCmd.str();
}

std::string ControlRobot_GetCoord()
{
    // CMD0 - Control Robot
    std::ostringstream ossCmd;

    ossCmd << "CMD000"; // Control Robot - Get - Current Position

    ossCmd << frameInfo.uf; //UF
    ossCmd << frameInfo.ut; //UT
    ossCmd << "EOF";

    return ossCmd.str();
}

void Cmd2Coord(std::string cmd, float *pXyzwpr, short *pConfig, float *pJoints)
{
    // Header
    std::string sCmdHeader = cmd.substr(0,3);
    if (0 != sCmdHeader.compare("RTN"))  { return;	}

	frameInfo.uf = cmd[4] - '0';
	frameInfo.ut = cmd[5] - '0';	
	
    // extract xyzwpr
    if(nullptr != pXyzwpr)
    {
        auto idx_x1 = cmd.find_first_of('X');
        auto idx_x2 = cmd.find_last_of('X');
        string2value(cmd.substr(idx_x1+1, idx_x2-idx_x1-1), pXyzwpr);
    }

    // extract config
    if(nullptr != pConfig)
    {
        auto idx_g1 = cmd.find_first_of('G');
        auto idx_g2 = cmd.find_last_of('G');
        string2value(cmd.substr(idx_g1+1, idx_g2-idx_g1-1), pConfig);
    }

    // extract joints
    if(nullptr != pJoints)
    {
        auto idx_j1 = cmd.find_first_of('J');
        auto idx_j2 = cmd.find_last_of('J');
        string2value(cmd.substr(idx_j1+1, idx_j2-idx_j1-1), pJoints);
    }
}
