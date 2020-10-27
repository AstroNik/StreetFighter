#include "Enemy.h"
#include <algorithm> //for min/max functions

Enemy::Enemy(SDL_Texture* tex, double x, double y) : Player(tex, x, y)
{
    spriteSrcRect = { 0,0,70,80 };
    spriteDestRect = { (int)(m_X - 200),(int)(m_Y - 200) ,140,160 };

    m_velX = 3.0f;
    m_velY = 0.5f;

    isFlipped = true;
    m_dRadius = 40;
    collision = false;
    attackRange = 50;
    canAttack = false;

    AddAnimState("Hadouken", AnimStateDefinition(0, 4, 60));
    AddAnimState("Idle", AnimStateDefinition(1, 4, 110, true, false));
    AddAnimState("Punch", AnimStateDefinition(2, 3, 60));
    AddAnimState("Move", AnimStateDefinition(3, 5, 90));

    AddAnimState("Kick", AnimStateDefinition(6, 5, 90));
    AddAnimState("Roundhouse", AnimStateDefinition(7, 5, 90));
    AddAnimState("Jump", AnimStateDefinition(8, 7, 90, false, true));
    AddAnimState("Crouch", AnimStateDefinition(9, 1, 20));

}

Enemy::~Enemy()
{
    for (int i = 0; i < (int)m_vHadoukens.size(); i++) {
        delete m_vHadoukens[i];
        m_vHadoukens[i] = nullptr;
    }
    m_vHadoukens.clear();
    m_vHadoukens.shrink_to_fit();
}

void Enemy::Render()
{
    for (int i = 0; i < (int)m_vHadoukens.size(); i++)
        m_vHadoukens[i]->Render();

    this->SpriteExAnimated::Render(); //invoke the base class's Render()
}

void Enemy::Update()
{
    this->UpdateEnemy();
}

void Enemy::UpdateEnemy()
{
    if (collision) {
        this->Defend();
    }
    else if (canAttack) {
        this->Attack();
    }
    else if (!canAttack) {
        randMove = rand() % 1000 + 1;
        this->Move();
    }

    spriteSrcRect.x = spriteSrcRect.w * m_iFrame;
    this->UpdateDestRect();
}

void Enemy::Attack() {
    currentAttackTime = SDL_GetTicks();
    if (currentAttackTime > attackDelay + 3000) {
        if (randAttackNum <= 100) {
            PlayState("Punch");
            this->SetAttack("Punch");
        }
        else if (randAttackNum <= 200) {
            PlayState("Kick");
            this->SetAttack("Kick");
        }
        else if (randAttackNum <= 300) {
            PlayState("Roundhouse");
            this->SetAttack("Roundhouse");
        }
        else if (randAttackNum <= 400) {
            PlayState("Hadouken");
            this->SetAttack("Hadouken");
            this->SpawnHadouken();
        }
    }
    else {
        PlayState("Idle");
    }
}

void Enemy::Defend() {
    if (randDefNum <= 50) {
        PlayState("Crouch");
        this->SetDefending(true);
    }
    else {
        this->SetDefending(false);
    }
}

void Enemy::Move() {
    currentTime = SDL_GetTicks();
    if (currentTime > lastTime + 2000) {
        m_velX += m_DX * m_dSpeed;

        m_velX = min(max(m_velX, -(m_velX * fabs(m_DX))), (m_velMax * fabs(m_DX)));

        if (randMove <= 500) {
            m_X -= m_velX * 25;
        }
        else {
            m_X += m_velX * 25;
        }
        PlayState("Move");
        lastTime = currentTime;
    }
    else {
        PlayState("Idle");
        randAttackNum = rand() % 500 + 1;
        randDefNum = rand() % 100 + 1;
    } 
}

void Enemy::UpdateHadoukens()
{
    for (int i = 0; i < (int)m_vHadoukens.size(); i++)
    {
        m_vHadoukens[i]->Update();

        if (m_vHadoukens[i]->GetX() < 0 || m_vHadoukens[i]->GetX() >
            1024 ||
            m_vHadoukens[i]->GetY() < 0 || m_vHadoukens[i]->GetY() > 768)
        {
            delete m_vHadoukens[i];
            m_vHadoukens[i] = nullptr;
            m_vHadoukens.erase(m_vHadoukens.begin() + i);
        }

    }

    m_vHadoukens.shrink_to_fit();

}

void Enemy::SpawnHadouken()
{
    //converting the player spaceship's angle to deltaX and deltaY 
    float dx = (float)cos((0) * M_PI / 180);
    float dy = (float)sin((0) * M_PI / 180);

    m_vHadoukens.push_back(new Hadouken(texture, m_X, m_Y, 0, dx, dy));
}