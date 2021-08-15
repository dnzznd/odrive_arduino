/* Odrive */
#include <ODriveTool.h>

/* ROS */
#include <ros.h>
#include <ros/time.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <ArduinoHardware.h>
#include <geometry_msgs/Twist.h>
/******/

// Arduino Mega or Due - Serial1
// pin 19: RX - connect to ODrive TX
// pin 18: TX - connect to ODrive RX
// See https://www.arduino.cc/reference/en/language/functions/communication/serial/ for other options
//HardwareSerial& odrive_serial = Serial1;

// Serial1はrosserialの方で使用されるため、ArduinoとOdriveはSerial2でUART通信を行う
// pin 17: RX - connect to ODrive TX
// pin 16: TX - connect to ODrive RX
//HardwareSerial& odrive_serial = Serial2;

// ODrive object
//ODriveTool odrive(Serial1);
ODriveTool odrive(Serial2);
//ODriveArduino odrive(odrive_serial);

void messageCb(const geometry_msgs::Twist& msg);

/* ROS */
ros::NodeHandle  nh;
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &messageCb);

/***********************************************************************
 * Global variables
 **********************************************************************/
const int kv = 16;
const int encoder_cpr = 90;
//const float pi = 3.14159262;

float w_r = 0.0;
float w_l = 0.0;

//wheel_rad is the wheel radius ,wheel_sep is
float wheel_rad = 0.085;
float wheel_sep = 0.32;

float speed_ang = 0.0;
float speed_lin = 0.0;

float vel1,vel2;

void ros_init()
{
    nh.getHardware()->setBaud(115200);
    nh.initNode();
    nh.subscribe(sub);
}

void setup() {
  odrive.odrive_reboot();

  for (int axis = 0; axis < 2 ; axis++) {
    //odrive.odrive_init(axis);
    odrive.odrive_init(axis, 5000.0f, 20.0f);
    delay(300);
  }

  ros_init();
}

void loop() {
  nh.spinOnce();
  delay(500);
}

void messageCb(const geometry_msgs::Twist& msg){
  speed_ang = msg.angular.z;
  speed_lin = msg.linear.x;
  w_r = (speed_lin/wheel_rad) + ((speed_ang*wheel_sep)/(2.0*wheel_rad));
  w_l = (speed_lin/wheel_rad) - ((speed_ang*wheel_sep)/(2.0*wheel_rad));

  //vel1 = w_r;
  //vel2 = w_l;
  vel1 = 2.0f;
  vel2 = 2.0f;
  odrive.SetVelocity(0, vel1);
  odrive.SetVelocity(1, vel2);
}