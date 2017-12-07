#ifndef __MSG_FIGHT_H__
#define __MSG_FIGHT_H__

#include "tools/StringTools.h"
#include "logic/GameLogic.h"
#pragma pack(push, 1)

struct sPlayerInfo
{
	int				id;
	string			name;
	bool			isRobot;
	bool			isReady;
};

struct sReady
{
	int				id;
	bool			isReady;
};

struct sGetPork
{
	int				id;
	Poker			poker[1];
};

struct sHitPork
{
	int				id;
	Poker			poker[1];
};

struct sChangeColorPork
{
	int				id;
	PokerColor		color;
};

struct sCalculateBasic
{
	int				id;
	bool			isRobot;
	bool			isLose;
	string			name;
	int				sumScore;
	int				curScore;
};

enum GameOverState
{
	WIN		=	0,
	LOSE	=	2,
	NO_OVER	=	3,
};

struct sCalculate	
{
	int				rolesCount;
	GameOverState	gameOverState;
	int				winnerId;
	sCalculateBasic roles[1]; //╫ги╚пео╒;
};

#pragma pack(pop)

#endif