#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <errno.h>

#include "des.h"

int main (int argc, char* argv[]) {

	char msg [200];
	int displayCID;
	int rplyMsg = 0;
	printf("The display is running as PID: %d\n", getpid());
	//pid_t serverpid = atoi(argv[1]);
	displayCID = ChannelCreate(0);

	/* Continuous while loop for receiving message from controller
	 * and displaying output messages*/
	while (1) {
		int rcid = MsgReceive(displayCID, &msg, sizeof(msg), NULL);
		if (rcid == -1) {
			printf("\nError receiving message from Controller \n");
			return EXIT_FAILURE;
		}
		if(strcmp(msg, "exit") == 0) {
			return EXIT_FAILURE;
		}
		printf("%s \n", msg);

		int rpid = MsgReply(rcid, 1, &rplyMsg, sizeof(rplyMsg));
		if (rpid == -1) {
			printf("\nError replying to Controller \n");
			return EXIT_FAILURE;
		}
	}
	/* Destroy the channel when done */
	 ChannelDestroy(displayCID);
   return EXIT_SUCCESS;
}
