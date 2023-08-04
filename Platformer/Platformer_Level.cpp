#include "Platformer_Level.h"
#include <strstream>
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cLevel::engine = nullptr;

cLevel::cLevel()
{
	bool wrongLevel = false;
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);

	std::string un = username;

	std::wifstream file("C:/Users/" + un + "/AppData/Roaming/Kirbo Superstar/save.txt");
	if (file)
	{
		std::wstring line;
		while (std::getline(file, line))
		{
            try
            {
                nUnlockedLevel = std::stoi(line);
            }
            catch (const std::invalid_argument& e)
            {
				nUnlockedLevel = 1;
				wrongLevel = true;
            }

			if (nUnlockedLevel < 1 || nUnlockedLevel > 6)
			{
				nUnlockedLevel = 1;
				wrongLevel = true;
			}
		}
	}
	file.close();

    if (wrongLevel)
    {
        std::ofstream save;

        save.open("C:/Users/" + un + "/AppData/Roaming/Kirbo Superstar/save.txt", std::ofstream::trunc);
        save << nUnlockedLevel;
        save.close();
    }
}

bool cLevel::LoadLevel(std::string levelName)
{
	nLevelWidth = 0;
	nLevelHeight = 0;
	sLevel = L"";

	std::wifstream file(levelName);

	if (file)
	{
		std::wstring line;

		while (std::getline(file, line))
		{
			if (line[0] == 'w')
			{
				// Width
				nLevelWidth = std::stoi(line.substr(1, line.size() - 1));
			}
			else if (line[0] == 'h')
			{
				// Height
				nLevelHeight = std::stoi(line.substr(1, line.size() - 1));
			}
			else if (line[0] == 'x')
			{
				// Init player pos X
				nInitPlayerPosX = std::stoi(line.substr(1, line.size() - 1));
			}
			else if (line[0] == 'y')
			{
				// Init player pos Y
				nInitPlayerPosY = std::stoi(line.substr(1, line.size() - 1));
			}
			else
			{
				// Rest of file is the level
				sLevel += line;
			}
		}
	}

	return true;
}

std::wstring cLevel::GetLevel()
{
	return sLevel;
}

int cLevel::GetWidth()
{
	return nLevelWidth;
}

int cLevel::GetHeight()
{
	return nLevelHeight;
}

int cLevel::GetInitPlayerPosX()
{
	return nInitPlayerPosX;
}

int cLevel::GetInitPlayerPosY()
{
	return nInitPlayerPosY;
}

bool cLevel::PopulateEnnemies(std::vector<cDynamicCreature*>& vecDyns, std::string levelName)
{
	std::wifstream file(levelName);

	if (file)
	{
		std::wstring line;

		while (std::getline(file, line))
		{
			std::wstring temp;
			std::vector<std::wstring> parts;
			std::wstringstream wss(line);
			while (std::getline(wss, temp, L','))
			{
				parts.push_back(temp);
			}

			// Populate a Blade Knight
			if (parts[0] == L"bladeKnight")
			{
				cDynamicCreature* mob = new cDynamicCreatureBladeKnight(this);
				vecDyns.push_back(mob);
				mob->InitDecal();
				mob->SetPosition(std::stoi(parts[1]), std::stoi(parts[2]));
			}

			// Populate a bomber
			if (parts[0] == L"bomber")
			{
				cDynamicCreature* mob = new cDynamicCreatureBomber(this);
				vecDyns.push_back(mob);
				mob->InitDecal();
				mob->SetPosition(std::stoi(parts[1]), std::stoi(parts[2]));
			}

			// Populate a Bronto Burt
			if (parts[0] == L"brontoBurt")
			{
				cDynamicCreature* mob = new cDynamicCreatureBrontoBurt(this, std::stof(parts[1]));
				vecDyns.push_back(mob);
				mob->InitDecal();
				mob->SetPosition(std::stoi(parts[2]), std::stoi(parts[3]));
			}

			// Populate a rocky
			if (parts[0] == L"rocky")
			{
				cDynamicCreature* mob = new cDynamicCreatureRocky(this);
				vecDyns.push_back(mob);
				mob->InitDecal();
				mob->SetPosition(std::stoi(parts[1]), std::stoi(parts[2]));
			}

			// Populate a scarfy
			if (parts[0] == L"scarfy")
			{
				cDynamicCreature* mob = new cDynamicCreatureScarfy(this);
				vecDyns.push_back(mob);
				mob->InitDecal();
				mob->SetPosition(std::stoi(parts[1]), std::stoi(parts[2]));
			}

			// Populate a waddle dee
			if (parts[0] == L"waddleDee")
			{
				cDynamicCreature* mob = new cDynamicCreatureWaddleDee(this);
				vecDyns.push_back(mob);
				mob->InitDecal();
				mob->SetPosition(std::stoi(parts[1]), std::stoi(parts[2]));
			}
		}
	}

	return true;
}

bool cLevel::PopulateMechanisms(std::string levelName)
{
	std::wifstream file(levelName);

	if (file)
	{
		std::wstring line;

		while (std::getline(file, line))
		{
			std::wstring temp;
			std::vector<std::wstring> parts;
			std::wstringstream wss(line);
			while (std::getline(wss, temp, L','))
			{
				parts.push_back(temp);
			}

			// Populate a basic platform
			if (parts[0] == L"Basic")
			{
				engine->AddPlatform(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4]);
			}
			if (parts[0] == L"HorizontalSine")
			{
				engine->AddHorizontalSinePtfm(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], std::stof(parts[5]), std::stof(parts[6]), parts[7]);
			}
			if (parts[0] == L"VerticalSine")
			{
				engine->AddVerticalSinePtfm(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], std::stof(parts[5]), std::stof(parts[6]), parts[7]);
			}
			if (parts[0] == L"HorizontalSingle")
			{
				engine->AddHorizontalSinglePtfm(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], std::stof(parts[5]), std::stof(parts[6]), parts[7]);
			}
			if (parts[0] == L"VerticalSingle")
			{
				engine->AddVerticalSinglePtfm(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], std::stof(parts[5]), std::stof(parts[6]), parts[7]);
			}
			if (parts[0] == L"Wall")
			{
				std::string id = "";
				if (parts.size() == 10)
				{
					id = engine->ToStr(parts[9]);
				}
				engine->AddWall(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], parts[5], parts[6], std::stof(parts[7]), std::stof(parts[8]), id);
			}
			if (parts[0] == L"Ceiling")
			{
				engine->AddCeiling(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], parts[5]);
			}
			if (parts[0] == L"HorizontalCrusher")
			{
				engine->AddHorizontalCrusher(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], std::stof(parts[5]));
			}
			if (parts[0] == L"VerticalCrusher")
			{
				engine->AddVerticalCrusher(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], std::stof(parts[5]));
			}
			if (parts[0] == L"HarmfulBloc")
			{
				engine->AddHarmfulBloc(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), std::stof(parts[4]), parts[5]);
			}
			if (parts[0] == L"Wind")
			{
				engine->AddWind(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), parts[4], std::stof(parts[5]));
			}
			if (parts[0] == L"Teleport")
			{
				engine->AddTeleport(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]), std::stof(parts[4]), engine->ToStr(parts[5]));
			}
			if (parts[0] == L"Scenery")
			{
				bool foreground = false;
                if (parts.size() == 5 && engine->ToStr(parts[4]) == "foreground")
                {
					foreground = true;
                }
                engine->AddScenery(std::stof(parts[1]), std::stof(parts[2]), engine->ToStr(parts[3]), foreground);
			}
		}
	}

	return true;
}

bool cLevel::PopulateBoss(std::vector<cDynamicCreature*>& vecDyns)
{
	cDynamicCreature* mob = nullptr;
	switch (nCurrentLevel)
	{
		case 0:
		{
			mob = new cDynamicCreatureWhispyWood(this);
			vecDyns.push_back(mob);
			mob->SetPosition(cfWhispyWoodPX, cfWhispyWoodPY);
		}
		break;
		case 1:
		{
			mob = new cDynamicCreatureMrShineMrBright(this);
			vecDyns.push_back(mob);
			mob->SetPosition(cfMrShineMrBrightPX, cfMrShineMrBrightPY);
		}
		break;
		case 2:
		{
			mob = new cDynamicCreatureFrosty(this);
			vecDyns.push_back(mob);
			mob->SetPosition(cfFrostyPX, cfFrostyPY);
		}
		break;
		case 3:
		{
			mob = new cDynamicCreatureKracko(this);
			vecDyns.push_back(mob);
			mob->SetPosition(cfKrackoPX, cfKrackoPY);
		}
		break;
		case 4:
		{
			mob = new cDynamicCreatureSSTierMetaKnight(this);
			vecDyns.push_back(mob);
			mob->SetPosition(cfSSTierMetaKnightPX, cfSSTierMetaKnightPY);
		}
		break;
		case 5:
		{
			mob = new cDynamicCreatureKingDDD(this);
			vecDyns.push_back(mob);
			mob->SetPosition(cfKingDDDPX, cfKingDDDPY);
		}
		break;
	}

	mob->InitDecal();
	mob->SetVacuumable(false);
	mob->SetBoss(true);
	mob->SetKnockable(false);

	return true;
}

void cLevel::DrawTiles(int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY)
{
	// Get offsets for smooth movement
	float fTileOffsetX = (fOffsetX - (int)fOffsetX) * engine->GetTileWidth();
	float fTileOffsetY = (fOffsetY - (int)fOffsetY) * engine->GetTileHeight();

    SelectTile(0, 16, nVisibleTilesX, nVisibleTilesY, fOffsetX, fOffsetY, fTileOffsetX, fTileOffsetY);
}

void cLevel::SelectTile(int startX, int endX, int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY)
{
	float tileW = engine->GetTileWidth();
	float tileH = engine->GetTileHeight();

	for (int x = startX; x < endX; x++)
	{
		for (int y = -2; y < nVisibleTilesY + 2; y++)
		{
			wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);

            olc::vf2d pos;
            pos.x = (int)(x * tileW - fTileOffsetX);
            pos.y = (int)(y * tileH - fTileOffsetY);

			switch (sTileID)
			{
				case L'G': // Ground Block
					DrawGroundTile(x, y, fTileOffsetX, fTileOffsetY, fOffsetX, fOffsetY, engine->GetGroundTilesDecal(), sTileID);
					break;
				case L'#': // Solid Block
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 0 * tileW, 0 * tileH }, { tileW, tileH });
					break;
				case L'B': // Breakable Block
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 1 * tileW, 0 * tileH }, { tileW, tileH });
					break;
				case L'_': // Semi solid platform
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 2 * tileW, 0 * tileH }, { tileW, tileH });
					break;
				case L'c': // Candy
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 3 * tileW, 0 * tileH }, { tileW, tileH });
					break;
				case L't': // Tomato
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 4 * tileW, 0 * tileH }, { tileW, tileH });
					break;
				case L's': // Strength boost
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 0 * tileW, 1 * tileH }, { tileW, tileH });
					break;
				case L'd': // Defense boost
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 1 * tileW, 1 * tileH }, { tileW, tileH });
					break;
				case L'P': // Partially broken Block
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 2 * tileW, 1 * tileH }, { tileW, tileH });
					break;
				case L'H': // Heavily damaged Block
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 3 * tileW, 1 * tileH }, { tileW, tileH });
					break;
				case L'x': // Minor heal item
					engine->DrawPartialDecal(pos, engine->GetSpecialTilesDecal(), { 4 * tileW, 1 * tileH }, { tileW, tileH });
					break;
				case L'w': // Door
					engine->DrawDecal({ x * tileW - fTileOffsetX, y * tileH - fTileOffsetY + (tileH - engine->GetDoorDecal()->sprite->height) + 5.0f }, engine->GetDoorDecal());
					break;
			}
		}
	}
}

void cLevel::DrawGroundTile(int x, int y, float fTileOffsetX, float fTileOffsetY, float fCamOffsetX, float fCamOffsetY, olc::Decal* decalTiles, wchar_t tile)
{
    float tileW = engine->GetTileWidth();
    float tileH = engine->GetTileHeight();

    olc::vf2d pos;
    pos.x = (int)(x * tileW - fTileOffsetX);
    pos.y = (int)(y * tileH - fTileOffsetY);

    // Get all tiles around the current tile
    wchar_t tilesAround[3][3];
    tilesAround[0][0] = GetTile(x - 1 + fCamOffsetX, y - 1 + fCamOffsetY); tilesAround[0][1] = GetTile(x + 0 + fCamOffsetX, y - 1 + fCamOffsetY); tilesAround[0][2] = GetTile(x + 1 + fCamOffsetX, y - 1 + fCamOffsetY);
    tilesAround[1][0] = GetTile(x - 1 + fCamOffsetX, y + 0 + fCamOffsetY); tilesAround[1][1] = tile;											  tilesAround[1][2] = GetTile(x + 1 + fCamOffsetX, y + 0 + fCamOffsetY);
    tilesAround[2][0] = GetTile(x - 1 + fCamOffsetX, y + 1 + fCamOffsetY); tilesAround[2][1] = GetTile(x + 0 + fCamOffsetX, y + 1 + fCamOffsetY); tilesAround[2][2] = GetTile(x + 1 + fCamOffsetX, y + 1 + fCamOffsetY);

    // Check the 47 configurations
    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 0 * tileW, 0 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 1 * tileW, 0 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 2 * tileW, 0 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 3 * tileW, 0 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 4 * tileW, 0 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 5 * tileW, 0 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 6 * tileW, 0 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 7 * tileW, 0 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 0 * tileW, 1 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 1 * tileW, 1 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 2 * tileW, 1 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 3 * tileW, 1 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 4 * tileW, 1 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 5 * tileW, 1 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 6 * tileW, 1 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 7 * tileW, 1 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 0 * tileW, 2 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 1 * tileW, 2 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 2 * tileW, 2 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 3 * tileW, 2 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 4 * tileW, 2 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 5 * tileW, 2 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 6 * tileW, 2 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 7 * tileW, 2 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 0 * tileW, 3 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 1 * tileW, 3 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 2 * tileW, 3 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 3 * tileW, 3 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 4 * tileW, 3 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 5 * tileW, 3 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 6 * tileW, 3 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 7 * tileW, 3 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && /*tilesAround[0][2]*/
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 0 * tileW, 4 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 1 * tileW, 4 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] == tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 2 * tileW, 4 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] == tile && tilesAround[2][2] != tile)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 3 * tileW, 4 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        tilesAround[2][0] == tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 4 * tileW, 4 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        tilesAround[2][0] != tile && tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 5 * tileW, 4 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] == tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 6 * tileW, 4 * tileH }, { tileW, tileH });
        return;
    }

    if (tilesAround[0][0] != tile && tilesAround[0][1] == tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 7 * tileW, 4 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] == tile &&
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 0 * tileW, 5 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && tilesAround[0][2] != tile &&
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 1 * tileW, 5 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] == tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 2 * tileW, 5 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] == tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 3 * tileW, 5 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] == tile && /*tilesAround[0][2]*/
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 4 * tileW, 5 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] == tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 5 * tileW, 5 * tileH }, { tileW, tileH });
        return;
    }

    if (/*tilesAround[0][0]*/		 tilesAround[0][1] != tile && /*tilesAround[0][2]*/
        tilesAround[1][0] != tile && tilesAround[1][1] == tile && tilesAround[1][2] != tile &&
        /*tilesAround[2][0]*/		 tilesAround[2][1] != tile	  /*tilesAround[2][2]*/)
    {
        engine->DrawPartialDecal(pos, decalTiles, { 6 * tileW, 5 * tileH }, { tileW, tileH });
        return;
    }
}

wchar_t cLevel::GetTile(int x, int y)
{
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
		return sLevel[y * nLevelWidth + x];
	else
		return L' ';
}

void cLevel::SetTile(int x, int y, wchar_t c)
{
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
		sLevel[y * nLevelWidth + x] = c;
}

void cLevel::LoadLevelsList()
{
	levels.push_back("assets/lvls/lvl1.txt");
	levels.push_back("assets/lvls/lvl2.txt");
	levels.push_back("assets/lvls/lvl3.txt");
	levels.push_back("assets/lvls/lvl4.txt");
	levels.push_back("assets/lvls/lvl5.txt");
	levels.push_back("assets/lvls/lvl6.txt");
}

void cLevel::LoadBossesList()
{
	bosses.push_back("assets/lvls/Bosslvl1.txt");
	bosses.push_back("assets/lvls/Bosslvl2.txt");
	bosses.push_back("assets/lvls/Bosslvl3.txt");
	bosses.push_back("assets/lvls/Bosslvl4.txt");
	bosses.push_back("assets/lvls/Bosslvl5.txt");
	bosses.push_back("assets/lvls/Bosslvl6.txt");
}

void cLevel::LoadEnnemiesList()
{
	ennemies.push_back("assets/lvls/ennemiesLvl1.txt");
	ennemies.push_back("assets/lvls/ennemiesLvl2.txt");
	ennemies.push_back("assets/lvls/ennemiesLvl3.txt");
	ennemies.push_back("assets/lvls/ennemiesLvl4.txt");
	ennemies.push_back("assets/lvls/ennemiesLvl5.txt");
	ennemies.push_back("assets/lvls/ennemiesLvl6.txt");
}

void cLevel::LoadMechanismsList()
{
	mechanisms.push_back("assets/lvls/mechanismsLvl1.txt");
	mechanisms.push_back("assets/lvls/mechanismsLvl2.txt");
	mechanisms.push_back("assets/lvls/mechanismsLvl3.txt");
	mechanisms.push_back("assets/lvls/mechanismsLvl4.txt");
	mechanisms.push_back("assets/lvls/mechanismsLvl5.txt");
	mechanisms.push_back("assets/lvls/mechanismsLvl6.txt");
}

void cLevel::LoadBossMechanismsList()
{
	bossMechanisms.push_back("assets/lvls/mechanismsBoss1.txt");
	bossMechanisms.push_back("assets/lvls/mechanismsBoss2.txt");
	bossMechanisms.push_back("assets/lvls/mechanismsBoss3.txt");
	bossMechanisms.push_back("assets/lvls/mechanismsBoss4.txt");
	bossMechanisms.push_back("assets/lvls/mechanismsBoss5.txt");
	bossMechanisms.push_back("assets/lvls/mechanismsBoss6.txt");
}

void cLevel::LoadSpecialTilesList()
{
	specialTiles.push_back("assets/gfx/tilemap00.png");
	specialTiles.push_back("assets/gfx/tilemap01.png");
	specialTiles.push_back("assets/gfx/tilemap02.png");
	specialTiles.push_back("assets/gfx/tilemap03.png");
	specialTiles.push_back("assets/gfx/tilemap04.png");
	specialTiles.push_back("assets/gfx/tilemap05.png");
}

void cLevel::LoadGroundTilesList()
{
	groundTiles.push_back("assets/gfx/grdTileMap00.png");
	groundTiles.push_back("assets/gfx/grdTileMap01.png");
	groundTiles.push_back("assets/gfx/grdTileMap02.png");
	groundTiles.push_back("assets/gfx/grdTileMap03.png");
	groundTiles.push_back("assets/gfx/grdTileMap04.png");
	groundTiles.push_back("assets/gfx/grdTileMap05.png");
}

void cLevel::LoadBackGroundsList()
{
	backgrounds.push_back("assets/gfx/BckGrd00.png");
	backgrounds.push_back("assets/gfx/BckGrd01.png");
	backgrounds.push_back("assets/gfx/BckGrd02.png");
	backgrounds.push_back("assets/gfx/BckGrd03.png");
	backgrounds.push_back("assets/gfx/BckGrd04.png");
	backgrounds.push_back("assets/gfx/BckGrd05.png");

	parallaxLayer1.push_back("assets/gfx/ParallaxLayer100.png");
	parallaxLayer1.push_back("assets/gfx/ParallaxLayer101.png");
	parallaxLayer1.push_back("assets/gfx/ParallaxLayer102.png");
	parallaxLayer1.push_back("assets/gfx/ParallaxLayer103.png");
	parallaxLayer1.push_back("assets/gfx/ParallaxLayer104.png");
	parallaxLayer1.push_back("assets/gfx/ParallaxLayer105.png");

	parallaxLayer2.push_back("assets/gfx/ParallaxLayer200.png");
	parallaxLayer2.push_back("assets/gfx/ParallaxLayer201.png");
	parallaxLayer2.push_back("assets/gfx/ParallaxLayer202.png");
	parallaxLayer2.push_back("assets/gfx/ParallaxLayer203.png");
	parallaxLayer2.push_back("assets/gfx/ParallaxLayer204.png");
	parallaxLayer2.push_back("assets/gfx/ParallaxLayer205.png");

	parallaxLayer3.push_back("assets/gfx/ParallaxLayer300.png");
	parallaxLayer3.push_back("assets/gfx/ParallaxLayer301.png");
	parallaxLayer3.push_back("assets/gfx/ParallaxLayer302.png");
	parallaxLayer3.push_back("assets/gfx/ParallaxLayer303.png");
	parallaxLayer3.push_back("assets/gfx/ParallaxLayer304.png");
	parallaxLayer3.push_back("assets/gfx/ParallaxLayer305.png");
}

void cLevel::LoadBossesBackGroundsList()
{
	bossesBackgrounds.push_back("assets/gfx/BossBckGrd00.png");
	bossesBackgrounds.push_back("assets/gfx/BossBckGrd01.png");
	bossesBackgrounds.push_back("assets/gfx/BossBckGrd02.png");
	bossesBackgrounds.push_back("assets/gfx/BossBckGrd03.png");
	bossesBackgrounds.push_back("assets/gfx/BossBckGrd04.png");
	bossesBackgrounds.push_back("assets/gfx/BossBckGrd05.png");
}

void cLevel::LoadMusicsList()
{
	musics.push_back("assets/snd/lvl00.wav");
	musics.push_back("assets/snd/lvl01.wav");
	musics.push_back("assets/snd/lvl02.wav");
	musics.push_back("assets/snd/lvl03.wav");
	musics.push_back("assets/snd/lvl04.wav");
	musics.push_back("assets/snd/lvl05.wav");
}

void cLevel::LoadBossesMusicsList()
{
	bossesMusics.push_back("assets/snd/bossLvl00.wav");
	bossesMusics.push_back("assets/snd/bossLvl01.wav");
	bossesMusics.push_back("assets/snd/bossLvl02.wav");
	bossesMusics.push_back("assets/snd/bossLvl01.wav"); // the music is the same for "Mr Shine and Mr Bright" and "Kracko"
	bossesMusics.push_back("assets/snd/bossLvl04.wav");
	bossesMusics.push_back("assets/snd/bossLvl05.wav");
}

std::string cLevel::GetName()
{
	return levels[nCurrentLevel];
}

std::string cLevel::GetBoss()
{
	return bosses[nCurrentLevel];
}

std::string cLevel::GetEnnemies()
{
	return ennemies[nCurrentLevel];
}

std::string cLevel::GetMechanisms()
{
	return mechanisms[nCurrentLevel];
}

std::string cLevel::GetBossMechanisms()
{
	return bossMechanisms[nCurrentLevel];
}

std::string cLevel::GetSpecialTilesSpritesheet()
{
	return specialTiles[nCurrentLevel];
}

std::string cLevel::GetGroundTilesSpritesheet()
{
	return groundTiles[nCurrentLevel];
}

std::string cLevel::GetBackGroundSpritesheet()
{
	return backgrounds[nCurrentLevel];
}

std::string cLevel::GetParallax1Spritesheet()
{
	return parallaxLayer1[nCurrentLevel];
}

std::string cLevel::GetParallax2Spritesheet()
{
	return parallaxLayer2[nCurrentLevel];
}

std::string cLevel::GetParallax3Spritesheet()
{
	return parallaxLayer3[nCurrentLevel];
}

std::string cLevel::GetBossBackGroundSpritesheet()
{
	return bossesBackgrounds[nCurrentLevel];
}

std::string cLevel::GetMusic()
{
	return musics[nCurrentLevel];
}

std::string cLevel::GetBossMusic()
{
	return bossesMusics[nCurrentLevel];
}

int cLevel::GetCurrentLvl()
{
	return nCurrentLevel;
}

void cLevel::SetCurrentLvl(int selectedLvl)
{
	nCurrentLevel = selectedLvl;
}

int cLevel::GetUnlockedLvl()
{
	return nUnlockedLevel;
}

void cLevel::UnlockNewLvl()
{
	nUnlockedLevel++;

	std::ofstream save;

	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);

	std::string un = username;

	save.open("C:/Users/" + un + "/AppData/Roaming/Kirbo Superstar/save.txt", std::ofstream::trunc);
	save << nUnlockedLevel;
	save.close();
}

bool cLevel::IsLastOfGame()
{
	return nCurrentLevel + 1 == levels.size();
}

bool cLevel::IsLastUnlocked()
{
	return nCurrentLevel + 1 == nUnlockedLevel;
}
