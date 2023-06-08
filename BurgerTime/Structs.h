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
	Climbing,
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


