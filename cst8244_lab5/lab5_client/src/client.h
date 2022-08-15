/*
 * client.h
 *
 *  Created on: Jun 23, 2022
 *      Author: bisi0005
 */

#ifndef CLIENT_H_
#define CLIENT_H_


struct client_send {
	int  left_hand;
	char operator;
	int  right_hand;
} typedef client_send_t;

#define SRVR_OK               0
#define SRVR_UNDEFINED        1
#define SRVR_INVALID_OPERATOR 2
#define SRVR_OVERFLOW         3

struct server_response {
	double answer;
	int    error; // [OK, UNDEFINED, INVALID_OPERATOR, OVERFLOW]
	char   errorMsg[128];
} typedef server_response_t;


#endif /* CLIENT_H_ */
