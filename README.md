# Swarm Robots-Graduation project 2018

## Swarm Intelligence:
SI systems are typically made up of a population of simple agents interacting locally with one another and with their environment. The group of individuals acting in such a manner is referred to as a swarm. The term Stigmergy is used to describe the indirect form of communication between individuals in a swarm via environment (one individual modiﬁes the environment, which in return modiﬁes the behavior of other individuals - they respond to the change). Individuals within the group interact by exchanging locally available information such that the problem (global objective) is solved more efﬁciently it would be done by a single individual. Problem-solving behavior that emerges from such interactions is called swarm intelligence. Algorithmic models of that behavior are called Computational Swarm Intelligence (CSI). For the simplicity, the name most frequently used is just "swarm intelligence".

## Project Description:

RowBot III is a custom made robot from scratch including the frame, PCB design and implementation. RowBot has three versions, the old versions were having problems such as the relatively large dimensions which weren’t compatible with swarm principle or hardware problems that made us obliged to redesign the PCB layout several times. Our goal wasn't only to design and manufacture our own robots but we also wanted to build a strong base for next teams working in the swarm robots field, to simplify the concept and implementation through designing simple, cheap, and suitable size robots.
[Robots design on "Thingiverse"](https://www.thingiverse.com/thing:2990383)

## Contents:
1. Robot library: includes functions to fit our robots specifications and enable us to focus on other tasks code without any conflicts or overheads.
	- Initializing motors speed to be 0, set the robot as not a leader, and to have no id,  and another constructor with certain id number,
	- Moving the robot in different directions by setting the speed of the motors to make the wheels move forward, backward, right, and left, with or without delay. And another function to stop the robot,
	- And another set of function to set and get different variables, and to set the robot with certain id to be a leader.

2. CV module: includes methods and functions used as a postioning system.
	- Detecting robots distinctly,
	- And localizing each of their positions, and angels.

3. Kernels:
	* Arduino Kernel: contains functions to control and communicate among robots and camera, using Arduino.
	* RTOS Kernel: contains functions to control and communicate among robots and camera, implemented on an RTOS.