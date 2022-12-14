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
#include "../../des_controller/src/des.h"

send_t msg_received;
response_t response;
name_attach_t *attach;
int rcvid;


int main(void) {


   if ((attach = name_attach(NULL, NAME_ATTACH_DISPLAY, 0)) == NULL) {
	   return EXIT_FAILURE;
   }

	printf("Display's PID: %d\n", getpid());

	while (1) {
//		Call MsgReceive() to receive Display object from controller
		if ((rcvid = MsgReceive(attach->chid, &msg_received, sizeof(send_t), NULL))
				== -1) {
			printf("Error receiving message from controller.");
			return EXIT_FAILURE;
		}

//		Call MsgReply(), sending EOK back to the controller
		if (MsgReply(rcvid, EOK, &response, sizeof(response_t)) == -1) {
			printf("Unable to reply to message.");
			return EXIT_FAILURE;
		}

//		IF message == ID_SCAN THEN
		if (msg_received.input == LEFT_SCAN
				|| msg_received.input == RIGHT_SCAN) {
			printf("%s %d\n", outMessage[SCAN_MSG], msg_received.person_id);
		} else if (msg_received.input == WEIGHT_SCAN) { //ELSE IF message = WEIGHED THEN
			//Print person has been weighed and their weight
			printf("Person ID: %d has been weighed: %.2f.\n",
					msg_received.person_id, msg_received.weight);
		} else if (msg_received.input == GUARD_LEFT_UNLOCK) {
			printf("%s\n", outMessage[LEFT_UNLOCK_MSG]);
		} else if (msg_received.input == GUARD_RIGHT_UNLOCK) {
			printf("%s\n", outMessage[RIGHT_UNLOCK_MSG]);
		} else if (msg_received.input == LEFT_OPEN) {
			printf("%s\n", outMessage[LEFT_OPEN_MSG]);
		} else if (msg_received.input == RIGHT_OPEN) {
			printf("%s\n", outMessage[RIGHT_OPEN_MSG]);
		} else if (msg_received.input == LEFT_CLOSE) {
			printf("%s\n", outMessage[LEFT_CLOSE_MSG]);
		} else if (msg_received.input == RIGHT_CLOSE) {
			printf("%s\n", outMessage[RIGHT_CLOSE_MSG]);
		} else if (msg_received.input == GUARD_LEFT_LOCK) {
			printf("%s\n", outMessage[LEFT_LOCK_MSG]);
		} else if (msg_received.input == GUARD_RIGHT_LOCK) {
			printf("%s\n", outMessage[RIGHT_LOCK_MSG]);
		} else {
			//		Print message to Neutrino console (i.e. stdout)
			printf("%s\n", outMessage[msg_received.input]);
		}

		if (msg_received.input == EX_INPUT) { //IF message == EXIT THEN
			break;
		}

	}

	name_detach(attach, 0);
	return EXIT_SUCCESS;

}
