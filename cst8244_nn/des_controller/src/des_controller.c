///
///
///
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "./des.h"

State state = READY_STATE;
send_t msg_received;
response_t response;
pid_t display_pid;
name_attach_t *attach;
int coid;
int rcvid;
int sendMsg;

int main(void) {
	//namespace
	if ((coid = name_open(NAME_ATTACH_DISPLAY, 0)) == -1) {
		perror("name_open failed.");
		return EXIT_FAILURE;
	}
//chid , dispatch
	if ((attach = name_attach(NULL, NAME_ATTACH_CONTROLLER, 0)) == NULL) {
		return EXIT_FAILURE;

	}

	printf("Controller PID: %d\n", getpid());

	while (1) {

		sendMsg = 0;
//attach to chid pointer
		if ((rcvid = MsgReceive(attach->chid, &msg_received, sizeof(send_t),
				NULL)) == -1) {
			printf("Could not receive message from inputs.");
			return EXIT_FAILURE;
		}

		if (msg_received.input == EX_INPUT) {
			state = EXIT_STATE;
		}

		switch (state) {
		case READY_STATE:
			if (msg_received.input == LEFT_SCAN) {
				state = LEFT_SCAN_STATE;
				sendMsg = 1;
			} else if (msg_received.input == RIGHT_SCAN) {
				state = RIGHT_SCAN_STATE;
				sendMsg = 1;
			}
			break;
		case LEFT_SCAN_STATE:
			if (msg_received.input == GUARD_LEFT_UNLOCK) {
				state = ENTER_LEFT_UNLOCK_STATE;
				sendMsg = 1;
			}
			break;
		case RIGHT_SCAN_STATE:
			if (msg_received.input == GUARD_RIGHT_UNLOCK) {
				state = LEAVE_RIGHT_UNLOCK_STATE;
				sendMsg = 1;
			}
			break;
		case ENTER_LEFT_UNLOCK_STATE:
			if (msg_received.input == LEFT_OPEN) {
				state = ENTER_LEFT_OPEN_STATE;
				sendMsg = 1;
			}
			break;
		case ENTER_LEFT_OPEN_STATE:
			if (msg_received.input == WEIGHT_SCAN) {
				state = ENTER_WEIGHTSCAN_STATE;
				sendMsg = 1;
			}
			break;
		case ENTER_WEIGHTSCAN_STATE:
			if (msg_received.input == LEFT_CLOSE) {
				state = ENTER_LEFT_CLOSE_STATE;
				sendMsg = 1;
			}
			break;
		case ENTER_LEFT_CLOSE_STATE:
			if (msg_received.input == GUARD_LEFT_LOCK) {
				state = ENTER_LEFTLOCK_STATE;
				sendMsg = 1;
			}
			break;
		case ENTER_LEFTLOCK_STATE:
			if (msg_received.input == GUARD_RIGHT_UNLOCK) {
				state = ENTER_RIGHT_UNLOCK_STATE;
				sendMsg = 1;
			}
			break;
		case ENTER_RIGHT_UNLOCK_STATE:
			if (msg_received.input == RIGHT_OPEN) {
				state = ENTER_RIGHT_OPEN_STATE;
				sendMsg = 1;
			}
			break;
		case ENTER_RIGHT_OPEN_STATE:
			if (msg_received.input == RIGHT_CLOSE) {
				state = ENTER_RIGHT_CLOSE_STATE;
				sendMsg = 1;
			}
			break;
		case ENTER_RIGHT_CLOSE_STATE:
			if (msg_received.input == GUARD_RIGHT_LOCK) {
				state = READY_STATE;
				sendMsg = 1;
			}
			break;
		case LEAVE_RIGHT_UNLOCK_STATE:
			if (msg_received.input == RIGHT_OPEN) {
				state = LEAVE_RIGHT_OPEN_STATE;
				sendMsg = 1;
			}
			break;
		case LEAVE_RIGHT_OPEN_STATE:
			if (msg_received.input == WEIGHT_SCAN) {
				state = LEAVE_WEIGHTSCAN_STATE;
				sendMsg = 1;
			}
			break;
		case LEAVE_WEIGHTSCAN_STATE:
			if (msg_received.input == RIGHT_CLOSE) {
				state = LEAVE_RIGHT_CLOSE_STATE;
				sendMsg = 1;
			}
			break;
		case LEAVE_RIGHT_CLOSE_STATE:
			if (msg_received.input == GUARD_RIGHT_LOCK) {
				state = LEAVE_RIGHTLOCK_STATE;
				sendMsg = 1;
			}
			break;
		case LEAVE_RIGHTLOCK_STATE:
			if (msg_received.input == GUARD_LEFT_UNLOCK) {
				state = LEAVE_LEFT_UNLOCK_STATE;
				sendMsg = 1;
			}
			break;
		case LEAVE_LEFT_UNLOCK_STATE:
			if (msg_received.input == LEFT_OPEN) {
				state = LEAVE_LEFT_OPEN_STATE;
				sendMsg = 1;
			}
			break;
		case LEAVE_LEFT_OPEN_STATE:
			if (msg_received.input == LEFT_CLOSE) {
				state = LEAVE_LEFT_CLOSE_STATE;
				sendMsg = 1;
			}
			break;
		case LEAVE_LEFT_CLOSE_STATE:
			if (msg_received.input == GUARD_LEFT_LOCK) {
				state = READY_STATE;
				sendMsg = 1;
			}
			break;
		case EXIT_STATE:
			MsgSend(coid, &msg_received, sizeof(send_t), &response,
					sizeof(response_t));
			MsgReply(rcvid, EOK, &response, sizeof(response_t));
			name_close(coid);
			name_detach(attach, 0);
			return EXIT_SUCCESS;
		}

		if (sendMsg == 1) {
			MsgSend(coid, &msg_received, sizeof(send_t), &response,
					sizeof(response_t));
		}

		MsgReply(rcvid, EOK, &response, sizeof(response_t));

		if (state == EXIT_STATE) {
			name_close(coid);
			name_detach(attach, 0);
			return EXIT_SUCCESS;
		}
	}

}
