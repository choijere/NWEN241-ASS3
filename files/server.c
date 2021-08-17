/**
 * Source file for Assignment 3
 * Full Name:	Jeremiah Choi
 * Student ID:	300474835
 *
 * You will need to modify this file to perform Tasks 4-5. See the
 * handout for more details.
 *
 * You may also use this as base code to perform Task 6. See the 
 * handout for more details.
 */


#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 


/**
 * Header file contains macros and user-defined types
 * needed in this program. Open server.h for more
 * details.
 */
#include "server.h"


/**
 * Load CSV database file. The implementation of this
 * function is provided so you do not need to implement
 * this.
 * 
 * @param fn CSV database filename
 * @param m Pointer to movie_t
 * @param mlen Number of elements in m
 * @return 0 if successful, -1 otherwise
 */ 
int db_loadfile(const char *fn, movie_t *m, int mlen);


/**
 * If you are implementing your own functions, declare
 * the function prototypes below. Document your function
 * prototypes using the format above in the prototype
 * declaration of db_loadfile.
 */



/**
 * Main function
 */
int main(void)
{
    int ret, sockfd;
    movie_t movies[CSVDB_RECORDS];
    struct sockaddr_in address; 
    
    printf("Starting server...\n");
    
    /* Load CSV database file */
    ret = db_loadfile(CSVDB_FILENAME, movies, CSVDB_RECORDS);
    if(ret) 
    {
        printf("Error: Failed to open %s\n", CSVDB_FILENAME);
        return 0;
    }
    
    /**
     * Write your code below to implement Task 4
     */
    //INITIATE THE REQUIRED SOCKET OPERATIONS TO CREATE AND BIND A SOCKET
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sockfd < 0)
    {
		printf("Failure creating server socket");
		exit(0);
	}
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(12345); //port, change to 12345 before you submit.
    
    if(bind(server_sockfd, (struct sockaddr *)&addr, sizeof(addr))<0)
    {
		printf("Error binding socket");
		exit(0);
	}
	
	while(1)
	{
		//LISTEN FOR CLIENTS AT TCP PORT 12345 (change when testing)
		if(listen(server_sockfd, SOMAXCONN) < 0)
		{
			printf("Error listening for connections");
			exit(0);
		}
		
		//SEND MESSAGE "HELLO" UPON SUCCESS CONNECTION
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);
		
		int client_fd = accept(server_sockfd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
		if(client_fd < 0)
		{
			printf("Error accepting connection");
			exit(0);
		}
		char msg[] = "HELLO\n\0";
		ssize_t r = send(client_fd, msg, strlen(msg), 0);
		if(r < 0)
		{
			printf("Error sending message");
			close(client_fd);
			exit(0);
		}
		//WAIT FOR A MESSAGE FROM CLIENT
    
		/**
		 * Write your code below to implement Task 5
		 */    
		while(1)
		{
			char incoming[100];
			ssize_t i = recv(client_fd, incoming, 100, 0);
			if(i <= 0)
			{
				printf("Error receiving message");
				close(client_fd);
				exit(0);
			}
			//Do stuff with reciving message:
			fflush(stdin);
			char readPrefix[4];
			strncpy(readPrefix, incoming, 3);
			printf("Received message: %s", incoming);
			readPrefix[4] = '\0';
			
			if(strcasecmp(readPrefix, "get") == 0)
			{
				char rowArgument[10];
				strncpy(rowArgument, incoming + 4, 10);
				
				int targetRow = 0;
				//convert ASCII char to actual integers
				for(i=0;i<strlen(rowArgument);i++)
				{
					char c = rowArgument[i];
					if(c >= '0' && c <= '9')
					{
						int digit = c - '0';
						targetRow = targetRow*10 + digit; 
					}
				}
				targetRow--;
				if(targetRow < sizeof(movies)/sizeof(movie_t) && targetRow >= 0)
				{
					char sendMessage[100];
					strcpy(sendMessage, movies[targetRow].title);
					strcat(sendMessage, ", ");
					strcat(sendMessage, movies[targetRow].director); 
					strcat(sendMessage, ", ");
					char temp[12];
					sprintf(temp, "%hi", movies[targetRow].year);
					strcat(sendMessage, temp);
					strcat(sendMessage, ", ");
					sprintf(temp, "%hi", movies[targetRow].oscars_won);
					strcat(sendMessage, temp);
					strcat(sendMessage, ", ");
					strcat(sendMessage, movies[targetRow].origin_country);
					strcat(sendMessage, "\n");
					ssize_t a = send(client_fd, sendMessage, strlen(sendMessage), 0);
					if(a < 0)
					{
						printf("Error sending message");
					}
				}
				else
				{
					char sendMessage[] = "Such row does not exist!\n";
					ssize_t a = send(client_fd, sendMessage, strlen(sendMessage), 0);
					if(a < 0)
					{
						printf("Error sending message");
					}
				}
				
			}
			else if(strcasecmp(readPrefix, "bye") == 0)
			{
				printf("closing!\n");
				close(client_fd);
				break;
			}
			else
			{
				char sendMessage[] = "Command not recognized!\n";
				ssize_t a = send(client_fd, sendMessage, strlen(sendMessage), 0);
				if(a < 0)
				{
					printf("Error sending message");
				}
				printf("Command not recognized!\n");
			}
		}
	}
    

    return 0;
}

/**
 * Implementation of db_loadfile function
 */
int db_loadfile(const char *fn, movie_t *movies, int mlen)
{
    // Input file stream to read csv
    FILE *csv = fopen("scifi.csv", "r");
    if(!csv) 
    {
        return -1;
    }

    // Read csv file line by line and scan for input
    int i = 0;
    while(!feof(csv) && i < mlen) 
    {
	    fscanf
	    (
			csv, "%[^,],%[^,],%d,%d,%[^\n]%*c", 
			movies[i].title, movies[i].director, &movies[i].year, 
			&movies[i].oscars_won, movies[i].origin_country
	    );
        i++;
    }

    // Close file after reading
    fclose(csv);
    
    return 0;
}

/**
 * If you are implementing your own functions, write
 * the function implementation below.
 */

