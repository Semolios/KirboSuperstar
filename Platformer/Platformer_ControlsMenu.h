#ifndef DEF_CONTROLS_MENU
#define DEF_CONTROLS_MENU

#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Zix_PGE_Controller.h"

class OneLoneCoder_Platformer;

class cControlsMenu
{
public:
	cControlsMenu(olc::PixelGameEngine* gfx,
				  olc::Sprite* background,
				  olc::Sprite* menubar,
				  olc::Sprite* unselQuit,
				  olc::Sprite* selQuit,
				  olc::Sprite* unselDefault,
				  olc::Sprite* selDefault,
				  olc::Sprite* X,
				  olc::Sprite* Y,
				  olc::Sprite* B,
				  olc::Sprite* Pause,
				  olc::Sprite* Up,
				  olc::Sprite* Down,
				  olc::Sprite* Left,
				  olc::Sprite* Right,
				  olc::Sprite* A,
				  olc::Sprite* RBump);

private:
	int nSelectedItem = 0;
	int nOffsetX = 0;
	int nOffsetY = 160;
	int nSpacingY = 28;
	int nIsnotSelected = 0;
	int nIsSelected = 20;
	int nDefaultX = 618;
	int nDefaultY = 442;
	int nQuitX = 708;
	int nQuitY = 442;
	int nTextX = 20;
	int nTextY = 165;
	int nCommandTextOutput = 3;
	int nKeyTextX = 370;
	int nControllerTextX = 630;
	int nButtonsTextX = 620;
	int nButtonsTextY = 164;
	bool bModify = false;
	bool bAppliedControls = false;

	olc::Sprite* sprBackGround;
	olc::Sprite* sprMenuBar;
	olc::Sprite* sprUnselQuit;
	olc::Sprite* sprSelQuit;
	olc::Sprite* sprUnselDefault;
	olc::Sprite* sprSelDefault;

	// Controller Sprites
	olc::Sprite* sprX;
	olc::Sprite* sprY;
	olc::Sprite* sprB;
	olc::Sprite* sprPause;
	olc::Sprite* sprUp;
	olc::Sprite* sprDown;
	olc::Sprite* sprLeft;
	olc::Sprite* sprRight;
	olc::Sprite* sprA;
	olc::Sprite* sprRBump;

	std::map<std::string, olc::Key> tempControls;
	std::map<std::string, olc::Key> previousControls;

public:
	static OneLoneCoder_Platformer* engine;

	bool Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller);
	int GetSelectedItem();
	void SetSelectedItem(int i);
	void UpdateSavedControls();
	void ApplyDefaultControls();
	bool CheckConflicts();
};

#endif // !DEF_CONTROLS_MENU
