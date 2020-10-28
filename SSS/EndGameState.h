#pragma once
#include "ScreenState.h"
#include "Button.h"
#include <vector>
#include <iostream>
using namespace std;

class EndGameState : public ScreenState
{
private:
	vector<Button*> m_vButtons;
	enum btn { exit };
	int finalPlayer = 0;
	int finalEnemy = 0;

public:
	EndGameState() { }
	EndGameState(int score, int enemScore);
	void Enter();
	void Exit();
	void Update();
	void Render();
	void Resume() {	cout << "Resuming Game..." << endl; }
};

