#pragma once
#include "Player.h"
#include "GameManager.h"
#include "Hadouken.h"

class Enemy : public Player
{
private:
	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;

	void UpdateEnemy();
	int randDefNum;
	int randAttackNum;
	int randMove;
	string attack;

	vector<Hadouken*> m_vHadoukens;
	void UpdateHadoukens();
	
	unsigned int lastTime = 0, currentTime;
	unsigned int attackDelay = 0, currentAttackTime;

public:
	Enemy(SDL_Texture* tex, double x, double y);
	~Enemy();
	void Update() override;
	void Render();
	void Attack();
	void Defend();
	void Move();
	void SetCanAttack(bool atk) { canAttack = atk; }

	int GetHealth()
	{
		return gm.GetHP();
	}

	void Reset(float x) { gm.HealthFix(); m_X = x; }


	void SpawnHadouken();

	vector<Hadouken*>& GetHadoukens() {
		return m_vHadoukens;
	}
};