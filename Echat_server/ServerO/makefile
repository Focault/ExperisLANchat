TARGET= Echat_Server.out
CC= gcc
CFLAGS= -ansi -g -pedantic -Wall -I $(DH) -I $(PROTO) -I $(TCP) -I $(USR) -I $(USRMGR) -I $(SRV) -I $(GRPMGR) -I $(GRP)
OBJS= Protocol.o main.o ServerApplication.o UserManager.o User.o TCPserver.o GroupsManager.o Group.o
DH= ../../DLIB/DLIB-H/
PROTO= ../../CommunicationProtocol/
TCP= ../TCPv1.3_selectDriver/Server/
USRMGR= ../UserManager/
USR= ../UserManager/User/
GRPMGR= ../GroupsManager/
GRP= ../GroupsManager/Group/
SRV= ../
LIB= -Wl,-rpath=../../DLIB/ -L../../DLIB/ -ldyn -lm

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB)

main.o: $(SRV)main.c $(SRV)ServerApplication.h
	$(CC) $(CFLAGS) -c $(SRV)main.c

ServerApplication.o: $(SRV)ServerApplication.c $(SRV)ServerApplication.h $(TCP)TCPserver.h $(PROTO)ServerAddress.h $(PROTO)EchatLimits.h $(PROTO)Protocol.h $(USRMGR)UserManager.h
	$(CC) $(CFLAGS) -c $(SRV)ServerApplication.c

Protocol.o: $(PROTO)Protocol.c $(PROTO)EchatLimits.h $(PROTO)Protocol.h
	$(CC) $(CFLAGS) -c $(PROTO)Protocol.c

UserManager.o: $(USRMGR)UserManager.c $(USRMGR)UserManager.h $(PROTO)EchatLimits.h $(USR)User.h $(DH)HashMapAPI.h
	$(CC) $(CFLAGS) -c $(USRMGR)UserManager.c

User.o: $(USR)User.c $(USR)User.h $(PROTO)EchatLimits.h $(DH)ListItr.h $(DH)ListFunctions.h $(DH)GenListAPI.h
	$(CC) $(CFLAGS) -c $(USR)User.c

TCPserver.o: $(TCP)TCPserver.c $(DH)ListItr.h $(DH)ListFunctions.h $(DH)GenListAPI.h $(TCP)TCPserver.h
	$(CC) $(CFLAGS) -c $(TCP)TCPserver.c

GroupsManager.o: $(GRPMGR)GroupsManager.c $(GRPMGR)GroupsManager.h $(GRP)Group.h $(PROTO)EchatLimits.h $(DH)ListItr.h $(DH)ListFunctions.h $(DH)GenListAPI.h $(DH)HashMapAPI.h $(DH)GenQueue.h
	$(CC) $(CFLAGS) -c $(GRPMGR)GroupsManager.c

Group.o: $(GRP)Group.c $(GRP)Group.h $(PROTO)EchatLimits.h
	$(CC) $(CFLAGS) -c $(GRP)Group.c

clean:
	rm -f $(TARGET) $(OBJS)