


///
/// controller.c
///
/// author bisi0005@algonquin.com

#include <stdlib.h>
#include <stdio.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define MY_PULSE_CODE _PULSE_CODE_MINAVAIL

typedef union {
	struct _pulse pulse;
	char msg[255];
} my_message_t;

int main(void) {
	my_message_t message;
	name_attach_t *attach;
	FILE *file;
	int rcvid;
	int const flags = 0;
	char status[10];

	if ((attach = name_attach(NULL, "mydevice", flags)) == NULL) {
		fprintf(stderr, "ERROR: Could not attach connection to 'mydevice'\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	if ((file = fopen("/dev/local/mydevice", "r")) == NULL) {
		fprintf(stderr, "ERROR: Could not open the file\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	fscanf(file, "%s %s", status, message.msg);

	if (strcmp(status, "status") == 0) {
		printf("Status: %s\n", message.msg);
		fclose(file);
	}

	if (strcmp(message.msg, "closed") == 0) {
		name_detach(attach, flags);
		fclose(file);
		exit(EXIT_SUCCESS);
	}

	while(1) {
		if ((rcvid = MsgReceivePulse(attach->chid, &message.msg, sizeof(message.msg), NULL)) == 0) {
			if (message.pulse.code == MY_PULSE_CODE) {
				printf("Small Integer: %d\n", message.pulse.value.sival_int);

				if ((file = fopen("/dev/local/mydevice", "r")) == NULL) {
					fprintf(stderr, "ERROR: Could not open the file\n");
					perror(NULL);
					exit(EXIT_FAILURE);
				}

				fscanf(file, "%s %s", status, message.msg);

				if (strcmp(status, "status") == 0) {
					printf("Status: %s\n", message.msg);
					fclose(file);
				}

				if (strcmp(message.msg, "closed") == 0) {
					name_detach(attach, flags);
					fclose(file);
					exit(EXIT_SUCCESS);
				}
			}
			else printf("ERROR: Could not receive pulse.\n");
		}
		else {
			printf("ERROR: Could not receive message.\n");
			perror(NULL);
			exit(EXIT_FAILURE);
		}
	}

	name_detach(attach, flags);
	exit(EXIT_SUCCESS);
}
