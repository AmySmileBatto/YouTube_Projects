#include <stdarg.h>
#include <stdbool.h>
#include <raylib.h>
#include <raymath.h>

typedef enum Cam {
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
} Cam;

typedef struct Sprite {
	unsigned int length;
	Texture2D* renders;
} Sprite;
void InitSprite(Sprite* sprite, int size, ...) {
	sprite->length = size;
	sprite->renders = malloc(sizeof(Texture2D) * size);
	va_list args;
	va_start(args, size);
	while (size) {
		const char* filename = va_arg(args, const char*);
		sprite->renders[sprite->length - size] = LoadTexture(filename);
		--size;
	}
	va_end(args);
}
void UnloadSprite(Sprite* sprite) {
	for (unsigned int i = 0; i < sprite->length; ++i) {
		UnloadTexture(sprite->renders[i]);
	}
	free(sprite->renders);
}

typedef struct Animatronic {
	int position;
	int recharge;
	int level;
} Animatronic;
void InitAnimatronic(Animatronic* animatronic, int recharge) {
	animatronic->position = 0;
	animatronic->recharge = recharge;
	animatronic->level = 0;
}
char IsReady(Animatronic* who, int f) {
	return !(f % who->recharge);
}

// Differenciates characters for use in the Jumpscare function
typedef enum {
	FREDDY, // Pull animation from Freddy's pool
	FOXYYY, // Pull animation from Foxy's pool
	BONNIE, // Pull animation from Bonnie's pool
	CHICAA, // Pull animation from Chica's pool
} Character;

void Jumpscare(Character animation) {
	switch (animation) {
	case FREDDY:
		// TODO: Show jumpscare animation
		break;
	case FOXYYY:
		// TODO: Show jumpscare animation
		break;
	case BONNIE:
		// TODO: Show jumpscare animation
		break;
	case CHICAA:
		// TODO: Show jumpscare animation
		break;
	default:
		// TODO: Default jumpscare?..
		break;
	}
}

int main1() {
	int windowWidth = 1920;
	int windowHeight = 1080;
	InitWindow(windowWidth, windowHeight, "FNaF++");
	SetTargetFPS(60);
	ToggleFullscreen();

#pragma region Renders	

#if _DEBUG // Initialize the debug textures so we only have to load each once
	Texture2D debug_Freddy = LoadTexture("Freddy_Debug.png");
	Texture2D debug_Foxyyy = LoadTexture("Foxy_Debug.png");
	Texture2D debug_Bonnie = LoadTexture("Bonnie_Debug.png");
	Texture2D debug_Chicaa = LoadTexture("Chica_Debug.png");
#endif

	// Array of renders for displaying Freddy
	Texture2D freddyRenders[8] = {
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
		LoadTexture("Freddy_ShowStage.png"),	// Show stage
		LoadTexture("Freddy_DiningHall.png"),	// Dining hall
		LoadTexture("Freddy_Bathrooms.png"),	// Bathrooms
		LoadTexture("Freddy_Kitchen.png"),	// Kitchen
		LoadTexture("Freddy_Hall_East.png"),	// East hall
		LoadTexture("Freddy_Corner_East.png"),	// East corner
		LoadTexture("Freddy_Door_East.png"),	// East door

		LoadTexture("Freddy_Door_West.png"),	// West door (power out)
	#endif
	};
	Sprite freddyJumpscare; InitSprite(&freddyJumpscare, 0); // TODO

	// Array of renders for displaying Foxy
	Texture2D foxyyyRenders[3] = {
	#if _DEBUG
		debug_Foxyyy,	// Pirate Cove (0)
		debug_Foxyyy,	// Pirate Cove (1)
		debug_Foxyyy,	// Pirate Cove (2)
	#else // TODO
		LoadTexture("Foxy_PirateCove_0.png"),	// Pirate Cove (0)
		LoadTexture("Foxy_PirateCove_1.png"),	// Pirate Cove (1)
		LoadTexture("Foxy_PirateCove_2.png"),	// Pirate Cove (2)
	#endif
	};
	Sprite foxyyyJumpscare; InitSprite(&foxyyyJumpscare, 0); // TODO
	Sprite foxyyyHallRun; InitSprite(&foxyyyHallRun, 0); // TODO

	// Array of renders for displaying Bonnie
	Texture2D bonnieRenders[7] = {
	#if _DEBUG
		debug_Bonnie,	// Show stage
		debug_Bonnie,	// Dining hall
		debug_Bonnie,	// Backstage
		debug_Bonnie,	// West hall
		debug_Bonnie,	// Storage closet
		debug_Bonnie,	// West corner
		debug_Bonnie,	// West door
	#else // TODO
		LoadTexture("Bonnie_ShowStage"),	// Show stage
		LoadTexture("Bonnie_DiningHall"),	// Dining hall
		LoadTexture("Bonnie_Backstage"),	// Backstage
		LoadTexture("Bonnie_Hall_West"),	// West hall
		LoadTexture("Bonnie_StorageCloset"),	// Storage closet
		LoadTexture("Bonnie_Corner_West"),	// West corner
		LoadTexture("Bonnie_Door_West"),	// West door
	#endif
	};
	Sprite bonnieJumpscare; InitSprite(&bonnieJumpscare, 0); // TODO

	// Array of renders for displaying Chica
	Texture2D chicaaRenders[7] = {
	#if _DEBUG
		debug_Chicaa,	// Show stage
		debug_Chicaa,	// Dining hall
		debug_Chicaa,	// Bathrooms
		debug_Chicaa,	// Kitchen
		debug_Chicaa,	// East hall
		debug_Chicaa,	// East corner
		debug_Chicaa,	// East door
	#else // TODO
		LoadTexture("Chica_ShowStage"),	// Show stage
		LoadTexture("Chica_DiningHall"),	// Dining hall
		LoadTexture("Chica_Bathroom"),	// Bathrooms
		LoadTexture("Chica_Kitchen"),	// Kitchen
		LoadTexture("Chica_Hall_East"),	// East hall
		LoadTexture("Chica_Corner_East"),	// East corner
		LoadTexture("Chica_Door_East"),	// East door
	#endif
	};
	Sprite chicaaJumpscare; InitSprite(&chicaaJumpscare, 0); // TODO
	Sprite chicaaHeadTwitch; InitSprite(&chicaaHeadTwitch, 0); // TODO

#pragma endregion

	int frame = 0;
	bool b_inCams = false;
	bool b_foxyIsStunned = false; 
	Animatronic freddy; InitAnimatronic(&freddy, 673);
	Animatronic foxyyy; InitAnimatronic(&foxyyy, 437);
	Animatronic bonnie; InitAnimatronic(&bonnie, 284);
	Animatronic chicaa; InitAnimatronic(&chicaa, 390);

	int freddysStoredCrits = 0;
	bool b_doorL = false;
	bool b_doorR = false;

	while (!WindowShouldClose()) {
#pragma region Update game variables

		if (IsKeyPressed(KEY_SPACE)) {
			b_inCams = !b_inCams;
			if (b_inCams) b_foxyIsStunned = true;
		}
		int frameRand = rand();
		if (IsReady(&freddy, frame)) {
			++freddysStoredCrits;
			if (!b_inCams) {
				freddy.position += (freddysStoredCrits - (frameRand++ % freddysStoredCrits));
				freddysStoredCrits = 0;
			}
		}
		if (IsReady(&foxyyy, frame) && !b_inCams) {
			if (!b_foxyIsStunned) foxyyy.position += (frameRand++ & 1);
			else b_foxyIsStunned = false;
		}
		if (IsReady(&bonnie, frame)) {
			bonnie.position += (frameRand++ & 1);

			if (bonnie.position >= 7) {
				Jumpscare(BONNIE);
			}
		}
		if (IsReady(&chicaa, frame)) {
			chicaa.position += (frameRand++ & 1);

			if (chicaa.position >= 7) {
				Jumpscare(CHICAA);
			}
		}

#pragma endregion

#pragma region Draw the frame

		BeginDrawing(); {

			// Rendering
			ClearBackground(BLACK);

#if _DEBUG
			DrawText(TextFormat("Freddy:\nFoxy:\nBonnie:\nChica:\n\nCurrent state: %s", (b_inCams ? "Camera" : "Office")), 0, 0, 8, WHITE);
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

		frame++;
	}
	// Unload & free memory

	for (unsigned int i = 0; i < 8; ++i) { UnloadTexture(freddyRenders[i]); }
	for (unsigned int i = 0; i < 3; ++i) { UnloadTexture(foxyyyRenders[i]); }
	for (unsigned int i = 0; i < 7; ++i) { UnloadTexture(bonnieRenders[i]); }
	for (unsigned int i = 0; i < 7; ++i) { UnloadTexture(chicaaRenders[i]); }

	CloseWindow();
	return 0;
}