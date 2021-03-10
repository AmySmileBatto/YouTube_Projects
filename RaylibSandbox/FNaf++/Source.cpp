#include <raylib.h>
#include <raymath.h>
/*************************************************************************
* 
*	This project uses Raylib (https://www.raylib.com/)
*	Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*	
*	C/C++
*		General
*			Additional Include Directories
*			====================================
*			$(SolutionDir)Library\raylib\include
*			====================================
* 
*	Linker
*		General
*			Additional Library Directories
*			=========================================================
*			$(SolutionDir)Library\raylib\bin\x64\$(Configuration).DLL
*			=========================================================
* 
*		Input
*			Additional Dependencies
*			============
*			raylib.lib
			kernel32.lib
			user32.lib
			gdi32.lib
			winmm.lib
			winspool.lib
			comdlg32.lib
			advapi32.lib
			shell32.lib
			ole32.lib
			oleaut32.lib
			uuid.lib
			odbc32.lib
			odbccp32.lib
*			============
* 
**************************************************************************/

int main() {
	int windowWidth = 1280;
	int windowHeight = 720;
	InitWindow(windowWidth, windowHeight, "FNaF++");
	SetTargetFPS(60);
	// Load memory & declare frame-global variables (variables that persist across frames)
	// TODO

	while (!WindowShouldClose()) {
		// Update game variables
		// TODO

		// Draw the frame
		BeginDrawing(); {

			// Rendering
			// TODO

		} EndDrawing();
	}
	// Unload & free memory
	// TODO

	CloseWindow();
	return 0;
}