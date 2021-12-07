#include "Pacman.h"

#include <sstream>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	srand(time(NULL));
	_pacman = new Player();
	_pacman->dead = false;


	for (size_t i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new Enemy();
		_ghosts[i]->Speed = 0.2f;
		_ghosts[i]->SpeedMult = 1.4f;
		_ghosts[i]->Direction = 0;
		_ghosts[i]->Frame = 0;
		_ghosts[i]->CurrentFrameTime = 0;
		_ghosts[i]->FrameTime = 250;
	}


	for (size_t i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Item();
		_munchies[i]->CurrentFrameTime = 0;
		_munchies[i]->Frame = rand() % 1;
		_munchies[i]->FrameTime = rand() % 500 + 50;
	}

	for (size_t i = 0; i < APPLECOUNT; i++)
	{
		_apples[i] = new Item();
		_apples[i]->CurrentFrameTime = 0;
		_apples[i]->Frame = 0;
		_apples[i]->FrameTime = 200;
	}

	for (size_t i = 0; i < TILECOUNT; i++)
	{
		_tiles[i] = new Tile();
	}

	_cherry = new Item();

	_pop = new SoundEffect();

	_pacman->CurrentFrameTime = 0;
	_pacman->Frame = 0;
	_pacman->FrameTime = 250;
	_pacman->Speed = 0.2f;
	_pacman->speedMult = 1.2f;
	_cherry->Frame = 0;
	_cherry->CurrentFrameTime = 0;
	_cherry->FrameTime = 750;

	// Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1020, 765, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

// Deletes remaining variables and textures from memory
Pacman::~Pacman()
{
	delete _pacman->Texture;
	delete _pacman->SourceRect;
	delete _pacman->Position;
	delete _pacman;
	for (size_t i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->Texture;
		delete _munchies[i]->Rect;
		delete _munchies[i];
	}
	for (size_t i = 0; i < APPLECOUNT; i++)
	{
		delete _apples[i]->Texture;
		delete _apples[i]->Rect;
		delete _apples[i];
	}
	for (size_t i = 0; i < TILECOUNT; i++)
	{
		delete _tiles[i]->Texture;
		delete _tiles[i]->Rect;
		delete _tiles[i];
	}
	delete _cherry->Texture;
	delete _cherry->Rect;
	delete _cherry;
	delete _menuBackground;
	delete _menuRectangle;
	delete _backdrop;
	delete _backdropRect;
	delete _pop;
}

// Loads all of the textures and variables used in the program
void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->Texture = new Texture2D();
	_pacman->Texture->Load("Textures/GB_Pacman.tga", false);
	_pacman->Position = new Vector2(350.0f, 350.0f);
	_pacman->SourceRect = new Rect(0.0f, 0.0f, 38, 38);

	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/MunchieBoth.png", false);

	Texture2D* appleTex = new Texture2D();
	appleTex->Load("Textures/GB_Apple.png", false);

	Texture2D* tileTex = new Texture2D();
	tileTex->Load("Textures/GB_Girder.png", false);

	// Load Ghosts
	for (size_t i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->Texture = new Texture2D();
		_ghosts[i]->Texture->Load("Textures/GhostBlue.png", false);
		_ghosts[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[i]->SourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}

	// Load Munchies
	for (size_t i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->Texture = munchieTex;
		_munchies[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchies[i]->Rect = new Rect(0.0f, 0.0f, 12, 12);
	}

	// Load Apples
	for (size_t i = 0; i < APPLECOUNT; i++)
	{
		_apples[i]->Texture = appleTex;
		_apples[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_apples[i]->Rect = new Rect(0.0f, 0.0f, 26, 30);
	}

	// Load Tiles
	for (size_t i = 0; i < TILECOUNT; i++)
	{
		_tiles[i]->Texture = tileTex;
		_tiles[i]->Position = new Vector2(51.0f*i, 0.0f);
		_tiles[i]->Rect = new Rect(0.0f, 0.0f, 51, 51);
		
	}

	// Load Cherry
	_cherry->Texture = new Texture2D();
	_cherry->Texture->Load("Textures/Cherry.png", true);
	_cherry->Position = new Vector2(750.0f, 350.0f);
	_cherry->Rect = new Rect(0.0f, 0.0f, 32, 32);

	// Set Sound Effect File
	_pop->Load("Sounds/pop.wav");

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f - 40.0f, Graphics::GetViewportHeight() / 2.0f);
	_pKeyDown = false;

	_backdrop = new Texture2D();
	_backdrop->Load("Textures/Back.png", false);
	_backdropRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
}

// Checks for if any of WASD are currently down, if so, move Pacman in that direction
void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->Direction = 0; // Makes Pacman look in the correct direction  
		_pacman->Position->X += _pacman->Speed * elapsedTime; //Moves Pacman right across X axis
	}
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->Direction = 2; // Makes Pacman look in the correct direction  
		_pacman->Position->X -= _pacman->Speed * elapsedTime; //Moves Pacman left across X axis
	}
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->Direction = 3; // Makes Pacman look in the correct direction  
		_pacman->Position->Y -= _pacman->Speed * elapsedTime; //Moves Pacman up the Y axis
	}
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->Direction = 1; // Makes Pacman look in the correct direction  
		_pacman->Position->Y += _pacman->Speed * elapsedTime; //Moves Pacman down the Y axis
	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}

	if (state->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
	}
}

void Pacman::CheckViewportCollision()
{ 
	// If Pacman goes 32 pixels off screen to the left or right, teleport him 32 pixels off of the opposite side (32 pixels is the size of Pacman, makes it look smooth)
	// If Pacman makes contact with the Top or Bottom, stop Pacman from going further up/down
	if (_pacman->Position->X + _pacman->SourceRect->Width > (Graphics::GetViewportWidth() + 32))
		_pacman->Position->X = -32;

	if (_pacman->Position->X < -32)
		_pacman->Position->X = Graphics::GetViewportWidth() + 32 - _pacman->SourceRect->Width;

	if (_pacman->Position->Y + _pacman->SourceRect->Height > Graphics::GetViewportHeight())
		_pacman->Position->Y = Graphics::GetViewportHeight() - _pacman->SourceRect->Height;

	if (_pacman->Position->Y < 0)
		_pacman->Position->Y = 0;
}

void Pacman::CheckGhostCollision()
{
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;

	for (size_t i = 0; i < GHOSTCOUNT; i++)
	{
		bottom2 = _ghosts[i]->Position->Y + _ghosts[i]->SourceRect->Height;
		left2 = _ghosts[i]->Position->X;
		right2 = _ghosts[i]->Position->X + _ghosts[i]->SourceRect->Width;
		top2 = _ghosts[i]->Position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			Audio::Play(_pop);
			i = GHOSTCOUNT;
			_ended = !_ended;
		}
	}


}

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->CurrentFrameTime += elapsedTime;

	if (_pacman->CurrentFrameTime > _pacman->FrameTime) // Everytime a certain number of frames pass, switch the texture of Pacman
	{
		_pacman->Frame++;

		if (_pacman->Frame >= 2) // Stops the texture from going over whats possible
			_pacman->Frame = 0;

		_pacman->CurrentFrameTime = 0; // Resets the timer
	}

	_pacman->SourceRect->X = _pacman->SourceRect->Width * _pacman->Frame; // Animates Pacman
	_pacman->SourceRect->Y = _pacman->SourceRect->Height * _pacman->Direction; // Sets the correct texture for the direction Pacman is facing
}

void Pacman::UpdateGhost(Enemy* ghost, int elapsedTime)
{


	//if (ghost->Position->X + ghost->SourceRect->Width > (Graphics::GetViewportWidth() + 32))
	//	ghost->Position->X = -32;

	//if (ghost->Position->X < -32)
	//	ghost->Position->X = Graphics::GetViewportWidth() + 32 - ghost->SourceRect->Width;

	if (ghost->Position->Y + ghost->SourceRect->Height > Graphics::GetViewportHeight())
		ghost->Position->Y = Graphics::GetViewportHeight() - ghost->SourceRect->Height;

	if (ghost->Position->Y < 0)
		ghost->Position->Y = 0;


	if (ghost->Direction == 0)
	{
	ghost->SourceRect->X = 0.0f;
	ghost->Position->X += ghost->Speed * elapsedTime;
	}

	else if (ghost->Direction == 1)
	{
		ghost->SourceRect->X = 20.0f;
		ghost->Position->X -= ghost->Speed * elapsedTime;
	}

	if (ghost->Position->X + ghost->SourceRect->Width >= Graphics::GetViewportWidth())
		ghost->Direction = 1;
	else if (ghost->Position->X <= 0)
		ghost->Direction = 0;
}

void Pacman::UpdateMunchie(Item* _munchie, int elapsedTime)
{
	_munchie->CurrentFrameTime += elapsedTime;

	if (_munchie->CurrentFrameTime > _munchie->FrameTime) // Everytime a certain number of frames pass, switch the texture of the Munchie
	{
		_munchie->Frame++;

		if (_munchie->Frame >= 2) // Stops the texture from going over whats possible
			_munchie->Frame = 0;

		_munchie->CurrentFrameTime = 0; // Resets the timer
	}

	_munchie->Rect->X = _munchie->Rect->Width * _munchie->Frame; // Animates the munchie
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherry->CurrentFrameTime += elapsedTime;

	if (_cherry->CurrentFrameTime > _cherry->FrameTime) // Everytime a certain number of frames pass, switch the texture of the Cherry
	{
		_cherry->Frame++;

		if (_cherry->Frame >= 2) // Stops the texture from going over whats possible
			_cherry->Frame = 0;

		_cherry->CurrentFrameTime = 0; // Resets the timer
	}

	_cherry->Rect->X = _cherry->Rect->Width * _cherry->Frame; // Animates the Cherry
}

void Pacman::UpdateApple(Item* _apple, int elapsedTime)
{
	_apple->CurrentFrameTime += elapsedTime;


	if (_apple->CurrentFrameTime > _apple->FrameTime) // Everytime a certain number of frames pass, switch the texture of the Munchie
	{
		_apple->Frame++;

		if (_apple->Frame == 7) // Stops the texture from going over whats possible
			_apple->Rect->Y = 30.0f;

		if (_apple->Frame >= 14) // Stops the texture from going over whats possible
			{
				_apple->Frame = 0;
				_apple->Rect->Y = 0.0f;
			}

		_apple->CurrentFrameTime = 0; // Resets the timer
	}

	if (_apple->Rect->Y == 30.0f)
	{
		_apple->Rect->X = _apple->Rect->Width * (_apple->Frame - 7);
	}
	else
		_apple->Rect->X = _apple->Rect->Width * _apple->Frame; 
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Starts the game upon pressing SPACE
	if (!_started)
	{
		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			_started = true;
		}
	}

	// When the game starts, begin the Mainloop
	if (_started)
	{ 
		CheckPaused(keyboardState, Input::Keys::P);

		// Mainloop for the game
		if (!_paused)
		{
			if (!_ended)
			{
				Input(elapsedTime, keyboardState);
				UpdatePacman(elapsedTime);
				for (size_t i = 0; i < GHOSTCOUNT; i++)
				{
					UpdateGhost(_ghosts[i], elapsedTime);
				}
				for (size_t i = 0; i < MUNCHIECOUNT; i++)
				{
					UpdateMunchie(_munchies[i], elapsedTime);
				}
				UpdateCherry(elapsedTime);
				for (size_t i = 0; i < APPLECOUNT; i++)
				{
					UpdateApple(_apples[i], elapsedTime);
				}
				CheckViewportCollision();
				CheckGhostCollision();
			}
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	if (_started == true) // When the game starts, begin drawing everything else
	{
		

		// Allows us to easily create a string
		std::stringstream stream;

		stream << "Pacman X: " << _pacman->Position->X << " Y: " << _pacman->Position->Y; // Draws the position counter in the top left

		SpriteBatch::BeginDraw(); // Starts Drawing

		SpriteBatch::Draw(_backdrop, _backdropRect, nullptr);

		for (size_t i = 0; i < MUNCHIECOUNT; i++)
		{
			SpriteBatch::Draw(_munchies[i]->Texture, _munchies[i]->Position, _munchies[i]->Rect); // Draws the munchie
		}
		SpriteBatch::Draw(_cherry->Texture, _cherry->Position, _cherry->Rect); // Draws the cherry

		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green); // Draws String

		for (size_t i = 0; i < GHOSTCOUNT; i++)
		{
			SpriteBatch::Draw(_ghosts[i]->Texture, _ghosts[i]->Position, _ghosts[i]->SourceRect); // Draws the munchie
		}

		for (size_t i = 0; i < APPLECOUNT; i++)
		{
			SpriteBatch::Draw(_apples[i]->Texture, _apples[i]->Position, _apples[i]->Rect); // Draws the munchie
		}

		if (!_pacman->dead)
		{
			SpriteBatch::Draw(_pacman->Texture, _pacman->Position, _pacman->SourceRect); // Draws Pacman
		}

		for (size_t i = 0; i < TILECOUNT; i++)
		{
			SpriteBatch::Draw(_tiles[i]->Texture, _tiles[i]->Position, _tiles[i]->Rect); // Draws the munchie
		}

		// When paused, draw the PAUSED! text and transparent overlay
		if (_paused)
		{
			std::stringstream menuStream;
			menuStream << "PAUSED!";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
		}

		if (_ended)
		{
			std::stringstream endStream;
			endStream << "GAME OVER!";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(endStream.str().c_str(), _menuStringPosition, Color::Red);
		}
	}

	if (_started == false) // Before the game starts, draw the starting text and background
	{
		std::stringstream startStream;
		startStream << "Press SPACE to start!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(startStream.str().c_str(), _menuStringPosition, Color::Green);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}


/*
Audio
Map
Ghost AI (Wander, Follow, Visual Chase x 2 (One Slow, One Fast))
More Levels
Kill Ghosts Item
More Items
Game Over Screen
Randomly Generated Game????
Seems Interesting
*/





