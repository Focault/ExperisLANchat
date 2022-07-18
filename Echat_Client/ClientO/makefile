TARGET= Echat_Client.out
CC= gcc
CFLAGS= -ansi -g3 -pedantic -Wall -I $(DH) -I $(UI) -I $(PROTO) -I $(NET) -I $(CHAT) -I $(CLI) 
OBJS= Protocol.o ClientNet.o ClientApplication.o UI.o main.o Chat.o
DH= ../../DLIB/DLIB-H/
UI= ../UI/
PROTO= ../../CommunicationProtocol/
NET= ../ClientNet/
CLI= ../
CHAT= ../Chat/
LIB= -Wl,-rpath=../../DLIB/ -L../../DLIB/ -ldyn -lm

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB)

main.o: $(CLI)main.c $(CLI)ClientApplication.h
	$(CC) $(CFLAGS) -c $(CLI)main.c

UI.o: $(UI)UI.c $(PROTO)EchatLimits.h $(UI)UI.h
	$(CC) $(CFLAGS) -c $(UI)UI.c

ClientApplication.o: $(CLI)ClientApplication.c $(PROTO)EchatLimits.h $(CLI)ClientApplication.h $(UI)UI.h $(NET)ClientNet.h $(PROTO)ServerAddress.h $(PROTO)Protocol.h $(CHAT)Chat.h
	$(CC) $(CFLAGS) -c $(CLI)ClientApplication.c

ClientNet.o: $(NET)ClientNet.c $(PROTO)EchatLimits.h $(NET)ClientNet.h
	$(CC) $(CFLAGS) -c $(NET)ClientNet.c

Protocol.o: $(PROTO)Protocol.c $(PROTO)EchatLimits.h $(PROTO)Protocol.h
	$(CC) $(CFLAGS) -c $(PROTO)Protocol.c

Chat.o: $(CHAT)Chat.c $(CHAT)Chat.h
	$(CC) $(CFLAGS) -c $(CHAT)Chat.c

clean:
	rm -f $(TARGET) $(OBJS)