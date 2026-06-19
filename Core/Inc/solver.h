/*
 * solver.h
 */

#ifndef INC_SOLVER_H_
#define INC_SOLVER_H_

typedef enum {
	FORWARD,
	LEFT,
	RIGHT,
	IDLE
} Action;

Action solver();
Action leftWallFollower();
Action floodFill();

#endif /* INC_SOLVER_H_ */
