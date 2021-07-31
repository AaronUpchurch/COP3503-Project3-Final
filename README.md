# COP3503-Project3-Final
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	To Run Code From Github On Visual Studio
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1. Create a new folder
2. In the new folder's command line interface type "git clone https://github.com/AaronUpchurch/COP3503-Project3-Final"
3. Create a new empty visual studio project
4. From the clone folder, copy and paste the following .h files into the header file folder in the solution explorer
	bhTree.h
	Particle.h
	Rand.h
5. From the clone folder, copy and paste the following .cpp files into the Source file folder in the solution explorer
	Project3A.cpp
6. Open the new visual studio project's file in file explorer
7. Go into the inner folder that contains several vcxproj files
8. Copy and paste all files from the clone folder into this project folder
9. In visual studio, open the Project3A.cpp file from the solution explorer by double clicking on it
9. In visual studio click, under the Project tab, click on properties located at the bottom
10. Under the C/C++ -> General tab, add the following to "Additional Include Directories"
	...pathToProject....SFML-2.5.1-windows-vc15-64-bit\SFML-2.5.1\include
	ex) C:\Users\aaron\source\repos\COP3530-Project3-Final\COP3530-Project3-Final\SFML-2.5.1-windows-vc15-64-bit\SFML-2.5.1\include
11. Under the Linker -> General tab add the following to "Additional Library Directoriest
	...pathToProject...SFML-2.5.1-windows-vc15-64-bit\SFML-2.5.1\lib
	ex) C:\Users\aaron\source\repos\COP3530-Project3-Final\COP3530-Project3-Final\SFML-2.5.1-windows-vc15-64-bit\SFML-2.5.1\lib
12. Under the Linker -> Input tab, add the following to "Additional Dependencies"
	While in Release Configuration
		sfml-audio.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib
	While in Debug Configuration
		sfml-audio-d.lib;sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib
13. Under the Build Tab, click "Configuration Manager"
14. Change Active Solution Platform to x64
15. Run the program
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					Program Controls
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Pause: Space
Zoom In: S
Zoom Out: A
Pan Left: Left Arrow
Pan RightL Right Arrow
Pan Up: Up Arrow
Pan Down: Down Arrow
Add Particles: Left Click
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				How Program Works
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
The entire simulation relies on two arrays

	1. The particle araray "Particles"
		Particles store the mathematical components of each astroid in the simulation
		Each particle stores values such as x position, y position, mass, x velocity and y velocity
		At each iteration of the simulation, all particles are added into the Barnes-Hill tree
	2. The CircleShape array "Circles"
		Circles store the graphical components of each astroid in the simulation
		Each circle stores the information required to display the astroid onto the window

The two arrays ineract in the following manner
	
	Each astroid in the simulation is reperesented by a single particle and a single Circlshape
	The x and y velocty contained within each particle point directly to its corresponding CircleShape
	This removes the need for constantly updating the CircleShape's position on the screen
	
		Particle		CircleShape
		x --------------------> xPosition
		y --------------------> yPosition
		mass			Color
		xVel			Size
		yVel			Radius
		....                    ....
		 
