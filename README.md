# FRC-LEDs

The goal of this project is to create an easy to use set of code to enable an FRC team to add LED lighting effects to their robot.

## Architecture

The system has two components.

They are ... not done yet.

1. **Rioduino** – We (will) use a REV Robotics Rioduino to interface with the RoboRIO via I2C and to control ws2812b LEDs.
2. **WPILib Components** – We (will) write components that fit into an FRC robot's code in a way that is consistent with other WPILib components. This will allow easily sending commands to the Rioduino to change modes.

## Principles

This project should be extremely easy for teams to integrate with their robot. The goal is provide an easy to use module to add LED lighting, while also enabling people to go deeper into writing custom LED animation code or creating more sophisticated visual feedback.

A major goal is to AVOID getting stuck in "why can't I get the serial communication to work?" mode, and to enable prototyping without a RoboRIO available so that students can work on this module without immediate access to their robot.

## Arduino Code

This project is designed to be used with a REV Robotics Rioduino, which provides easy communication between a RoboRIO and an Arduino compatible microcontroller through I2C communication via the RoboRIO's MXP port.

### I2C

There is example code for Arduinos to receive I2C messages in the AndyMark "RIOduino and Display Examples (zip)" demo for the Revduino at http://www.andymark.com/product-p/am-2999.htm

## Robot Code

I'm planning to write Java code, since I mentor Team 5507 and we use Java.

### WPILib docs

* http://first.wpi.edu/FRC/roborio/release/docs/java/edu/wpi/first/wpilibj/I2C.html
  * http://first.wpi.edu/FRC/roborio/beta/docs/java/edu/wpi/first/wpilibj/I2C.Port.html to specify MXP or Onboard

## Inspiration

This project is inspired by several other projects.

* https://github.com/6d6163/FRC_Status_LEDs
* https://github.com/frc-4931/2015-Robot/blob/master/RobotFramework/src/org/frc4931/robot/component/RIODuino.java
* https://github.com/FRC2706/2017-2706-Robot-Code/blob/master/src/org/usfirst/frc/team2706/robot/subsystems/Bling.java
* https://github.com/FRC-6420/RIODuino
