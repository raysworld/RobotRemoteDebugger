#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "RobotClient/RobotClient.h"
#include "RobotClient/RobotObject.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set initial robot speed
    robotSpeed = ui->sbSpeed->value();
    robotMotionStep = ui->sbStep->value();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbGetCoord_clicked()
{
    char recvMsg[256] = "";
    sendCmd(ControlRobot_GetCoord(), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);
    Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, frameInfo.Joints);

    ui->leUF->setText(QString::number((frameInfo.uf)));
    ui->leUT->setText(QString::number((frameInfo.ut)));

    string sXyzwpr = "";
    string sConfig = "";
    string sJoints = "";
    value2string(frameInfo.Xyzwpr, &sXyzwpr, 9);
    value2string(frameInfo.Config, &sConfig, 7);
    value2string(frameInfo.Joints, &sJoints, 9);

    ui->ptXyzwprValue->setPlainText(QString::fromStdString(sXyzwpr));
    ui->leConfigValue->setText(QString::fromStdString(sConfig));
    ui->ptJointValue->setPlainText(QString::fromStdString(sJoints));

    ui->lJ1Value->setText(QString::number(frameInfo.Joints[0]));
    ui->lJ2Value->setText(QString::number(frameInfo.Joints[1]));
    ui->lJ3Value->setText(QString::number(frameInfo.Joints[2]));
    ui->lJ4Value->setText(QString::number(frameInfo.Joints[3]));
    ui->lJ5Value->setText(QString::number(frameInfo.Joints[4]));
    ui->lJ6Value->setText(QString::number(frameInfo.Joints[5]));

    ui->lXValue->setText(QString::number(frameInfo.Xyzwpr[0]));
    ui->lYValue->setText(QString::number(frameInfo.Xyzwpr[1]));
    ui->lZValue->setText(QString::number(frameInfo.Xyzwpr[2]));
    ui->lWValue->setText(QString::number(frameInfo.Xyzwpr[3]));
    ui->lPValue->setText(QString::number(frameInfo.Xyzwpr[4]));
    ui->lRValue->setText(QString::number(frameInfo.Xyzwpr[5]));

    ui->lConfigInfo->setText(QString::fromStdString(sConfig));
}

void MainWindow::on_sbSpeed_valueChanged(int arg1)
{
    robotSpeed = arg1;
}

void MainWindow::on_sbStep_valueChanged(int arg1)
{
    robotMotionStep = arg1;
}

void MainWindow::on_pbSetJoint_clicked()
{
    if (ui->ptJointValue->toPlainText().isEmpty()) {
        return;
    }
    string2value(ui->ptJointValue->toPlainText().toStdString(), frameInfo.Joints);

    char recvMsg[256] = "";
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);
    sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);
}

void MainWindow::on_pbSetCartesian_clicked()
{
    if (ui->ptXyzwprValue->toPlainText().isEmpty() ||
            ui->leConfigValue->displayText().isEmpty()) {
        return;
    }
    string2value(ui->ptXyzwprValue->toPlainText().toStdString(), frameInfo.Xyzwpr);
    string2value(ui->leConfigValue->displayText().toStdString(), frameInfo.Config);

    frameInfo.ut = ui->leUT->displayText().toShort();
    frameInfo.uf = ui->leUF->displayText().toShort();

    char recvMsg[256] = "";
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);
    sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);
}

void MainWindow::on_pbRDOOn_clicked()
{
    char recvMsg[256] = "";
    switch (ui->sbRDO->value()) {
    case 1:case 2:
        sendCmd(ControlHand(HAND_OPENED), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        break;
    case 3:
        sendCmd(ControlGripper(GRIPPER_OPENED), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        break;
    default:
        break;
    }
}

void MainWindow::on_pbRDOOff_clicked()
{
    char recvMsg[256] = "";
    switch (ui->sbRDO->value()) {
    case 1:case 2:
        sendCmd(ControlHand(HAND_CLOSED), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        break;
    case 3:
        sendCmd(ControlGripper(GRIPPER_CLOSED), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        break;
    default:
        break;
    }
}



void MainWindow::on_pbJ1pos_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[0] = frameInfo.Joints[0] + robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[0] = frameInfo.Xyzwpr[0] + robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ1neg_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[0] = frameInfo.Joints[0] - robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[0] = frameInfo.Xyzwpr[0] - robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ2pos_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[1] = frameInfo.Joints[1] + robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[1] = frameInfo.Xyzwpr[1] + robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ2neg_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[1] = frameInfo.Joints[1] - robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[1] = frameInfo.Xyzwpr[1] - robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ3pos_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[2] = frameInfo.Joints[2] + robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[2] = frameInfo.Xyzwpr[2] + robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ3neg_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[2] = frameInfo.Joints[2] - robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[2] = frameInfo.Xyzwpr[2] - robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}



void MainWindow::on_pbJ4pos_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[3] = frameInfo.Joints[3] + robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[3] = frameInfo.Xyzwpr[3] + robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ4neg_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[3] = frameInfo.Joints[3] - robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[3] = frameInfo.Xyzwpr[3] - robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ5pos_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[4] = frameInfo.Joints[4] + robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[4] = frameInfo.Xyzwpr[4] + robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ5neg_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[4] = frameInfo.Joints[4] - robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[4] = frameInfo.Xyzwpr[4] - robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ6pos_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[5] = frameInfo.Joints[5] + robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[5] = frameInfo.Xyzwpr[5] + robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_pbJ6neg_clicked()
{
    char recvMsg[256] = "";

    // Set speed first
    sendCmd(ControlRobot_SetSpeed(robotSpeed), recvMsg);
    ui->lwMsg->insertItem(0, recvMsg);

    if (ui->rbJoint->isChecked()) {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, nullptr, nullptr, frameInfo.Joints);

        frameInfo.Joints[5] = frameInfo.Joints[5] - robotMotionStep;
        sendCmd(ControlRobot_SetJoint(frameInfo.Joints), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
    else {
        sendCmd(ControlRobot_GetCoord(), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
        Cmd2Coord(recvMsg, frameInfo.Xyzwpr, frameInfo.Config, nullptr);

        frameInfo.Xyzwpr[5] = frameInfo.Xyzwpr[5] - robotMotionStep;
        sendCmd(ControlRobot_SetXyzwpr(frameInfo.Xyzwpr, frameInfo.Config), recvMsg);
        ui->lwMsg->insertItem(0, recvMsg);
    }
}

void MainWindow::on_actionIP_Config_triggered()
{

}
