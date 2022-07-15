TARGET= Echat_Server.out
CC= gcc
CFLAGS= -ansi -g -pedantic -Wall -I $(DH) -I $(PROTO) -I $(TCP) -I $(USR) -I $(USRMGR)
OBJS= Protocol.o main.o ServerApplication.o UserManager.o User.o TCPserver.o
DH= ../DLIB/DLIB-H/
PROTO= ../CommunicationProtocol/
TCP= TCPv1.3_selectDriver/Server/
USRMGR= UserManager/
USR= UserManager/User/
LIB= -Wl,-rpath=../DLIB/ -L../DLIB/ -ldyn -lm

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB)

main.o: main.c ServerApplication.h
	$(CC) $(CFLAGS) -c main.c

ServerApplication.o: ServerApplication.c ServerApplication.h $(TCP)TCPserver.h $(PROTO)ServerAddress.h $(PROTO)EchatLimits.h $(PROTO)Protocol.h $(USRMGR)UserManager.h
	$(CC) $(CFLAGS) -c ServerApplication.c

Protocol.o: $(PROTO)Protocol.c $(PROTO)EchatLimits.h $(PROTO)Protocol.h
	$(CC) $(CFLAGS) -c $(PROTO)Protocol.c

UserManager.o: $(USRMGR)UserManager.c $(USRMGR)UserManager.h $(PROTO)EchatLimits.h $(USR)User.h
	$(CC) $(CFLAGS) -c $(USRMGR)UserManager.c

User.o: $(USR)User.c $(USR)User.h $(PROTO)EchatLimits.h $(DH)ListItr.h $(DH)ListFunctions.h $(DH)GenListAPI.h
	$(CC) $(CFLAGS) -c $(USR)User.c

TCPserver.o: $(TCP)TCPserver.c $(DH)ListItr.h $(DH)ListFunctions.h $(DH)GenListAPI.h $(TCP)TCPserver.h
	$(CC) $(CFLAGS) -c $(TCP)TCPserver.c

clean:
	rm -f $(TARGET) $(OBJS)