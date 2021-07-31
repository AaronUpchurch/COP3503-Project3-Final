# COP3503-Project3-Final
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
To Get Code From Github To Working Visual Studio Program
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1. in folder, git clone "url"
2. Create a new visual studio empty project
3. Drag and drop 3.h files and 1 cpp files into solution explorer files
4. Drag and drop SFML folder, font, and 6 sfml-dl.2 files directly into code's folder
5. CHange C++ general and Linker general setting to it points to files inside the cloned file
6. Add shit to linker input
7. change to 64

1. Create a new folder
2. In the new folder's command line interface type "git clone <url>"
3. Create a new empty visual studio project
4. From the new folder, copy and paste the following .h files into the header file folder in the solution explorer
	bhTree.h
	Particle.h
	Rand.h
5. From the new folder, copy and paste the following .cpp files into the Source file folder in the solution explorer
	Project3A.cpp
6. Open the new visual studio project's file in file explorer
7. Go to the inner folder of the same name that contains several vcxproj files
8. Copy and paste all files from the clone folder into this project folder
9. In visual studio, open the Project3A.cpp file by double clicking on it
9. In visual studio click on properties under the Project tab
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
13. Under tehe Build Tab, click "Configuration Manager"
14. Change Active Solution Platform to x64
15. Run the program
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
