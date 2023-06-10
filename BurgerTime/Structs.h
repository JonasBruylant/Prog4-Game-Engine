#pragma once

enum class PlayerDirection
{
	Up,
	Right,
	Down,
	Left,
	None,

	Count
};

enum class State
{
	Idle,
	Walking,
	CanClimb,
	CanClimbDown,
	ClimbingDown,
	ClimbingUp,
	Attacking,
	Damaged,
	Stunned,

	Count
};

enum class EnemyType
{
	Egg,
	Bean,
	Pickle,

	Count
};

enum class BurgerPieceType
{
	TopBun,
	Cheese,
	Meat,
	Salad,
	BottomBun
};


