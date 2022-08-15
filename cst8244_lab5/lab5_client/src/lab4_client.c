#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include "client.h"
/**
 * Msg_Passing_Client.c
 *
 * Usage: ./Msg_Passing_Client PID_of_Server
 *
 * "" "" server pid \int \op \int
 */
int main (int argc, char* argv[])
{
	client_send_t smsg;
	//server_response_t resMsg;
	char rmsg [200];
	int  coid;
	//
	pid_t serverpid = atoi(argv[1]);
	// 2nd Command-Line Argument - Left Hand
	smsg.left_hand = atoi (argv[2]); //trunc if >32bits
	// 3rd Command-Line Argument - Operator
	if(atol(argv[2] !=atoi(argv[2]))){
		puts("Overflow @client");
	}
	char *op; // use 2d arr
	op = argv[3];
	switch(*op) {
		case '+' :
			smsg.operator = '+';
			break;
		case '-':
			smsg.operator = '-';
			break;
		case 'x':
			smsg.operator = 'x';
			break;
		case '/':
			smsg.operator = '/';
			break;

	}
	/* 4th Command-Line Argument - Right Hand */
	smsg.right_hand = atoi (argv[4]);// if alot of args
	if(argc > 5){
		puts('ERR alot of args');
	}
	// establish a connection to the backend
	coid = ConnectAttach (ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		fprintf (stderr, "Couldn't ConnectAttach\n");// fail/wrong arg
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	// send the message
	if (MsgSend (coid, &smsg, sizeof (smsg)+ 1, rmsg, sizeof (rmsg)) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	if((smsg.right_hand == 0) && (smsg.left_hand != 0) && (smsg.operator == '/')){
		printf("\nCannot divide a non-zero number by 0\n");
		exit(-1);
	}
	if((smsg.right_hand == 0) && (smsg.left_hand == 0) && (smsg.operator == '/')){
		printf("\nThe server has calculated the result of 0 / 0 as 0.00\n");
		exit(0);
	}
	if (strlen (rmsg) > 0) {
		printf("The server has calculated the result of %d %c %d ",smsg.left_hand, smsg.operator, smsg.right_hand);
		printf("as %s\n", rmsg);
	}

	//Disconnect from the channel
	ConnectDetach(coid);
   return EXIT_SUCCESS;
}
