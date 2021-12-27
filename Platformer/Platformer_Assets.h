#ifndef DEF_ASSETS
#define DEF_ASSETS

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

#include <map>

class cAssets
{
public:
	static cAssets& get()
	{
		static cAssets me;
		return me;
	}

	cAssets(cAssets const&) = delete;
	void operator=(cAssets const&) = delete;

	olc::Sprite* GetSprite(std::string name)
	{
		return m_mapSprites[name];
	}
	/*
	* TODO quand la classe item sera créée
	*
	cItem* GetItem(string name)
	{
		return m_mapItems[name];
	}
	*/

	void LoadSprites();
	void LoadItems();

private:
	cAssets();
	~cAssets();

	std::map<std::string, olc::Sprite*> m_mapSprites;

	// TODO quand la classe item sera créée
	//std::map<std::string, cItem*> m_mapItems;
};

#endif // !DEF_ASSETS
