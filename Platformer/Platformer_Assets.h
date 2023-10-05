#ifndef DEF_ASSETS
#define DEF_ASSETS

#pragma once
#include "olcPixelGameEngine.h"

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

	olc::Sprite* GetSprite(const std::string& name)
	{
		return m_mapSprites[name];
	}

	void LoadSprites();
	void LoadFont();

private:
	cAssets();
	~cAssets();

	std::map<std::string, olc::Sprite*> m_mapSprites;
};

#endif // !DEF_ASSETS
