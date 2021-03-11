#include <raylib.h>
#include <raymath.h>
#include <random>
#include <vector>
/*************************************************************************
* 
*	This project uses Raylib (https://www.raylib.com/)
*	Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
* 
*	To understand what the functions/types not defined inside this doc
*	are/do, you can search for them on the Raylib Cheatsheet
*	https://www.raylib.com/cheatsheet/cheatsheet.html
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

// Putting an @ anywhere in a comment will cause anything after the @ to only show up when looking at the comment itself, so that it doesn't show up in Quick-Info when hovering a variable/type/function name.
// I do this so that explanations of why I wrote my code in certain ways doesn't appear when trying to remember what a variable/type/function is supposed to be for.

// Enumerator for storing what camera a button is associated with
// @TODO: This enum hasn't been used anywhere yet...
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

// Stores an array of textures for displaying in an animation
struct Sprite {
	
	/***************************************************************************************************************************************
	* 
	*	TIP: the syntax `Type (&foo)[number]` allows you to pass a reference to a stack array as parameter.
	*	A "stack array" is what I call an array declared with the syntax `Type foo[number]`. It is an array of constant size.
	*	The fact that the array is stored on the stack and not the heap makes it easier (in my opinion) to work with.
	*	"malloc()/new[]" and "free()/delete[]" don't have to be called on stack arrays, and they can be initialized in a function's parameter list.
	*	The only downside is the fact that the array's size has to be known at compile time and can't be based on a variable.
	*	To combat this, I usually just template the constructor/function taking the stack array parameter. This makes it possible for
	*	the compiler to determine for itself what versions of the constructor are needed for the program to work and implement them.
	*/
	/*
	*	NOTE: `Type &foo` is the syntax for lvalue references (reference to an existing variable).
	*	`Type &&foo` is the syntax for rvalue references (reference to a variable created at the time the function is called).
	*	I used an rvalue reference here so that I can still reference an array, but the array doesn't have to *already exist* prior to
	*	calling the constructor. As a result, the constructor can take an array that hasn't even been given a name and doesn't exist
	*	outside the scope of the constructor.
	*/
	/*
	*	The resulting syntax for calling the constructor for Sprite is:
	* 
	*	` Sprite foo({ "frame0.png", "frame1.png", "frame2.png" }); `
	* 
	*	(in this example, foo loads and stores three textures with the respective filenames)
	* 
	***************************************************************************************************************************************/

	// Allocates space for storing the array of textures, then loads the textures using the supplied filenames.
	template<unsigned int _length> // @ For each unique array size passed as a parameter for the Sprite constructor, a copy of the constructor is made to match the passed array size.
	Sprite(const char* (&&_fileNameArray)[_length]) : length(_length), renders(nullptr) {
		renders = new Texture2D[length]; // Allocate the memory for storing the array of textures
		for (size_t i = 0; i < length; ++i) { renders[i] = LoadTexture(_fileNameArray[i]); } // For each element of the passed array, initialize the memory with the loaded texture
	}
	// Unloads and frees the array of textures
	// @ The destructor doesn't need to be (and really shouldn't) templated, as length will have been given a value by this point.
	~Sprite() {
		for (unsigned int i = 0; i < length; ++i) { UnloadTexture(renders[i]); } // Unload the textures before freeing the memory so we can still access the textures *to* unload them.
		delete[] renders; // Free the memory so the OS can use it for more important stuff. This only frees the array, not its elements; which is why we unload the textures first.
	}

	const unsigned int length; // How many frames are in the render
	Texture2D* renders; // Pointer to array of renders
};

// Type for storing information about an animatronic
struct Animatronic {
	// Construct the animatronic with its base charge time
	// There is no default constructor. Animatronic recharge is required as a non-default due to it being a const.
	Animatronic(int _recharge) : position(0), recharge(_recharge), level(0) {};
	// Where the animatronic is in the building
	// refers to the index in the animatronic's Renders array
	int position;
	// How many frames between movement opprotunities
	// Do not increment/decrement this, it should stay the same at all times once initialized.
	const int recharge;
	// The AI level of the animatronic
	// Movement oppronity RNG will be compared against this number to determine success of the "dice roll" (expected 0..20)
	int level;

	// Whether the animatronic has the opprotunity to move this frame. They will still need to succeed the RNG to actually move.
	bool IsReady(int f) {
		return !(f % recharge); // If the frame evenly modulos by the recharge time, the animatronic has the opprotunity to move.
	}
};

// Differenciates characters for use in the Jumpscare function
enum class Character {
	FREDDY, // Pull animation from Freddy's pool
	FOXYYY, // Pull animation from Foxy's pool
	BONNIE, // Pull animation from Bonnie's pool
	CHICAA, // Pull animation from Chica's pool
};

void Jumpscare(Character animation) {
	switch (animation) {
	case Character::FREDDY:
		// TODO: Show jumpscare animation
		break;
	case Character::FOXYYY:
		// TODO: Show jumpscare animation
		break;
	case Character::BONNIE:
		// TODO: Show jumpscare animation
		break;
	case Character::CHICAA:
		// TODO: Show jumpscare animation
		break;
	default:
		// TODO: Default jumpscare?..
		break;
	}
}

int main1() {
	int windowWidth = 1920; // FHD screen resolution for width
	int windowHeight = 1080; // FHD screen resolution for height
	InitWindow(windowWidth, windowHeight, "FNaF++"); // Create the window the game will run in
	SetTargetFPS(60); // We want the game to run at 60 fps; no more, less is bad but what can ya do.
	ToggleFullscreen(); // Because programs start in windowed mode, toggling fullscreen will make the window fullscreen.
	// Load memory & declare frame-global variables (variables that persist across frames)

#pragma region Renders	

#if _DEBUG // Initialize the debug textures so we only have to load each once
	Texture2D debug_Freddy = LoadTexture("Freddy_Debug.png");
	Texture2D debug_Foxyyy = LoadTexture("Foxy_Debug.png"  );
	Texture2D debug_Bonnie = LoadTexture("Bonnie_Debug.png");
	Texture2D debug_Chicaa = LoadTexture("Chica_Debug.png" );
#endif

	// Array of renders for displaying Freddy
	Texture2D freddyRenders[8]{
	#if _DEBUG
		debug_Freddy,	// Show stage
		debug_Freddy,	// Dining hall
		debug_Freddy,	// Bathrooms
		debug_Freddy,	// Kitchen
		debug_Freddy,	// East hall
		debug_Freddy,	// East corner
		debug_Freddy,	// East door

		debug_Freddy,	// West door (power out)
	#else // TODO
		LoadTexture("Freddy_ShowStage.png"  ),	// Show stage
		LoadTexture("Freddy_DiningHall.png" ),	// Dining hall
		LoadTexture("Freddy_Bathrooms.png"  ),	// Bathrooms
		LoadTexture("Freddy_Kitchen.png"    ),	// Kitchen
		LoadTexture("Freddy_Hall_East.png"  ),	// East hall
		LoadTexture("Freddy_Corner_East.png"),	// East corner
		LoadTexture("Freddy_Door_East.png"  ),	// East door

		LoadTexture("Freddy_Door_West.png"  ),	// West door (power out)
	#endif
	};
	Sprite freddyJumpscare({ "" }); // TODO

	// Array of renders for displaying Foxy
	Texture2D foxyyyRenders[3]{
	#if _DEBUG
		debug_Foxyyy,	// Pirate Cove (0)
		debug_Foxyyy,	// Pirate Cove (1)
		debug_Foxyyy,	// Pirate Cove (2)
		// West hall (animated) TODO
		// West door (animated) TODO
	#else // TODO
		LoadTexture("Foxy_PirateCove_0.png"),	// Pirate Cove (0)
		LoadTexture("Foxy_PirateCove_1.png"),	// Pirate Cove (1)
		LoadTexture("Foxy_PirateCove_2.png"),	// Pirate Cove (2)
		// West hall (animated) TODO
		// West door (animated) TODO
	#endif
	};
	Sprite foxyyyJumpscare({ "" }); // TODO
	Sprite foxyyyHallRun({ "","" });

	// Array of renders for displaying Bonnie
	Texture2D bonnieRenders[7]{
	#if _DEBUG
		debug_Bonnie,	// Show stage
		debug_Bonnie,	// Dining hall
		debug_Bonnie,	// Backstage
		debug_Bonnie,	// West hall
		debug_Bonnie,	// Storage closet
		debug_Bonnie,	// West corner
		debug_Bonnie,	// West door
	#else // TODO
		LoadTexture("Bonnie_ShowStage"    ),	// Show stage
		LoadTexture("Bonnie_DiningHall"   ),	// Dining hall
		LoadTexture("Bonnie_Backstage"    ),	// Backstage
		LoadTexture("Bonnie_Hall_West"    ),	// West hall
		LoadTexture("Bonnie_StorageCloset"),	// Storage closet
		LoadTexture("Bonnie_Corner_West"  ),	// West corner
		LoadTexture("Bonnie_Door_West"    ),	// West door
	#endif
	};
	Sprite bonnieJumpscare({ "" }); // TODO

	// Array of renders for displaying Chica
	Texture2D chicaaRenders[7]{
	#if _DEBUG
		debug_Chicaa,	// Show stage
		debug_Chicaa,	// Dining hall
		debug_Chicaa,	// Bathrooms
		debug_Chicaa,	// Kitchen
		debug_Chicaa,	// East hall
		debug_Chicaa,	// East corner
		debug_Chicaa,	// East door
	#else // TODO
		LoadTexture("Chica_ShowStage"  ),	// Show stage
		LoadTexture("Chica_DiningHall" ),	// Dining hall
		LoadTexture("Chica_Bathroom"   ),	// Bathrooms
		LoadTexture("Chica_Kitchen"    ),	// Kitchen
		LoadTexture("Chica_Hall_East"  ),	// East hall
		LoadTexture("Chica_Corner_East"),	// East corner
		LoadTexture("Chica_Door_East"  ),	// East door
	#endif
	};
	Sprite chicaaJumpscare({ "" }); // TODO
	Sprite chicaaHeadTwitch({ "" });

#pragma endregion

	int frame = 0; // What frame we are on @ The frame number can be a clean integer, time would be a float and may not line up with the times we are performing calculations.
	bool b_inCams = false; // Whether the player is looking at the cameras
	bool b_foxyIsStunned = false; // Foxy must wait for both b_inCams & b_foxyIsStunned to both be false before he can move.
	Animatronic // @ The following comma-separated identifiers (variable names) will all be of the type Animatronic until a semicolon is reached.
		freddy(673), // Freddy @ Following the variable name with parenthesis containing parameters that are valid with a constructor for the type calls the constructor on the variable in the same line it is declared.
		foxyyy(437), // Foxy   @ Note that Animatronic does not have a default constructor. If we declared these variables without calling the specialized constructor, it would give an error. This is on purpose.
		bonnie(284), // Bonnie
		chicaa(390); // Chica
	int freddysStoredCrits = 0; // Freddy stores movement opprotunities for later use
	bool b_doorL, b_doorR = b_doorL = false; // Simultaneously declare both b_doorL and b_doorR, initializing them both to false in the same line. (b_doorL = false; b_doorR = b_doorL (which is now false);)

	while (!WindowShouldClose()) { // This is the game loop; what happens every frame the program is running
		#pragma region Update game variables

		if (IsKeyPressed(KEY_SPACE)) { // If the space key was pressed this frame,
			b_inCams = !b_inCams; // Toggle the "are we watching the cameras" bool
			if (b_inCams) b_foxyIsStunned = true; // Then, if we are *now* in the cameras (meaning we've entered the cam this frame), stun Foxy.
		}
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

			if (bonnie.position >= 7) { // Invalid index; no render exists for this so we will instead initiate the jumpscare sequence.
				Jumpscare(Character::BONNIE);
			}
		}
		if (chicaa.IsReady(frame)) {
			chicaa.position += (frameRand++ & 1);

			if (chicaa.position >= 7) { // Invalid index; no render exists for this so we will instead initiate the jumpscare sequence.
				Jumpscare(Character::CHICAA);
			}
		}

		#pragma endregion

		#pragma region Draw the frame

		BeginDrawing(); { // You don't have to put the drawing code in its own scope, it's just a personal preference so that it gets automatically indented and doesn't leak any rendering locals.

			// Rendering
			ClearBackground(BLACK); // Clears the frame to be totally black at the start of rendering, giving us a clean slate to work off of.

		#if _DEBUG // I don't want the debug data being displayed in the release build. The "#if _DEBUG { ... } #endif" will leave this section of code out of any version where _DEBUG is 0.
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

		#pragma endregion

		frame++; // Increment the frame counter at the end of the frame.
	}
	// Unload & free memory

	// the variable "render" is a constant Texture2D reference so that it doesn't perform a potentially expensive copy operation for each render.
	for (const Texture2D& render : freddyRenders) { UnloadTexture(render); } // Unload each texture in the freddyRenders array
	for (const Texture2D& render : foxyyyRenders) { UnloadTexture(render); } // Unload each texture in the foxyyyRenders array
	for (const Texture2D& render : bonnieRenders) { UnloadTexture(render); } // Unload each texture in the bonnieRenders array
	for (const Texture2D& render : chicaaRenders) { UnloadTexture(render); } // Unload each texture in the chicaaRenders array

	CloseWindow();
	return 0;
}