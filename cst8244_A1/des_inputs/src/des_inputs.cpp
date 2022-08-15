#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <errno.h>

#include "des.h"
int main(int argc, char *argv[]) {
	pid_t controllerPid = atoi(argv[1]);
	char inputEvent[10];
	char badgeID [128];
	char weight [128];
	char sendMsg [128];
	int rmsg;
	int cidToController = ConnectAttach (0, controllerPid, 1, 0, 0);
	if (cidToController == -1) {
			printf("Cannot Connect to Server. Please try again\n");
			return EXIT_FAILURE;
		}
	while (1) {
			/* Display all the options - INPUT EVENT */
			printf("\nEnter the event type (ls= left scan, rs= right scan, ws= weight scale, lo =left open, ro=right open, lc = left closed, rc = right closed , gru = guard right unlock, grl = guardright lock, gll=guard left lock, glu = guard left unlock, exit = exit programs)\n");
			if(scanf(" %s", inputEvent) == -1){
				printf("\nError on scanning Input-Event. \n");
				return 0;
			}
			/* If exit is entered */
			if(strcmp(inputEvent, "exit") == 0) {
				printf("Exiting Controller.\n");
				printf("Exiting Display.\n");
				strcpy(sendMsg, "exit");
				int exitTemp = MsgSend (cidToController, &sendMsg, sizeof (sendMsg), &rmsg, sizeof (rmsg));
				exit (EXIT_FAILURE);
			}
			/* If one of the door is scanned, ask for bageID number */
			if(strcmp(inputEvent, "ls") == 0 || strcmp(inputEvent, "rs") == 0) {
				printf("\nEnter the Person's ID: \n");
				scanf(" %s", badgeID);
				printf("\nPerson scanned ID, ID is = %s", badgeID);
			}
			/* If ws then prompt for weight */
			if(strcmp(inputEvent, "ws") == 0) {
				printf("\nEnter the Person's weight: \n");
				scanf(" %s", weight);
				printf("\nPerson weighed, Weight = %s", weight);
			}
			/* Send event to Controller */
			if (MsgSend (cidToController, &inputEvent, sizeof (inputEvent)+ 1, &rmsg, sizeof (rmsg)) == -1) {
					fprintf (stderr, "Error during MsgSend\n");
					perror (NULL);
					exit (EXIT_FAILURE);
				}
			if (rmsg == -1) {
				printf("\nError receiving reply from Controller - Input-Event");
			}
	}
	ConnectDetach(cidToController);
	return EXIT_SUCCESS;
}
