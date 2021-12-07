#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

#define MUNCHIECOUNT 50
#define GHOSTCOUNT 4
#define APPLECOUNT 4
#define TILECOUNT 20

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct Item
{
	Vector2* Position;
	Texture2D* Texture;
	Rect* Rect;
	int Frame;
	int FrameTime;
	int CurrentFrameTime;
};

struct Tile
{
	Vector2* Position;
	Texture2D* Texture;
	Rect* Rect;
};

struct Enemy
{
	Vector2* Position;
	Texture2D* Texture;
	Rect* SourceRect;
	float Speed;
	float SpeedMult;
	int Direction;
	int Frame;
	int FrameTime;
	int CurrentFrameTime;
};

struct Player
{
	Vector2* Position;
	Texture2D* Texture;
	Rect* SourceRect;
	bool dead;
	float Speed;
	float speedMult;
	int Direction;
	int Frame;
	int FrameTime;
	int CurrentFrameTime;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	Player* _pacman;
	Item* _munchies[MUNCHIECOUNT];
	Item* _cherry;
	Enemy* _ghosts[GHOSTCOUNT];
	Item* _apples[APPLECOUNT];
	Tile* _tiles[TILECOUNT];

	char map[15][20] = {"WHHHHHHHHHHHHHHHHHJ",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"V                 V",
						"EHHHHHHHHHHHHHHHHHK", };

	// Position for String
	Vector2* _stringPosition;

	SoundEffect* _pop;
	Texture2D* _backdrop;
	Rect* _backdropRect;

	// Data for Pause Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _pKeyDown;

	// Data for Start Screen
	Texture2D* _startBackground;
	Rect* _startRectangle;
	Vector2* _startStringPosition;
	bool _started = false;

	// Data for Game Over Screen
	Texture2D* _endBackground;
	Rect* _endRectangle;
	Vector2* _endStringPosition;
	bool _ended = false;

	// Input Methods
	void Input(int elapsedTime, Input::KeyboardState* state);

	// Check Methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void CheckGhostCollision();

	// Update Methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(Item*, int elapsedTime);
	void UpdateCherry(int elapsedTime);
	void UpdateGhost(Enemy*, int elaspedTime);
	void UpdateApple(Item*, int elapsedTime);


public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};