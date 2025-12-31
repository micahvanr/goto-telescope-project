# Goto Telescope Project

The goal of this project is to create a microcontroller that will interact with other modules through 
various interfaces to get information and control the telescope. Specifically, it will get the time, 
the current location in latitude and longitude, the orientation of the telescope tube, and control 
the telescope to not only find different objects but track them as well. With that information and 
the current time, it will calculate and display on a small screen what constellations, planets or 
other points of interest are above you. In addition, I will be using git/github and docker to create 
a CI (continuous integration) system. This CI system will just be for gaining experience with it and 
only for myself so it will not be as thorough as one might be in a professional setting.

## Directory Structure
| Directory 		| Description															|
|-------------------|-----------------------------------------------------------------------|
| build/ 			| Build output (object files + executables)								|
| docs/ 			| Documentation (ex. coding standards,images)							|
| external/ 		| External dependencies													|
| src/				| Source files (.c/.h) and linker script and startup file               |
| src/app/			| Source files for application layer									|
| src/bsp/			| Source files for other modules and chips (board support package layer)|
| src/drivers/		| Source files for drivers layer										|
| src/common/		| Source files for code that drivers and app use						|
| src/test/			| Source files for test code											|

## Build
The two ways I will be building this project is through a makefile and through an IDE (STM32CubeIDE in this case). The IDE is easier to debug and test the code 
while the makefile is useful to integrate into the CI system.
 
### Make (Makefile)
WIP

### IDE
WIP

## Tests
WIP

## Pushing a new change or feature
WIP

## Commit message
* One change per commit:
    * Each commit should make one change or addition in order for them to be organized well and the commits will be clear.
* Commits should build (if applicable):
    * Each commit should be able to be built. 
* Commit messages should have the following structures:
Subject Line - 		{\<type>:\<scope>:\<description>}	(Describe solution not problem)  <br>	
Blank Line - 		{\n}  <br>
Body - 		   		{Explain what and why}  <br>
Blank Line - 		{\n}  <br>
Optional Footer - 	{Any additional comments or what it connects to}  <br>
 <br>
Ex commit:  <br>
git -commit -m "feat(UART): Add UART drivers  <br>
 <br>
UART drivers are required for GPS locator <br>
 <br>
n/a <br>
"
	
* \<type> - List of types
    * feat
    * fix
    * modify

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