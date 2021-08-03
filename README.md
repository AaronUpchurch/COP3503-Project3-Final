# COP3503-Project3-Final
+++++++++++++++++++
For TA Grader
+++++++++++++++++
The files we created were Rand.h, bhTree.h, particle.h, and Project3A.cpp
All other particles are not written by us

+++++++++++++++++++
Sources
+++++++++++++++++++
SFML: https://www.sfml-dev.org/tutorials/2.5/start-vc.php
Font: https://www.fontsquirrel.com/fonts/Unibody-8
++++++++++++++++++++
  To Run The Code
++++++++++++++++++++
1. Git Clone the executable folder
2. Run the executable
+++++++++++++++++++++
Program Controls
++++++++++++++++++++
Pause: Space
Zoom In: S
Zoom Out: A
Pan Left: Left Arrow
Pan RightL Right Arrow
Pan Up: Up Arrow
Pan Down: Down Arrow
Add Particles: Left Click
+++++++++++++++++
How Program Works
++++++++++++++++
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
		 
