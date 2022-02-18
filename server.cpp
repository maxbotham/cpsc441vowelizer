#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <iostream>

//max buffer size
#define MAX 1024
//port that server runs on
#define PORT 8080
#define SA struct sockaddr
using namespace std;

//udpServer function, returns a char*.
char *udpServer(char *response, char userChoice)
{
    int MSG_CONFIRM = 0;
    //udpsocket var
    int UDPsockfd;
    //buffer for user input
    char buffer[MAX];
    struct sockaddr_in UDPservaddr, UDPcliaddr;

    // Create UDP socket and check for error
    if ((UDPsockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("UDP socket server creation failed.");
        exit(EXIT_FAILURE);
    }
    memset(&UDPservaddr, 0, sizeof(UDPservaddr));
    memset(&UDPcliaddr, 0, sizeof(UDPcliaddr));

    //Add the server information
    UDPservaddr.sin_family = AF_INET; // IPv4
    UDPservaddr.sin_addr.s_addr = INADDR_ANY;
    UDPservaddr.sin_port = htons(PORT);

    //Bind the created socket with the server addr, check for error.
    if (bind(UDPsockfd, (const struct sockaddr *)&UDPservaddr,
             sizeof(UDPservaddr)) < 0)
    {
        perror("Bind to server address failed.");
        exit(EXIT_FAILURE);
    }

    int length, n;
    length = sizeof(UDPcliaddr);

    //read data from client into buffer
    n = recvfrom(UDPsockfd, (char *)buffer, MAX,
                 MSG_WAITALL, (struct sockaddr *)&UDPcliaddr,
                 (socklen_t *)&length);
    buffer[n] = '\0';
    //send data to client (from arg passed in to this function)
    sendto(UDPsockfd, (const char *)response, strlen(response),
           MSG_CONFIRM, (const struct sockaddr *)&UDPcliaddr,
           length);
    //close the socket
    close(UDPsockfd);

    //if the user wants to envowel a message, we must return the vowels they sent.
    if (userChoice == '2' || userChoice == '4')
    {
        char *returnValues = new char[MAX];
        for (int i = 0; i < n; i++)
        {
            returnValues[i] = buffer[i];
        }
        return returnValues;
    }
}

//Infinite loop to deal with persistent TCP communication with client.
int communicationLoop(int connfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, MAX);
        //read the message from client and copy it into the buffer
        read(connfd, buff, sizeof(buff));
        //if user wants to devowel with simple method
        if (buff[0] == '1')
        {
            cout << "Beginning process to devowelize with a simple method." << endl;
            bzero(buff, MAX);
            //read the message sent over TCP
            read(connfd, buff, sizeof(buff));
            int i = 0;
            char vowelLetters[MAX];
            char otherLetters[MAX];
            //formula to devowel a message into two char arrays:
            //otherLetters, and vowelLetters.
            while (buff[i] != '\n' && i < MAX)
            {
                if (buff[i] == 'a' || buff[i] == 'e' || buff[i] == 'i' || buff[i] == 'o' || buff[i] == 'u' || buff[i] == 'A' || buff[i] == 'E' || buff[i] == 'I' || buff[i] == 'O' || buff[i] == 'U')
                {
                    vowelLetters[i] = buff[i];
                    otherLetters[i] = ' ';
                }
                else
                {
                    otherLetters[i] = buff[i];
                    vowelLetters[i] = ' ';
                }
                i++;
            }
            vowelLetters[i] = '\n';
            otherLetters[i] = '\n';
            //print all vowels and other chars being sent back to client
            cout << "Vowel letters being sent back to client: " << endl;
            for (int i = 0; vowelLetters[i] != '\n'; i++)
            {
                cout << vowelLetters[i];
            }
            cout << endl;
            cout << "Other letters being sent back to client" << endl;
            for (int i = 0; otherLetters[i] != '\n'; i++)
            {
                cout << otherLetters[i];
            }
            cout << endl;
            //send other chars back to client over TCP
            write(connfd, otherLetters, sizeof(otherLetters));
            //send vowels back to client over UDP
            udpServer(vowelLetters, '1');
        }
        //if user wants to envowel char array using simple method
        else if (buff[0] == '2')
        {
            cout << "Beginning process to envowelize with a simple method." << endl;
            //read vowels from over UDP.
            char *vowels = udpServer("", '2');
            //print vowels.
            cout << "Received vowels: \n"
                 << vowels;
            bzero(buff, MAX);
            //read other chars over TCP
            read(connfd, buff, sizeof(buff));
            //print other chars
            cout << "Received other letters: \n"
                 << buff;
            char response[MAX];
            int i = 0;
            //envoweling algorithm
            for (i; vowels[i] != '\n'; i++)
            {
                if (vowels[i] != ' ')
                {
                    response[i] = vowels[i];
                }
                else
                {
                    response[i] = buff[i];
                }
            }
            response[i] = '\n';
            cout << "Sending this envowelized string back to client: " << endl;
            for (i = 0; response[i] != '\n'; i++)
            {
                cout << response[i];
            }
            cout << endl;
            //send back the envoweleed message over TCP
            write(connfd, response, sizeof(response));
        }
        //if the user wants to devowel with advanced method
        else if (buff[0] == '3')
        {
            cout << "Beginning process to devowelize with an advanced method." << endl;
            bzero(buff, MAX);
            //read the message the user wants to devowel (from client), over TCP
            read(connfd, buff, sizeof(buff));
            int i = 0;
            char vowelLetters[MAX];
            char otherLetters[MAX];
            int nonVowelCounter = 0;
            int vowelCharCounter = 0;
            int otherLettersCounter = 0;
            //algorithm to devowel the user's message
            while (buff[i] != '\n' && i < MAX)
            {
                if (buff[i] == 'a' || buff[i] == 'e' || buff[i] == 'i' || buff[i] == 'o' || buff[i] == 'u' || buff[i] == 'A' || buff[i] == 'E' || buff[i] == 'I' || buff[i] == 'O' || buff[i] == 'U')
                {
                    vowelLetters[vowelCharCounter] = (char)(nonVowelCounter + 48);
                    vowelLetters[vowelCharCounter + 1] = buff[i];
                    vowelCharCounter += 2;
                    nonVowelCounter = 0;
                }
                else
                {
                    otherLetters[otherLettersCounter] = buff[i];
                    nonVowelCounter++;
                    otherLettersCounter++;
                }
                i++;
            }
            otherLetters[otherLettersCounter] = '\n';
            otherLetters[otherLettersCounter + 1] = '\0';
            vowelLetters[vowelCharCounter] = '\n';
            vowelLetters[vowelCharCounter + 1] = '\0';
            //print the vowel chars and other chars being sent back to client
            cout << "Vowel letters being sent back to client: " << endl;
            for (int i = 0; vowelLetters[i] != '\n'; i++)
            {
                cout << vowelLetters[i];
            }
            cout << endl;
            cout << "Other letters being sent back to client" << endl;
            for (int i = 0; otherLetters[i] != '\n'; i++)
            {
                cout << otherLetters[i];
            }
            cout << endl;
            //send the other chars back to client over TCP
            write(connfd, otherLetters, sizeof(otherLetters));
            //send vowels back to client over UDP
            udpServer(vowelLetters, '1');
        }
        //if the user wants to envowel using an advanced method
        else if (buff[0] == '4')
        {
            cout << "Beginning process to envowelize with an advanced method." << endl;
            //receive vowels from the user over UDP
            char *vowels = udpServer("", '2');
            cout << "Received vowels: \n"
                 << vowels;
            bzero(buff, MAX);
            //receieve other chars from the user over TCP
            read(connfd, buff, sizeof(buff));
            cout << "Received other letters: \n"
                 << buff;
            char response[MAX];
            int nonVowelCounter = 0;
            int vowelIndex = 0;
            int otherIndex = 0;
            //algorithm to envowel the other chars with the vowels.
            for (int i = 0; i < MAX; i++)
            {
                if (vowels[vowelIndex] == (char)(nonVowelCounter + 48))
                {
                    response[i] = vowels[vowelIndex + 1];
                    vowelIndex += 2;
                    nonVowelCounter = 0;
                }
                else
                {
                    response[i] = buff[otherIndex];
                    otherIndex++;
                    nonVowelCounter++;
                }
            }
            //print the result of envowelizing.
            cout << "Sending this envowelized string back to client: " << endl;
            for (int i = 0; response[i] != '\n'; i++)
            {
                cout << response[i];
            }
            cout << endl;
            //send back the envoweled message over TCP
            write(connfd, response, sizeof(response));
        }
    }
}

// Driver function
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    //Create the socket, check for error
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("The server TCP socket creation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully created.\n");
    bzero(&servaddr, sizeof(servaddr));

    //Add server details
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Try to bind the socket to server address, check for error.
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        perror("Server socket bind to server address failed.\n");
        exit(0);
    }
    else
        printf("Server socket successfully binded.\n");

    //Server should listen for connection to client, check for error
    if ((listen(sockfd, 5)) != 0)
    {
        perror("Server listen failed.\n");
        exit(0);
    }
    else
        printf("Server listening and waiting for connections.\n");
    unsigned int len = sizeof(cli);

    //Accept the client connection, check for error.
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        perror("The server was unable to accept the client's connection.\n");
        exit(0);
    }
    else
        printf("The server accepted the client on TCP.\n");

    // infinite loop for communication with client
    communicationLoop(connfd);

    //Close the socket
    close(sockfd);
}
