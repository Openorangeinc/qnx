#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include "server.h"

/**
 * server
 * receives request from client (channeled)
 * provides pid to client
 */
int main (int argc, char* argv[]) {
    int     rcvid;         // indicates who we should reply to
    int     chid;          // the channel ID
    client_send_t msg; // big enough for our purposes
    server_response_t resMsg;
    char message [50];

    // create a channel
    chid = ChannelCreate (0);
    if (chid == -1)
    {
    	perror("failed to create the channel.");
    	exit (EXIT_FAILURE);
    }
    printf("Server PID is %d\n", getpid());
    // this is typical of a server:  it runs forever
    while (1) {
        // get the message, and print it
        rcvid = MsgReceive (chid, &msg, sizeof (msg), NULL);
        printf("Server PID is %d\n", getpid());
        double tempLeft = msg.left_hand;
        double tempRight = msg.right_hand;
        switch(msg.operator) {
        		case '+' :
        			resMsg.answer = (tempLeft + tempRight);
        			break;
        		case '-':
        			resMsg.answer = (tempLeft - tempRight);
        			break;
        		case 'x':
        			resMsg.answer = (tempLeft * tempRight);
        			break;
        		case '/':
        			resMsg.answer = (tempLeft / tempRight);
        			break;
        	}

        //snprint:
        snprintf(message, 50, "%.2f", resMsg.answer);


        //reply
        MsgReply (rcvid, EOK, message, sizeof (message));
    }

    // destroy the channel when done clean mem
    ChannelDestroy(chid);
   return EXIT_SUCCESS;
}
