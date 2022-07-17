TARGET= UserManagerTests.out
CC= gcc
CFLAGS= -ansi -g -pedantic -I $(DH) -I User/ -I ../../CommunicationProtocol
OBJS=  UserManager.o User.o UserManagerTests.o
DH= ../../DLIB/DLIB-H/
LIB= -Wl,-rpath=../../DLIB/ -L../../DLIB/ -ldyn -lm

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB)

UserManagerTests.o: UserManagerTests.c UserManager.h 
	$(CC) $(CFLAGS) -c UserManagerTests.c

UserManager.o: UserManager.c UserManager.h 
	$(CC) $(CFLAGS) -c UserManager.c

User.o: User/User.c User/User.h  	
	$(CC) $(CFLAGS) -c User/User.c



clean:
	rm -f $(TARGET) $(OBJS)
