#include <raylib.h>
#include <raymath.h>
#include <random>
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

enum class Cam {
	Cam_1A,		// Show stage
	Cam_1B,		// Dining hall
	Cam_1C,		// Pirate Cove
	Cam_2A,		// West hall
	Cam_2B,		// West corner
	Cam_3,		// Supply closet
	Cam_4A,		// East hall
	Cam_4B,		// East corner
	Cam_5,		// Backstage
	Cam_6,		// Kitchen
	Cam_7,		// Bathrooms
};

namespace Renders {

	Texture2D g_FreddyRenders[]{
	#if _DEBUG
		LoadTexture("Freddy_Debug"),	// Show stage
		g_FreddyRenders[0],				// Dining hall
		g_FreddyRenders[0],				// Bathrooms
		g_FreddyRenders[0],				// Kitchen
		g_FreddyRenders[0],				// East hall
		g_FreddyRenders[0],				// East corner
		g_FreddyRenders[0],				// East door
	#else // TODO
		LoadTexture(""),				// Show stage
		LoadTexture(""),				// Dining hall
		LoadTexture(""),				// Bathrooms
		LoadTexture(""),				// Kitchen
		LoadTexture(""),				// East hall
		LoadTexture(""),				// East corner
		LoadTexture(""),				// East door
	#endif
	};
	Texture2D g_FoxyRenders[]{
	#if _DEBUG
		LoadTexture("Foxy_Debug"),		// Pirate Cove (0)
		g_FoxyRenders[0],				// Pirate Cove (1)
		g_FoxyRenders[0],				// Pirate Cove (2)
		g_FoxyRenders[0],				// West hall (animated) TODO
		g_FoxyRenders[0],				// West door (animated) TODO
	#else // TODO
		LoadTexture(""),				// Pirate Cove (0)
		LoadTexture(""),				// Pirate Cove (1)
		LoadTexture(""),				// Pirate Cove (2)
		LoadTexture(""),				// West hall (animated) TODO
		LoadTexture(""),				// West door (animated) TODO
	#endif
	};
	Texture2D g_BonnieRenders[]{
	#if _DEBUG
		LoadTexture("Bonnie_Debug"),	// Show stage
		g_BonnieRenders[0],				// Dining hall
		g_BonnieRenders[0],				// Backstage
		g_BonnieRenders[0],				// West hall
		g_BonnieRenders[0],				// Storage closet
		g_BonnieRenders[0],				// West corner
		g_BonnieRenders[0],				// West door
	#else // TODO
		LoadTexture(""),				// Show stage
		LoadTexture(""),				// Dining hall
		LoadTexture(""),				// Backstage
		LoadTexture(""),				// West hall
		LoadTexture(""),				// Storage closet
		LoadTexture(""),				// West corner
		LoadTexture(""),				// West door
	#endif
	};
	Texture2D g_ChicaRenders[]{
	#if _DEBUG
		LoadTexture("Chica_Debug"),		// Show stage
		g_ChicaRenders[0],				// Dining hall
		g_ChicaRenders[0],				// Bathrooms
		g_ChicaRenders[0],				// Kitchen
		g_ChicaRenders[0],				// East hall
		g_ChicaRenders[0],				// East corner
		g_ChicaRenders[0],				// East door
	#else // TODO
		LoadTexture(""),				// Show stage
		LoadTexture(""),				// Dining hall
		LoadTexture(""),				// Bathrooms
		LoadTexture(""),				// Kitchen
		LoadTexture(""),				// East hall
		LoadTexture(""),				// East corner
		LoadTexture(""),				// East door
	#endif
	};
}

struct Animatronic {
	// Construct the animatronic with its base charge time
	Animatronic(int _recharge) : position(0), recharge(_recharge) {};
	int position; // Where the animatronic is in the building (refers to the index in the animatronic's Renders array)
	int recharge; // How many frames between movement opprotunities

	bool IsReady(int f) {
		return !(f % recharge); // If the frame evenly modulos by the recharge time
	}
};

int main() {
	int windowWidth = 1280;
	int windowHeight = 720;
	InitWindow(windowWidth, windowHeight, "FNaF++");
	SetTargetFPS(60);
	// Load memory & declare frame-global variables (variables that persist across frames)
	int frame = 0;
	bool b_inCams = false;
	bool b_foxyIsStunned = false; // Foxy must wait for both b_inCams & b_foxyIsStunned to both be false before he can move.
	Animatronic
		freddy(640), // TODO: Find better numbers
		foxyyy(400),
		bonnie(200),
		chicaa(250);
	int freddysStoredCrits = 0; // Freddy stores movement opprotunities for later use
	bool b_doorL, b_doorR = b_doorL = false; // TODO: Implement these

	while (!WindowShouldClose()) {
		// Update game variables
		if (IsKeyPressed(KEY_SPACE)) {
			b_inCams = !b_inCams;
			if (b_inCams) b_foxyIsStunned = true;
		}
		frame++;
		int frameRand = rand(); // Note that this value will be the same across all uses this frame. To help this, post-increment it each time it is used.
		// TODO: What happens when an animatronic's position is greater than the last valid index in the animatronic's renders array?
		if (freddy.IsReady(frame)) {
			++freddysStoredCrits;
			if (!b_inCams) {
				// (frameRand % freddysStoredCrits) is guaranteed to never exceed freddysStoredCrits
				freddy.position += (freddysStoredCrits - (frameRand++ % freddysStoredCrits));
				freddysStoredCrits = 0;
			}
		}
		if (foxyyy.IsReady(frame) && !b_inCams) {
			if (!b_foxyIsStunned) foxyyy.position += (frameRand++ & 1);
			else b_foxyIsStunned = false;
		}
		if (bonnie.IsReady(frame)) {
			bonnie.position += (frameRand++ & 1);
		}
		if (chicaa.IsReady(frame)) {
			chicaa.position += (frameRand++ & 1);
		}

		// Draw the frame
		BeginDrawing(); {

			// Rendering
			ClearBackground(BLACK);

#if _DEBUG
			// Print the debug data
			// Split into multiple sections because the default Raylib font isn't monospace
			DrawText(TextFormat("Freddy:\nFoxy:\nBonnie:\nChica:\n\nCurrent state: %s",(b_inCams ? "Camera" : "Office")), 0, 0, 8, WHITE);
			DrawText(TextFormat("%i\n%i\n%i\n%i",
								freddy.position,
								foxyyy.position,
								bonnie.position,
								chicaa.position), 48, 0, 8, WHITE);
			DrawText(TextFormat("%i\n%i\n%i\n%i",
								frame % freddy.recharge,
								frame % foxyyy.recharge,
								frame % bonnie.recharge,
								frame % chicaa.recharge), 69, 0, 8, WHITE);
			DrawText(TextFormat(" / %i (opprotunities: %i)  |  stored crits: %i\n / %i (opprotunities: %i)  |  %s\n / %i (opprotunities: %i)\n / %i (opprotunities: %i)",
								freddy.recharge, frame / freddy.recharge, freddysStoredCrits,
								foxyyy.recharge, frame / foxyyy.recharge, (b_foxyIsStunned ? "stunned" : ""),
								bonnie.recharge, frame / bonnie.recharge,
								chicaa.recharge, frame / chicaa.recharge), 86, 0, 8, WHITE);
#endif
			// TODO: render the animatronics

		} EndDrawing();
	}
	// Unload & free memory
	// TODO

	CloseWindow();
	return 0;
}

// My dad is telling me to go upstairs now, I apologize if I ruin this code with sleepy-brain when I get back.