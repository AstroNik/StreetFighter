#include "GameState.h"
#include "Game.h"
#include "PauseMenuPopupState.h"
#include "EndGameState.h"

void GameState::Enter()
{
	mainSpriteTex = Game::Instance()->LoadTexture("Img/Players/PlayerKenSprite2.png");
	EnemySpriteTex = Game::Instance()->LoadTexture("Img/Players/EnemySprite.png");

	m_pFont = TTF_OpenFont("Font/LTYPE.TTF", 30);

	SDL_Rect bgSrcRect;
	bgSrcRect.x = bgSrcRect.y = 0;
	SDL_QueryTexture(bgSpriteTex, NULL, NULL, &bgSrcRect.w, &bgSrcRect.h);

	SDL_Rect bgDestRect;
	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	bg = new SpriteEx(bgSpriteTex, bgSrcRect, bgDestRect);

	gm = new GameManager();
	  
	player = new Player(mainSpriteTex, bgDestRect.w*0.25, bgDestRect.h - 100);

	enemy = new Enemy(EnemySpriteTex, bgDestRect.w * 0.95, bgDestRect.h - 100);
	
	playerXPos = bgDestRect.w * 0.25;

	enemyXPos = bgDestRect.w * 0.95;

}


void GameState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_ESCAPE) == 1)
	{
		Game::Instance()->GetFSM()->PushState(new PauseMenuPopupState());
		return; //to make sure we don't run the game in background while pause menu shows
	}

	if (player) player->Update();

	if (enemy) enemy->Update();
 

	if (player) this->CheckCollision();
	if (enemy) this->CheckAttackRange();

	if (enemy->GetHealth() <= 0)
	{
		playWins += 1;
		enemy->Reset(enemyXPos);
		player->Reset(playerXPos);
	}
	else if (player->GetHealth() <= 0)
	{
		enemWins += 1;
		player->Reset(playerXPos);
		enemy->Reset(enemyXPos);
	}
 
	if (playWins == 3 || enemWins == 3)
	{
		SDL_Delay(500);
		Game::Instance()->GetFSM()->ChangeState(new EndGameState(playWins,enemWins));
	}
}


void GameState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255); //set background window color
	SDL_RenderClear(Game::Instance()->GetRenderer()); //clear the screen

	bg->Render();

	if (player) player->Render();
	if (enemy) enemy->Render();

	ScreenState::Render();

}


void GameState::Exit()
{
	//to-do:: de-allocate memory, texture, surface etc, sound 
	//player->~Player();
	//enemy->~Enemy();
}

void GameState::CheckCollision()
{
	if (CircleCollisionTest((player->GetX() + player->GetRadius()), (player->GetY() + player->GetRadius()),
		(enemy->GetX() + enemy->GetRadius()), (enemy->GetY() + enemy->GetRadius()), //need to add in radius to get center of rendered image for proper midpoint value
		player->GetRadius(), enemy->GetRadius())) 
	{
		player->SetCollision(true);
		enemy->SetCollision(true);
		if (player->GetAttack() == "Punch" && !enemy->GetDefending())
		{
			if (player->GetAnimState())
				enemy->TakeHit(5);
		}
		if (player->GetAttack() == "Kick" && !enemy->GetDefending())
		{
			if (player->GetAnimState())
				enemy->TakeHit(10);
		}
		if (player->GetAttack() == "Roundhouse" && !enemy->GetDefending()) 
		{
			if (player->GetAnimState())
				enemy->TakeHit(15);
		}
	}
	else if (player->GetAttack() == "Hadouken" && !enemy->GetDefending()) 
	{
		if (player->GetAnimState())
			cout << "here" << endl;
		this->CheckPlayerHadoukenHit();
	}
	else {
		player->SetCollision(false);
		enemy->SetCollision(false);
	}
}

void GameState::CheckAttackRange()
{
	if (CircleCollisionTest((player->GetX() + player->GetAttackRange()), (player->GetY() + player->GetAttackRange()),
		(enemy->GetX() + enemy->GetAttackRange()), (enemy->GetY() + enemy->GetAttackRange()), //need to add in radius to get center of rendered image for proper midpoint value
		player->GetAttackRange(), enemy->GetAttackRange()))
	{
		enemy->SetCanAttack(true);
		if (enemy->GetAttack() == "Punch" && !player->GetDefending()) {
			if(enemy->GetAnimState())
				player->TakeHit(5);
		}
		if (enemy->GetAttack() == "Kick" && !player->GetDefending()) {
			if (enemy->GetAnimState())
				player->TakeHit(10);
		}
		if (enemy->GetAttack() == "Roundhouse" && !player->GetDefending()) {
			if (enemy->GetAnimState())
				player->TakeHit(15);
		}
	} 
	else if (enemy->GetAttack() == "Hadouken" && !player->GetDefending()) 
	{
		if (enemy->GetAnimState())
			this->CheckEnemyHadoukenHit();
	}
	else {
		enemy->SetCanAttack(false);
	}
}


void GameState::CheckPlayerHadoukenHit()
{
	bool isBreakOutOfLoop = false;
	//check for bullet and asteroid collision
	for (int b = 0; b < (int)player->GetHadoukens().size(); b++)
	{
		Hadouken* hadouken = player->GetHadoukens()[b];

		if (CircleCollisionTest(hadouken->GetX(), hadouken->GetY(),
			enemy->GetX(), enemy->GetY(),
			hadouken->GetRadius(), enemy->GetRadius()
		))
		{
			cout << "Hadouken collided with the enemy!!\n";

			enemy->TakeHit(30);

			//cleanup/destroy the bullet
			delete hadouken;
			player->GetHadoukens()[b] = nullptr;
			player->GetHadoukens().erase(player->GetHadoukens().begin() + b);

			isBreakOutOfLoop = true;

		}
		if (isBreakOutOfLoop)	break;
	}

	if (isBreakOutOfLoop)
	{
		player->GetHadoukens().shrink_to_fit();
	}
}

void GameState::CheckEnemyHadoukenHit()
{
	bool isBreakOutOfLoop = false;
	//check for bullet and asteroid collision
	for (int b = 0; b < (int)enemy->GetHadoukens().size(); b++)
	{
		Hadouken* hadouken = enemy->GetHadoukens()[b];

		if (CircleCollisionTest(hadouken->GetX(), hadouken->GetY(),
			player->GetX(), player->GetY(),
			hadouken->GetRadius(), player->GetRadius()
		))
		{
			cout << "Hadouken collided with the player!!\n";

			enemy->TakeHit(30);

			//cleanup/destroy the bullet
			delete hadouken;
			enemy->GetHadoukens()[b] = nullptr;
			enemy->GetHadoukens().erase(enemy->GetHadoukens().begin() + b);

			isBreakOutOfLoop = true;

		}
		if (isBreakOutOfLoop)	break;
	}

	if (isBreakOutOfLoop)
	{
		enemy->GetHadoukens().shrink_to_fit();
	}
}