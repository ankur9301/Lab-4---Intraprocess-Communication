#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>  // For close()

#include "list.h"  // Assuming this contains the linked list functions

#define PORT 9001
#define ACK "ACK"

int main(int argc, char const* argv[]) {
    int n, val, idx;  // Declare variables for value and index
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    char buf[1024];
    char sbuf[1024];
    char* token;

    struct sockaddr_in servAddr;
    list_t *mylist = list_alloc();  // Allocate the linked list

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(servSockD, 1) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Waiting for a client to connect...\n");
    int clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket < 0) {
        perror("Client connection failed");
        exit(1);
    }
    printf("Client connected!\n");

    while (1) {
        memset(buf, 0, sizeof(buf));  // Clear buf at the start of each loop

        n = recv(clientSocket, buf, sizeof(buf), 0);
        buf[n] = '\0';

        if (n > 0) {
            token = strtok(buf, " ");
            memset(sbuf, 0, sizeof(sbuf));  // Clear sbuf before each response

            if (strcmp(token, "exit") == 0) {
                list_free(mylist);
                printf("Server exiting...\n");
                close(clientSocket);
                close(servSockD);
                exit(0);
            } 
            else if (strcmp(token, "print") == 0) {
                snprintf(sbuf, sizeof(sbuf), "%s", listToString(mylist));
            } 
            else if (strcmp(token, "get_length") == 0) {
                val = list_length(mylist);
                snprintf(sbuf, sizeof(sbuf), "Length = %d", val);
            } 
            else if (strcmp(token, "add_back") == 0) {
                token = strtok(NULL, " ");
                val = atoi(token);
                list_add_to_back(mylist, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            } 
            else if (strcmp(token, "add_front") == 0) {
                token = strtok(NULL, " ");
                val = atoi(token);
                list_add_to_front(mylist, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            } 
            else if (strcmp(token, "add_position") == 0) {
                token = strtok(NULL, " ");
                idx = atoi(token);
                token = strtok(NULL, " ");
                val = atoi(token);
                list_add_at_index(mylist, idx, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d at %d", ACK, val, idx);
            } 
            else if (strcmp(token, "remove_back") == 0) {
                val = list_remove_from_back(mylist);
                snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
            } 
            else if (strcmp(token, "remove_front") == 0) {
                val = list_remove_from_front(mylist);
                snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
            } 
            else if (strcmp(token, "remove_position") == 0) {
                token = strtok(NULL, " ");
                idx = atoi(token);
                val = list_remove_at_index(mylist, idx);
                snprintf(sbuf, sizeof(sbuf), "Removed = %d from %d", val, idx);
            } 
            else if (strcmp(token, "get") == 0) {
                token = strtok(NULL, " ");
                idx = atoi(token);
                val = list_get_elem_at(mylist, idx);
                snprintf(sbuf, sizeof(sbuf), "Element at %d = %d", idx, val);
            } 
            else {
                snprintf(sbuf, sizeof(sbuf), "Unknown command");
            }

            send(clientSocket, sbuf, strlen(sbuf) + 1, 0);  // Send with null terminator
        }
    }

    return 0;
}







// #include <netinet/in.h> //structure for storing address information 
// #include <stdio.h> 
// #include <string.h>
// #include <stdlib.h> 
// #include <sys/socket.h> //for socket APIs 
// #include <sys/types.h> 
// #include "list.h"

// #define PORT 9001
// #define ACK "ACK"
  
// int main(int argc, char const* argv[]) 
// { 
  
// 	  int n, val, idx;
//     // create server socket similar to what was done in 
//     // client program 
//     int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
  
//     // string store data to recv/send to/from client 
//     char buf[1024];
// 		char sbuf[1024];
//     char* token;

//     // define server address 
//     struct sockaddr_in servAddr; 
  
// 	  // list
// 		list_t *mylist;


//     servAddr.sin_family = AF_INET; 
//     servAddr.sin_port = htons(PORT); 
//     servAddr.sin_addr.s_addr = INADDR_ANY; 
  
//     // bind socket to the specified IP and port 
//     bind(servSockD, (struct sockaddr*)&servAddr, 
//          sizeof(servAddr)); 
  
//     // listen for connections 
//     listen(servSockD, 1); 
  
//     // integer to hold client socket. 
//     int clientSocket = accept(servSockD, NULL, NULL); 

//     mylist = list_alloc();  // create the list

//     while(1){
//         // recvs messages from client socket 
//         n = recv(clientSocket, buf, sizeof(buf), 0);
// 				buf[n] = '\0';

//         if(n > 0){
//             token = strtok(buf, " ");
    
// 						if(strcmp(token,"exit") == 0){
// 							  list_free(mylist);
// 							  exit(1);
// 						}
// 						else if(strcmp(token,"get_length") == 0){
// 								// Make a Call
// 								val = list_length(mylist);
// 								sprintf(sbuf,"%s%d", "Length = ", val);
// 						}
// 						else if(strcmp(token,"add_front") == 0){
// 							  token = strtok(NULL, " ");  // get next token (value)
// 								val = atoi(token);
// 								// Make a Call
// 								sprintf(sbuf,"%s%d", ACK, val);
// 								list_add_to_front(mylist,val);
// 						}
// 						else if(strcmp(token,"remove_position") == 0){
// 							  token = strtok(NULL, " ");
// 								idx = atoi(token);
// 								// Make a Call
// 								val = list_remove_at_index(mylist,idx);
// 								sprintf(sbuf,"%s%d", ACK, val);
// 						}
// 						else if(strcmp(token,"print") == 0){
// 							  sprintf(sbuf,"%s", listToString(mylist));
// 						}
// 						// ADD THE OTHER OPERATIONS


//                 // send's messages to client socket 
//             send(clientSocket, sbuf, sizeof(sbuf), 0);
//         }
// 				memset(buf, '\0', 1024);
//     }
  
//     return 0; 
// }
