#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.2f), _cPacmanFrameTime(250), _cMunchieFrameTime(500), _cCherryFrameTime(750)
{
	_munchieFrame = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0;
	_cherryFrame = 0;
	_cherryCurrentFrameTime = 0;

	// Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

// Deletes remaining variables and textures from memory
Pacman::~Pacman()
{
	delete _pacmanTextureUp;
	delete _pacmanTextureRight;
	delete _pacmanSourceRect;
	delete _munchieTexture;
	delete _munchieRect;
	delete _cherryTexture;
	delete _cherryRect;
	delete _startBackground;
	delete _startRectangle;
	delete _menuBackground;
	delete _menuRectangle;
}

// Loads all of the textures and variables used in the program
void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTextureRight = new Texture2D();
	_pacmanTextureRight->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieTexture = new Texture2D();
	_munchieTexture->Load("Textures/MunchieBoth.png", true);
	_munchiePosition = new Vector2(200.0f, 350.0f);
	_munchieRect = new Rect(0.0f, 0.0f, 12, 12);

	// Load Cherry
	_cherryTexture = new Texture2D();
	_cherryTexture->Load("Textures/Cherry.png", true);
	_cherryPosition = new Vector2(750.0f, 350.0f);
	_cherryRect = new Rect(0.0f, 0.0f, 32, 32);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f - 40.0f, Graphics::GetViewportHeight() / 2.0f);
	_pKeyDown = false;

	// Set Start Menu parameters
	_startBackground = new Texture2D();
	_startBackground->Load("Textures/Start.png", false);
	_startRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_startStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f - 80.0f, Graphics::GetViewportHeight() / 2.0f);
}

// Checks for if any of WASD are currently down, if so, move Pacman in that direction
void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacmanDirection = 0; // Makes Pacman look in the correct direction  
		_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman right across X axis
	}
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacmanDirection = 2; // Makes Pacman look in the correct direction  
		_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; //Moves Pacman left across X axis
	}
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacmanDirection = 3; // Makes Pacman look in the correct direction  
		_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; //Moves Pacman up the Y axis
	}
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacmanDirection = 1; // Makes Pacman look in the correct direction  
		_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman down the Y axis
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
	if (_pacmanPosition->X + _pacmanSourceRect->Width > (Graphics::GetViewportWidth() + 32))
		_pacmanPosition->X = -32; 

	if (_pacmanPosition->X < -32)
		_pacmanPosition->X = Graphics::GetViewportWidth() + 32 - _pacmanSourceRect->Width;

	if (_pacmanPosition->Y + _pacmanSourceRect->Height > Graphics::GetViewportHeight())
		_pacmanPosition->Y = Graphics::GetViewportHeight() - _pacmanSourceRect->Height;

	if (_pacmanPosition->Y < 0)
		_pacmanPosition->Y = 0;
}

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacmanCurrentFrameTime += elapsedTime;

	if (_pacmanCurrentFrameTime > _cPacmanFrameTime) // Everytime a certain number of frames pass, switch the texture of Pacman
	{
		_pacmanFrame++;

		if (_pacmanFrame >= 2) // Stops the texture from going over whats possible
			_pacmanFrame = 0;

		_pacmanCurrentFrameTime = 0; // Resets the timer
	}

	_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame; // Animates Pacman
	_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection; // Sets the correct texture for the direction Pacman is facing
}

void Pacman::UpdateMunchie(int elapsedTime)
{
	_munchieCurrentFrameTime += elapsedTime;

	if (_munchieCurrentFrameTime > _cMunchieFrameTime) // Everytime a certain number of frames pass, switch the texture of the Munchie
	{
		_munchieFrame++;

		if (_munchieFrame >= 2) // Stops the texture from going over whats possible
			_munchieFrame = 0;

		_munchieCurrentFrameTime = 0; // Resets the timer
	}

	_munchieRect->X = _munchieRect->Width * _munchieFrame; // Animates the munchie
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherryCurrentFrameTime += elapsedTime;

	if (_cherryCurrentFrameTime > _cCherryFrameTime) // Everytime a certain number of frames pass, switch the texture of the Cherry
	{
		_cherryFrame++;

		if (_cherryFrame >= 2) // Stops the texture from going over whats possible
			_cherryFrame = 0;

		_cherryCurrentFrameTime = 0; // Resets the timer
	}

	_cherryRect->X = _cherryRect->Width * _cherryFrame; // Animates the Cherry
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
			Input(elapsedTime, keyboardState);
			UpdatePacman(elapsedTime);
			UpdateMunchie(elapsedTime);
			UpdateCherry(elapsedTime);
			CheckViewportCollision();
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	if (_started == true) // When the game starts, begin drawing everything else
	{
		// Allows us to easily create a string
		std::stringstream stream;

		stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y; // Draws the position counter in the top left

		SpriteBatch::BeginDraw(); // Starts Drawing
		SpriteBatch::Draw(_munchieTexture, _munchiePosition, _munchieRect); // Draws the munchie
		SpriteBatch::Draw(_cherryTexture, _cherryPosition, _cherryRect); // Draws the cherry
		SpriteBatch::Draw(_pacmanTextureRight, _pacmanPosition, _pacmanSourceRect); // Draws Pacman
		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green); // Draws String

		// When paused, draw the PAUSED! text and transparent overlay
		if (_paused)
		{
			std::stringstream menuStream;
			menuStream << "PAUSED!";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
		}
	}

	if (_started == false) // Before the game starts, draw the starting text and background
	{
		std::stringstream startStream;
		startStream << "Press SPACE to start!";

		SpriteBatch::Draw(_startBackground, _startRectangle, nullptr);
		SpriteBatch::DrawString(startStream.str().c_str(), _startStringPosition, Color::Green);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}