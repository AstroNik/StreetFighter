#pragma once
#include "SpriteEx.h"

class Hadouken : public SpriteExAnimated
{
public:
	friend class Player;
	Hadouken(SDL_Texture* tex, double x, double y, double a, double dx, double dy)
		:
		SpriteExAnimated(tex, x, y, a)
	{
		m_DX = dx;
		m_DY = dy;
		spriteSrcRect = { 0,0,70,80 };
		spriteDestRect = { (int)(m_X - 300),(int)(m_Y - 200)  ,140,160 };
		m_dRadius = 15;

		AddAnimState("Fireball", AnimStateDefinition(4, 2, 90));
		AddAnimState("FireballExplode", AnimStateDefinition(5, 4, 90));
	}

	void Update()
	{
		m_X += (m_DX * 2.5f);
		m_Y += (m_DY * 2.5f);
		PlayState("Fireball");
		this->UpdateDestRect();
	}
};
