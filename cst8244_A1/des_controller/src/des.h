/*
 * des.h
 *
 *  An example H file that you can use as a model for defining enums for the inputs, outputs, and state names.
 *  This approach will make your code more readable :)
 */

#ifndef DES_H_
#define DES_H_

#define NUM_STATES 8
typedef enum {
	START_STATE = 0,
	READY_STATE = 1,
	LEFT_DOWN_STATE = 2,
	RIGHT_DOWN_STATE = 3,
	ARMED_STATE = 4,
	PUNCH_STATE = 5,
	EXIT_STATE = 6,
	STOP_STATE = 7
} State;

#define NUM_INPUTS 5
typedef enum {
	LEFT_BUTTON_DOWN = 0,
	LEFT_BUTTON_UP = 1,
	RIGHT_BUTTON_DOWN = 2,
	RIGHT_BUTTON_UP = 3,
	STOP_BUTTON = 4
} Input;

#define NUM_OUTPUTS 8
typedef enum {
	START_MSG = 0,
	READY_MSG = 1,
	LEFT_DOWN_MSG = 2,
	RIGHT_DOWN_MSG = 3,
	ARMED_MSG = 4,
	PUNCH_MSG = 5,
	EXIT_MSG = 6,
	STOP_MSG = 7
} Output;

#define NUM_ACTIONS 11
typedef enum {
	LEFT_SCAN = 0,
	RIGHT_SCAN = 1,
	WEIGHT_SCALE = 2,
	LEFT_OPEN = 3,
	RIGHT_OPEN = 4,
	LEFT_CLOSED = 5,
	RIGHT_CLOSED = 6,
	GUARD_RIGHT_UNLOCK = 7,
	GUARD_RIGHT_LOCK = 8,
	GUARD_LEFT_LOCK = 9,
	GUARD_LEFT_UNLOCK = 10
} Actions;

const char *outMessage[NUM_OUTPUTS] = {
		"Start Message",
		"Ready.",
		"Left Button Down = Press Right Button to Arm Press",
		"Right Button Down - Press Left Button to Arm Press",
		"DANGER - Press Armed",
		"Punching.",
		"Exiting.",
		"Stop Message."
};

const char *inMessage[NUM_INPUTS] = {
		"LD",
		"LU",
		"RD",
		"RU",
		"S"
};

const char *outDisplayMessage[NUM_ACTIONS] = {
		"Left Scan",
		"Right Scan",
		"Weight Scale",
		"Person opened left door",
		"Person opened right door",
		"Left Closed",
		"Right Closed",
		"Right door unlocked by guard",
		"Right door locked by guard",
		"Left door locked by guard",
		"Left door unlocked by guard"
};

#endif /* PROJ_H_ */
