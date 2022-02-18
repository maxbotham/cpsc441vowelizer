#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

// maximum buffer size
#define MAX 1024
// port of server
#define PORT 8080
// ip addr of server
#define IP_ADDR "127.0.0.1"
#define SA struct sockaddr
using namespace std;

// This function sets up the UDP client and connects it to the server.
int udpClient(char userChoice, char *vowels)
{
    int MSG_CONFIRM = 0;
    int sockfd;
    // buffer to store strings
    char buffer[MAX];
    char *empty = "";
    // servaddr to set up connection to the server
    struct sockaddr_in servaddr;

    // Create socket and check for error
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("The creation of the UDP client socket failed.");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Add server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;

    // If we are receiving vowels
    if (userChoice == '1' || userChoice == '3')
    {
        // for some reason, this empty 'send' statement seems to be necessary for this socket
        // to be able to receive a response from the UDP server.
        sendto(sockfd, (const char *)empty, strlen(empty),
               MSG_CONFIRM, (const struct sockaddr *)&servaddr,
               sizeof(servaddr));
        // receive the vowels from the server
        n = recvfrom(sockfd, (char *)buffer, MAX,
                     MSG_WAITALL, (struct sockaddr *)&servaddr,
                     (socklen_t *)&len);
        buffer[n] = '\0';
        printf("Vowels received through UDP:\n");
        // print the vowels received
        for (int i = 0; buffer[i] != '\n'; i++)
        {
            cout << buffer[i];
        }
        cout << endl;
    }

    // If we are sending vowels
    else if (userChoice == '2' || userChoice == '4')
    {
        // send the vowels to the server
        sendto(sockfd, (const char *)vowels, strlen(vowels),
               MSG_CONFIRM, (const struct sockaddr *)&servaddr,
               sizeof(servaddr));
    }
    // close the socket
    close(sockfd);
    return 0;
}

// This is the main while loop that executes to continue persistent TCP communication to server
int communicationLoop(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        // User guide / options
        cout << "Please choose one of the following options by entering the corresponding number in your terminal:" << endl;
        cout << "1: Devowel a message with simple encoding" << endl;
        cout << "2: Envowel a message with simple encoding" << endl;
        cout << "3: Devowel a message with advanced encoding" << endl;
        cout << "4: Envowel a message with advanced encoding" << endl;
        cout << "5: Exit the program" << endl;
        string userChoice;
        getline(cin, userChoice); // read input
        // If the input is invalid, get user to try again
        if (userChoice.at(0) != '1' && userChoice.at(0) != '2' && userChoice.at(0) != '3' &&
            userChoice.at(0) != '4' && userChoice.at(0) != '5' && userChoice.length() == 1)
        {
            cout << "\nInvalid input. Please try again.\n\n"
                 << endl;
        }
        // Kill the client
        else if (userChoice.at(0) == '5')
        {
            cout << "Terminating the client." << endl;
            exit(1);
        }
        else
        {
            bzero(buff, sizeof(buff));
            buff[0] = userChoice.at(0);
            // write the user choice to server (now server knows what its job is)
            write(sockfd, buff, sizeof(buff));
            // Devowel a message
            if (userChoice.at(0) == '1' || userChoice.at(0) == '3')
            {
                cout << "\nPlease enter the message you would like to devowel: " << endl;
                bzero(buff, sizeof(buff));
                int j = 0;
                while ((buff[j] = getchar()) != '\n')
                {
                    j++;
                }
                // write the message to the server
                write(sockfd, buff, sizeof(buff));
                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff)); // read the non-vowel response
                udpClient(userChoice.at(0), "");  // use udpClient function to read/print vowels
                printf("Other characters received through TCP:\n");
                // print non-vowels
                for (int i = 0; buff[i] != '\n'; i++)
                {
                    cout << buff[i];
                }
                cout << endl;
            }
            // Envowel a message
            else if (userChoice.at(0) == '2' || userChoice.at(0) == '4')
            {
                cout << "\nPlease enter the vowels of the message you would like to envowel: " << endl;
                bzero(buff, sizeof(buff));
                int j = 0;
                // read vowels user inpiut
                while ((buff[j] = getchar()) != '\n')
                {
                    j++;
                }
                // send vowels from udpClient to server
                udpClient(userChoice.at(0), buff);
                cout << "\nPlease enter the other characters of the message you would like to envowel: " << endl;
                bzero(buff, sizeof(buff));
                j = 0;
                // read other chars user input
                while ((buff[j] = getchar()) != '\n')
                {
                    j++;
                }
                // send other chars over TCP to server
                write(sockfd, buff, sizeof(buff));
                bzero(buff, sizeof(buff));
                // read TCP envoweled server response
                read(sockfd, buff, sizeof(buff));
                // print respones
                printf("Here is the envoweled message, received through TCP from the server: \n");
                for (int i = 0; buff[i] != '\n'; i++)
                {
                    cout << buff[i];
                }
                cout << endl;
            }
        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("The creation of the TCP client socket failed.\n");
        exit(0);
    }
    else
    {
        printf("The socket was successfully created.\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // Add server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDR);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("Connection with the TCP server failed...\n");
        exit(0);
    }
    else
    {
        printf("You have successfully connected to the server.\n");
    }
    // Infinite loop
    communicationLoop(sockfd);
    // close the socket
    close(sockfd);
}
