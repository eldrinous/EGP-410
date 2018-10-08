#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sstream>
#include <string>
#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Font.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "PlayerMoveToMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "InputManager.h"

Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpFont(NULL)
	,mShouldExit(false)
	,mBackgroundBufferID("")
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	srand(static_cast<unsigned>(time(NULL)));
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init( WIDTH, HEIGHT );
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpGraphicsSystem);
	mpSpriteManager = new SpriteManager();

	mpInputManager = new InputManager();
	mpMessageManager = new GameMessageManager();
	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"arrow.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");

	//load Font
	mpFont = new Font("cour.ttf", 24);
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight() );
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	//get weights
	std::ifstream iFile;
	std::string value;
	iFile.open("FlockWeights.txt");
	iFile.ignore(256, ':');
	std::getline(iFile, value);
	mAlignmentWeight = stof(value);

	iFile.ignore(256,':');
	std::getline(iFile, value);
	mCohesionWeight = stof(value);

	iFile.ignore(256,':');
	std::getline(iFile, value);
	mSeperationWeight = stof(value);
	return true;
}

void Game::cleanup()
{
	std::ofstream oFile;
	oFile.open("FlockWeights.txt");
	oFile << "Alignment: " + std::to_string(mAlignmentWeight) << std::endl;
	oFile << "Cohesion: " + std::to_string(mCohesionWeight) << std::endl;
	oFile << "Seperation: " + std::to_string(mSeperationWeight) << std::endl;
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	delete mpFont;
	mpFont = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpInputManager;
	mpInputManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
	
void Game::processLoop()
{
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
	mpInputManager->inputManagerUpdate();

	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	GraphicsBuffer* pDest = mpGraphicsSystem->getBackBuffer();
	mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

	//draw units
	mpUnitManager->drawAll();

	//write text at mouse position
	mpGraphicsSystem->writeText(*mpFont, 0, 700, "Align: " + std::to_string(mAlignmentWeight), BLACK_COLOR);
	mpGraphicsSystem->writeText(*mpFont, 300, 700, "Cohede: " + std::to_string(mCohesionWeight), BLACK_COLOR);
	mpGraphicsSystem->writeText(*mpFont, 600, 700, "Seperate: " + std::to_string(mSeperationWeight), BLACK_COLOR);
	//test of fill region
	//mpGraphicsSystem->fillRegion(*pDest, Vector2D(300, 300), Vector2D(500, 500), RED_COLOR);
	mpGraphicsSystem->swap();

	mpMessageManager->processMessagesForThisframe();

	mpInputManager->inputManagerUpdate();
	/*
		if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		Vector2D pos(x, y);
		GameMessage* pMessage = new PlayerMoveToMessage(pos);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	//all this should be moved to InputManager!!!
	{
		//get keyboard state
		const Uint8 *state = SDL_GetKeyboardState(NULL);

		//if escape key was down then exit the loop
		if (state[SDL_SCANCODE_ESCAPE])
		{
			mShouldExit = true;
		}
	}

	Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
	if (pUnit == NULL)
	{

	}

	*/

	
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

void Game::modifyExit(bool check)
{
	mShouldExit = check;
}

void Game::modifyWeight(float value, weightType type)
{
	switch (type)
	{
	case ALIGN:
		mAlignmentWeight += value;
		if (mAlignmentWeight > 1)
		{
			mAlignmentWeight = 1;
		}
		else if (mAlignmentWeight < 0)
		{
			mAlignmentWeight = 0;
		}
		break;
	case COHESION:
		mCohesionWeight += value;
		if (mCohesionWeight > 1)
		{
			mCohesionWeight = 1;
		}
		else if (mCohesionWeight < 0)
		{
			mCohesionWeight = 0;
		}
		break;
	case SEPERATE:
		mSeperationWeight += value;
		if (mSeperationWeight > 1)
		{
			mSeperationWeight = 1;
		}
		else if (mSeperationWeight < 0)
		{
			mSeperationWeight = 0;
		}
		break;
	default:
		break;
	}
}

float Game::getWeight(weightType type)
{
	switch (type)
	{
	case ALIGN:
		return mAlignmentWeight;
		break;
	case COHESION:
		return mCohesionWeight;
		break;
	case SEPERATE:
		return mSeperationWeight;
		break;
	default:
		return 0;
		break;
	}
}

