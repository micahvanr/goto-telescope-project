# Goto Telescope Project

## Intro
The goal of this project is to create a microcontroller that will interact with other modules through 
various interfaces to get information and control the telescope. Specifically, it will get the time, 
the current location in latitude and longitude, the orientation of the telescope tube, and control 
the telescope to not only find different objects but track them as well. With that information and 
the current time, it will calculate and display on a small screen what constellations, planets or 
other points of interest are above you. The project will be developed in two parts: first the controller and then the telescope.

## Outline
* Custom controller and telescope "harness" to control telescope
* Driver, application, linker script, and startup code written from scratch
* Building with Makefile
* Static analysis using cppcheck
* Unit testing with the Unity framework
* Formatting using clang-format
* Git version control

### Controller 
* Bulk of the development. I will be creating a small controller with a 4 x 20 LCD screen, GPS locator, and sliders and buttons for input.
* Custom PCB with an STM32F407 MCU.
* Custom 3D printed controller enclosure and buttons.
* The controller should have the hardware support for the telescope features so I can use the same PCB.

### Telescope
* Custom 3D printed telescope "harness" to attach to the telescope with no drilling into telescope required.
* Motor controller driver to control motors through PWM signal.
* Gyroscope to provide controller with orientation of optical tube.

## Directory Structure
Based on the pitchfork layout

| Directory 		| Description															|
|-------------------|-----------------------------------------------------------------------|
| build/ 			| Build output (object files + executables)								|
| docs/ 			| Documentation (ex. coding standards, images)							|
| external/ 		| External dependencies													|
| src/				| Source files (.c/.h) contains linker script and startup file          |
| src/app/			| Source files for application layer									|
| src/bsp/			| Source files for other modules and chips (board support package layer)|
| src/drivers/		| Source files for drivers layer										|
| src/common/		| Source files for code that drivers and app use						|
| test/             | Makefile with test source files                                       |
| tools/            | Any tools used such as dockerfile and unit testing frameworks         |

## Build
The way I will be building the project is through a Makefile. I will be debugging the code through vscode with the cortex-debug extension. This will allow me to use vscode with most if not all the debugging tools as a standard IDE would. In addition, I will then still be able to use make to build my project the way I want to.
 
## Tests
I will be performing unit testing using the Unity framework with the Fake Function Framework (FFF) to create mocks. The Unity is an all C testing framework made for the embedded field. FFF similiar in that it is a simple C header implementation. I will not be performing unit tests on driver layer code. Instead I will be relying on integration and system tests to ensure the drivers work properly. 

## Pushing a new change or addition
1. Create a local branch
2. Make code changes
3. Build code
4. Flash and test code
5. Format code
6. Static analyze code
7. Commit code
8. Push the branch to GitHub
9. Pass CI/CD system (if fail rework code till it passes)
10. Open pull-request
11. Merge pull-request

## Commit message
* One change per commit:
    * Each commit should make one change or addition in order for them to be organized well and the commits will be clear.
* Commits should build (if applicable):

### Commit Structure:
Subject Line - 		{\<type>:\<scope>:\<description>}	(Describe solution not problem)  <br>	
Blank Line - 		{\n}  <br>
Body - 		   		{Explain what and why}  <br>
Blank Line - 		{\n}  <br>
Optional Footer - 	{Any additional comments or what it connects to} 

### Ex commit:  
git -commit -m "feat(UART): Add UART drivers  <br>
 <br>
UART drivers are required for GPS locator"
	
### List of types - \<type>  
* add
* fix
* modify
* del

## Code formatter
Clang is used to format the code and keep it consistent. 

## Coding guidelines
Additional coding guidelines apart from formatting are listed in docs/coding_guidelines.md file 

## Static Analysis
Cppcheck is a static analysis tool for C/C++ that will detect more bugs and possible dangerous behavior. I will be using it to perform static analysis on my code. 

## Diagrams
WIP

## Project roadmap
WIP 

## Project screens
<img src="/docs/Telescope Project Screens.png">

## Software architecture
<img src="/docs/Telescope Project Hardware Architecture.png">
<img src="/docs/Telescope Project Software Architecture.png">

## Schematic
WIP

## Block diagram
WIP