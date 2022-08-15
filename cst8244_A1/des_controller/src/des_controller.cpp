#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <errno.h>
#include "des.h"


int main (int argc, char* argv[]) {
	pid_t displayPid = atoi(argv[1]);
	int rcvid;
	int rpyid;// indicates who we should reply to
    int controllerCID;          // the channel ID
    int connectToDisplay;
    char msgRvs[256];
    int msgRply = 0;
    int rplyForDisplay;
    char sndMsg[256];
    int LDC = 1; /* Left Door Close */
    int LDL = 1; /* Left Door Lock */
    int RDC = 1; /* Right Door Close */
    int RDL = 1; /* Right Door Lock */
    /* Creating a channel for Inputs to connect */
    controllerCID = ChannelCreate (0);
    if (controllerCID == -1) {
    	perror("\nFailed to create the channel. Exiting. ");
    	exit (EXIT_FAILURE);
    }
    /* Connecting to channel created by Display */
    connectToDisplay = ConnectAttach (0, displayPid, 1, 0, 0);
	if (connectToDisplay == -1) {
		printf("\nCannot Connect to Display. Exiting ");
		return EXIT_FAILURE;
	}
	/* Printing PID of current process */
    printf("The controller is running as PID: %d\n", getpid());
    // this is typical of a server:  it runs forever
    int actionState;
	char tempBadgeID[128];
	char tempWeight[128];
    while (1) {
        // get the message, and print it
    	actionState = -1;

        rcvid = MsgReceive (controllerCID, &msgRvs, sizeof (msgRvs), NULL);
        if(rcvid == -1){
        	printf("\nError receiving message from Inputs. Exiting. \n");
        	return EXIT_FAILURE;
        }

        if(strcmp(msgRvs, "exit") == 0) {
        	strcpy(sndMsg, "exit");
        	int exitTemp = MsgSend (connectToDisplay, &sndMsg, sizeof (sndMsg), &rplyForDisplay, sizeof (rplyForDisplay));;
			exit (EXIT_FAILURE);
        }
        /* If event received is Left Scan */
        if(strcmp(msgRvs, "ls") == 0) {
        	actionState = -1;
        	if(!((LDC == 1) && (LDL == 1) && (RDC == 1) && (RDL == 1))){
        		actionState = LEFT_SCAN;
        		strcpy(sndMsg,"\n Someone else is trying to use the gate. Please wait your turn. \n");
        	}
        	if((LDC == 1) && (LDL == 1) && (RDC == 1) && (RDL == 1)){

        		scanf(" %s", tempBadgeID);
        		strcpy(sndMsg, "\nPerson scanned ID, ID is = ");
        		strcat(sndMsg, tempBadgeID);

        	}
        }
        /* If event received is Right Scan */
        if(strcmp(msgRvs, "rs") == 0) {
        	actionState = -1;
        	if(!((LDC == 1) && (LDL == 1) && (RDC == 1) && (RDL == 1))){
        		actionState = RIGHT_SCAN;
        		strcpy(sndMsg,"\n Someone else is trying to use the gate. Please wait your turn. \n");
        	}
        	else if((LDC == 1) && (LDL == 1) && (RDC == 1) && (RDL == 1)){

        		scanf(" %s", tempBadgeID);
        		strcpy(sndMsg, "\nPerson scanned ID, ID is = ");
        		strcat(sndMsg, tempBadgeID);

        	}
        }
        /* If event received is Right Scan */
        if(strcmp(msgRvs, "ws") == 0) {
        	actionState = -1;
        	if((LDC == 0) || (RDC == 0)){

        		scanf(" %s", tempWeight);
        		strcpy(sndMsg, "\nPerson weighed, Weight = ");
        		strcat(sndMsg, tempWeight);

        	}
        	else if ((LDC == 1) && (RDC == 1)){
        		actionState = -WEIGHT_SCALE;
        		strcpy(sndMsg, "\nCannot weight person. Door is closed.\n");
        	}
        }
        /* If event received is Left Open */
        if(strcmp(msgRvs, "lo") == 0) {
        	actionState = LEFT_OPEN;
        	if(LDL == 1){
        		strcpy(sndMsg,"\n Cannot open Left-door. Door is Locked \n");
        	}
        	else if (LDL == 0){
        		LDC = 0;
        		strcpy(sndMsg,outDisplayMessage[LEFT_OPEN]);
        	}
        }
        /* If event received is Right Open */
        if(strcmp(msgRvs, "ro") == 0) {
        	actionState = RIGHT_OPEN;
        	if(RDL == 1){
        		strcpy(sndMsg,"\n Cannot open Right-door. Door is Locked \n");
        	}
        	else if (RDL == 0){
        		RDC = 0;
        		strcpy(sndMsg,outDisplayMessage[RIGHT_OPEN]);
        	}
        }
        /* If event received is Left Closed */
        if(strcmp(msgRvs, "lc") == 0) {
        	actionState = LEFT_CLOSED;
        	if(LDC == 0){
        		LDC = 1;
        		strcpy(sndMsg,outDisplayMessage[LEFT_CLOSED]);
        	}
        	else if(LDC == 1){
        		strcpy(sndMsg,"\n Left-Door is already closed. \n");
        	}
        }
        /* If event received is Right Closed */
        if(strcmp(msgRvs, "rc") == 0) {
        	actionState = RIGHT_CLOSED;
        	if(RDC == 0){
        		RDC = 1;
        		strcpy(sndMsg,outDisplayMessage[RIGHT_CLOSED]);
        	}
        	else if(RDC == 1){
        		strcpy(sndMsg,"\n Right-Door is already closed. \n");
        	}
        }
        /* If event received is Guard Right Unlock*/
        if(strcmp(msgRvs, "gru") == 0) {
        	actionState = GUARD_RIGHT_UNLOCK;
        	if(RDL == 0){
        		strcpy(sndMsg,"\n Right-Door is already unlocked. \n");
        	}
        	else if(RDL == 1){
        		RDL = 0;
        		strcpy(sndMsg,outDisplayMessage[GUARD_RIGHT_UNLOCK]);
        	}
        }
        /* If event received is Guard Right Lock*/
        if(strcmp(msgRvs, "grl") == 0) {
        	actionState = GUARD_RIGHT_LOCK;
        	if(RDL == 1){
        		strcpy(sndMsg,"\n Right-Door is already locked. \n");
        	}
        	else if(RDL == 0){
        		RDL = 1;
        		strcpy(sndMsg,outDisplayMessage[GUARD_RIGHT_LOCK]);
        	}
        }
        /* If event received is Guard Left Lock*/
        if(strcmp(msgRvs, "gll") == 0) {
        	actionState = GUARD_LEFT_LOCK;
        	if(LDL == 1){
        		strcpy(sndMsg,"\n Left-Door is already locked. \n");
        	}
        	else if (LDL == 0){
        		LDL = 1;
        		strcpy(sndMsg,outDisplayMessage[GUARD_LEFT_LOCK]);
        	}
        }
        /* If event received is Guard Left Unlock*/
        if(strcmp(msgRvs, "glu") == 0) {
        	actionState = GUARD_LEFT_UNLOCK;
        	if(LDL == 0){
        		strcpy(sndMsg,"\n Left-Door is already unlocked. \n");
        	}
        	else if (LDL == 1){
        		LDL = 0;
        		strcpy(sndMsg,outDisplayMessage[GUARD_LEFT_UNLOCK]);
        	}
        }

        if(actionState > -1){
        	int tempSnd = MsgSend (connectToDisplay, &sndMsg, sizeof (sndMsg), &rplyForDisplay, sizeof (rplyForDisplay));;
        	if (tempSnd == -1) {
        		fprintf (stderr, "\nError during MsgSend. Exiting. \n");
        	    perror (NULL);
        	    exit (EXIT_FAILURE);
        	}
        	if (rplyForDisplay == -1) {
        		printf("\nError receiving reply from Display\n");
        	}
        }
        /* Reply back to Input for Event Input again */
        rpyid = MsgReply(rcvid, 1, &msgRply, sizeof(msgRply));
        if(rpyid == -1){
        	printf("\nError replying to Inputs. Exiting. \n");
            return EXIT_FAILURE;
        }
    }

    // destroy the channel when done
    ChannelDestroy(controllerCID);
   return EXIT_SUCCESS;
}
