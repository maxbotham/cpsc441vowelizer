Hi there, welcome to my submission for CPSC 441 assignment 2.

Please navigate to this directory (the one containing README.md) in a terminal window on the machine you want to run the server on. Please run the following command:

g++ server.cpp -o server

Now, to run the server, please run the following command:

./server

Next, please navigate to this directory (the one containing README.md) in a terminal window on the machine you want to run the client on. 

Now, edit the file client.cpp in your favourite code editor. 

Please change line 18 (#define IP_ADDR "127.0.0.1") to contain the IP address of the machine on which you are running the server. If running both the client and the server on the same machine, please leave this IP address as is.

Now, run the following command:

g++ client.cpp -o client

To run the client, please run the following command:

./client

The client and server should now be connected persistently through TCP. Please follow the instructions in the client terminal window to test the envoweling and devoweling system, both with simple and advanced methods.

All pieces of this assignment have been completed, except for the 4 bonus marks.