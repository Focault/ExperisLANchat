
## This is a LAN Chat made as a project for Experis Core Course By Team 1 of H71 Class ##


### IMPORTENT: For use in multiple computers change values in ServerAddress.h to real server details ###

## Compilation Instructions: ##

1. Enter DLIB/DLIB-O and make

$ cd DLIB/DLIB-O \n
$ make

2. Enter Echat_Client/Chat and compile Sender and Receiver seperatly like that:

$ gcc -ansi -pedantic Sender.c -o Sender.out \n
$ gcc -ansi -pedantic Receiver.c -o Receiver.out

3. Enter Echat_Client/ClientO and make

$ cd Echat_Client/ClientO \n
$ make

4. Enter Echat_server/ServerO and make

$ cd Echat_server/ServerO \n
$ make

5. run Echat_Server.out and Echat_Client.out (In that order!)

$ ./Echat_Server.out \n
$ ./Echat_Client.out