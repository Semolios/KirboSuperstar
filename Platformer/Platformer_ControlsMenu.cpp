#include "Platformer_ControlsMenu.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cControlsMenu::engine = nullptr;

cControlsMenu::cControlsMenu(olc::PixelGameEngine* gfx,
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
							 olc::Sprite* RBump)
{
	sprBackGround = background;
	sprMenuBar = menubar;
	sprUnselQuit = unselQuit;
	sprSelQuit = selQuit;
	sprUnselDefault = unselDefault;
	sprSelDefault = selDefault;

	// Controller sprites
	sprX = X;
	sprY = Y;
	sprB = B;
	sprPause = Pause;
	sprUp = Up;
	sprDown = Down;
	sprLeft = Left;
	sprRight = Right;
	sprA = A;
	sprRBump = RBump;
}

bool cControlsMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller)
{
	// On first coming on this screen, apply saved controls on temps controls
	if (!bAppliedControls)
	{
		bAppliedControls = true;

		std::wifstream file("assets/ctrls/controls.txt");

		if (file)
		{
			std::wstring line;

			while (std::getline(file, line))
			{
				std::wstring temp;
				std::vector<std::wstring> parts;
				std::wstringstream wss(line);
				while (std::getline(wss, temp, L'='))
				{
					parts.push_back(temp);
				}

				tempControls[engine->ToStr(parts[0])] = engine->ToOlcKey(engine->ToStr(parts[1]));
				previousControls[engine->ToStr(parts[0])] = engine->ToOlcKey(engine->ToStr(parts[1]));
			}
		}
	}

	gfx->SetPixelMode(olc::Pixel::ALPHA);

	gfx->DrawSprite(0, 0, sprBackGround);

	// Handle input
	if (!bModify)
	{
		if (gfx->GetKey(olc::DOWN).bPressed || controller->GetButton(DOWN).bPressed)
		{
			engine->PlaySample("menuBip");
			nSelectedItem++;
			if (nSelectedItem > 10) nSelectedItem = 0;
		}

		if (gfx->GetKey(olc::UP).bPressed || controller->GetButton(UP).bPressed)
		{
			engine->PlaySample("menuBip");
			nSelectedItem--;
			if (nSelectedItem < 0)
				nSelectedItem = 10;
			else if (nSelectedItem == 10)
				nSelectedItem = 9;
		}

		if (gfx->GetKey(olc::LEFT).bPressed || controller->GetButton(LEFT).bPressed || gfx->GetKey(olc::RIGHT).bPressed || controller->GetButton(RIGHT).bPressed)
		{
			if (nSelectedItem == 10)
			{
				engine->PlaySample("menuBip");
				nSelectedItem = 11;
			}
			else if (nSelectedItem == 11)
			{
				engine->PlaySample("menuBip");
				nSelectedItem = 10;
			}
		}

		if (gfx->GetKey(olc::SPACE).bPressed || controller->GetButton(A).bPressed || gfx->GetKey(olc::ENTER).bPressed)
		{
			if (nSelectedItem == 10)
				ApplyDefaultControls();
			else if (nSelectedItem != 11)
				bModify = true;
		}
	}
	else
	{
		if (engine->GetAnyKey())
		{
			switch (nSelectedItem)
			{
				case 0: tempControls["vacuum"] = engine->GetFirstKeyPressed();			 break;
				case 1: tempControls["launchJesusCross"] = engine->GetFirstKeyPressed(); break;
				case 2: tempControls["slapAttack"] = engine->GetFirstKeyPressed();		 break;
				case 3: tempControls["jump"] = engine->GetFirstKeyPressed();		     break;
				case 4: tempControls["pause"] = engine->GetFirstKeyPressed();			 break;
				case 5: tempControls["flyOrEnterDoor"] = engine->GetFirstKeyPressed();	 break;
				case 6: tempControls["down"] = engine->GetFirstKeyPressed();			 break;
				case 7: tempControls["moveLeft"] = engine->GetFirstKeyPressed();	     break;
				case 8: tempControls["moveRight"] = engine->GetFirstKeyPressed();	     break;
				case 9: tempControls["poyo"] = engine->GetFirstKeyPressed();		     break;
			}

			if (CheckConflicts())
				tempControls = previousControls;
			else
				bModify = false;
		}
	}

	// Draw items
	for (int i = 0; i < 10; i++)
	{
		if (i == nSelectedItem)
			nOffsetX = nIsSelected;
		else
			nOffsetX = nIsnotSelected;

		gfx->DrawSprite(nOffsetX, nOffsetY + i * nSpacingY, sprMenuBar);
	}

	// controls names
	engine->DrawKirboString(nTextX + ((nSelectedItem == 0) ? nIsSelected : nIsnotSelected), nTextY + 0 * nSpacingY - nCommandTextOutput, "Vacuum");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 1) ? nIsSelected : nIsnotSelected), nTextY + 1 * nSpacingY - nCommandTextOutput, "Jesus Cross");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 2) ? nIsSelected : nIsnotSelected), nTextY + 2 * nSpacingY - nCommandTextOutput, "Slap Attack");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 3) ? nIsSelected : nIsnotSelected), nTextY + 3 * nSpacingY - nCommandTextOutput, "Jump");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 4) ? nIsSelected : nIsnotSelected), nTextY + 4 * nSpacingY - nCommandTextOutput, "Pause");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 5) ? nIsSelected : nIsnotSelected), nTextY + 5 * nSpacingY - nCommandTextOutput, "Fly Or Enter Door");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 6) ? nIsSelected : nIsnotSelected), nTextY + 6 * nSpacingY - nCommandTextOutput, "Down");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 7) ? nIsSelected : nIsnotSelected), nTextY + 7 * nSpacingY - nCommandTextOutput, "MoveLeft");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 8) ? nIsSelected : nIsnotSelected), nTextY + 8 * nSpacingY - nCommandTextOutput, "MoveRight");
	engine->DrawKirboString(nTextX + ((nSelectedItem == 9) ? nIsSelected : nIsnotSelected), nTextY + 9 * nSpacingY - nCommandTextOutput, "Poyo");

	engine->DrawKirboString(nKeyTextX, nTextY - nSpacingY, "Keyboard", 1, true);

	// controls keys
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 0) ? nIsSelected : nIsnotSelected), nTextY + 0 * nSpacingY, (bModify && nSelectedItem == 0) ? "Enter a new key" : engine->olcKeyToStr(tempControls["vacuum"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 1) ? nIsSelected : nIsnotSelected), nTextY + 1 * nSpacingY, (bModify && nSelectedItem == 1) ? "Enter a new key" : engine->olcKeyToStr(tempControls["launchJesusCross"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 2) ? nIsSelected : nIsnotSelected), nTextY + 2 * nSpacingY, (bModify && nSelectedItem == 2) ? "Enter a new key" : engine->olcKeyToStr(tempControls["slapAttack"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 3) ? nIsSelected : nIsnotSelected), nTextY + 3 * nSpacingY, (bModify && nSelectedItem == 3) ? "Enter a new key" : engine->olcKeyToStr(tempControls["jump"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 4) ? nIsSelected : nIsnotSelected), nTextY + 4 * nSpacingY, (bModify && nSelectedItem == 4) ? "Enter a new key" : engine->olcKeyToStr(tempControls["pause"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 5) ? nIsSelected : nIsnotSelected), nTextY + 5 * nSpacingY, (bModify && nSelectedItem == 5) ? "Enter a new key" : engine->olcKeyToStr(tempControls["flyOrEnterDoor"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 6) ? nIsSelected : nIsnotSelected), nTextY + 6 * nSpacingY, (bModify && nSelectedItem == 6) ? "Enter a new key" : engine->olcKeyToStr(tempControls["down"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 7) ? nIsSelected : nIsnotSelected), nTextY + 7 * nSpacingY, (bModify && nSelectedItem == 7) ? "Enter a new key" : engine->olcKeyToStr(tempControls["moveLeft"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 8) ? nIsSelected : nIsnotSelected), nTextY + 8 * nSpacingY, (bModify && nSelectedItem == 8) ? "Enter a new key" : engine->olcKeyToStr(tempControls["moveRight"]), 1, true);
	engine->DrawKirboString(nKeyTextX + ((nSelectedItem == 9) ? nIsSelected : nIsnotSelected), nTextY + 9 * nSpacingY, (bModify && nSelectedItem == 9) ? "Enter a new key" : engine->olcKeyToStr(tempControls["poyo"]), 1, true);

	engine->DrawKirboString(nControllerTextX, nTextY - nSpacingY, "Controller (fixed)", 1, true);

	// controller buttons
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 0) ? nIsSelected : nIsnotSelected), nButtonsTextY + 0 * nSpacingY, sprX);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 1) ? nIsSelected : nIsnotSelected), nButtonsTextY + 1 * nSpacingY, sprY);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 2) ? nIsSelected : nIsnotSelected), nButtonsTextY + 2 * nSpacingY, sprB);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 3) ? nIsSelected : nIsnotSelected), nButtonsTextY + 3 * nSpacingY, sprA);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 4) ? nIsSelected : nIsnotSelected), nButtonsTextY + 4 * nSpacingY, sprPause);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 5) ? nIsSelected : nIsnotSelected), nButtonsTextY + 5 * nSpacingY, sprUp);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 6) ? nIsSelected : nIsnotSelected), nButtonsTextY + 6 * nSpacingY, sprDown);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 7) ? nIsSelected : nIsnotSelected), nButtonsTextY + 7 * nSpacingY, sprLeft);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 8) ? nIsSelected : nIsnotSelected), nButtonsTextY + 8 * nSpacingY, sprRight);
	gfx->DrawSprite(nButtonsTextX + ((nSelectedItem == 9) ? nIsSelected : nIsnotSelected), nButtonsTextY + 9 * nSpacingY, sprRBump);

	if (nSelectedItem == 10)
	{
		gfx->DrawSprite(nDefaultX, nDefaultY, sprSelDefault);
		gfx->DrawSprite(nQuitX, nQuitY, sprUnselQuit);
	}
	else if (nSelectedItem == 11)
	{
		gfx->DrawSprite(nDefaultX, nDefaultY, sprUnselDefault);
		gfx->DrawSprite(nQuitX, nQuitY, sprSelQuit);
	}
	else
	{
		gfx->DrawSprite(nDefaultX, nDefaultY, sprUnselDefault);
		gfx->DrawSprite(nQuitX, nQuitY, sprUnselQuit);
	}

	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

int cControlsMenu::GetSelectedItem()
{
	return nSelectedItem;
}

void cControlsMenu::SetSelectedItem(int i)
{
	nSelectedItem = i;
}

void cControlsMenu::UpdateSavedControls()
{
	std::string ctrls =
		"vacuum=" + engine->olcKeyToStr(tempControls["vacuum"]) + "\n"
		"launchJesusCross=" + engine->olcKeyToStr(tempControls["launchJesusCross"]) + "\n"
		"slapAttack=" + engine->olcKeyToStr(tempControls["slapAttack"]) + "\n"
		"jump=" + engine->olcKeyToStr(tempControls["jump"]) + "\n"
		"pause=" + engine->olcKeyToStr(tempControls["pause"]) + "\n"
		"flyOrEnterDoor=" + engine->olcKeyToStr(tempControls["flyOrEnterDoor"]) + "\n"
		"down=" + engine->olcKeyToStr(tempControls["down"]) + "\n"
		"moveLeft=" + engine->olcKeyToStr(tempControls["moveLeft"]) + "\n"
		"moveRight=" + engine->olcKeyToStr(tempControls["moveRight"]) + "\n"
		"poyo=" + engine->olcKeyToStr(tempControls["poyo"]);

	std::ofstream controls;
	controls.open("assets/ctrls/controls.txt", std::ofstream::trunc);
	controls << ctrls;
	controls.close();
}

void cControlsMenu::ApplyDefaultControls()
{
	tempControls["vacuum"] = olc::Key::SHIFT;
	tempControls["launchJesusCross"] = olc::Key::Z;
	tempControls["slapAttack"] = olc::Key::D;
	tempControls["jump"] = olc::Key::SPACE;
	tempControls["pause"] = olc::Key::P;
	tempControls["flyOrEnterDoor"] = olc::Key::UP;
	tempControls["down"] = olc::Key::DOWN;
	tempControls["moveLeft"] = olc::Key::LEFT;
	tempControls["moveRight"] = olc::Key::RIGHT;
	tempControls["poyo"] = olc::Key::O;
}

bool cControlsMenu::CheckConflicts()
{
	std::map<int, int> aux_map;

	for (const auto& pair : tempControls) ++aux_map[pair.second];

	for (const auto& pair : tempControls)
	{
		std::cout << pair.first << ' ' << pair.second;
		const int num_times = aux_map[pair.second];
		if (num_times > 1)
			return true;
	}

	return false;
}
