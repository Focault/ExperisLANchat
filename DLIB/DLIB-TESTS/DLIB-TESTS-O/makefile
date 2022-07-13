TARGET= ../../DLIB-TESTS.out
CC= gcc
CFLAGS= -ansi -g3 -pedantic -Wall -I $(DH)
OBJS= TestDLIB.o BSTTests.o GenListTests.o GenVectorTests.o GenHeapTests.o GenQueueTests.o HashMapTests.o
DH= ../../DLIB-H/
LIST= ../../GenDoubleList/
LIB= -Wl,-rpath=./ -L../../ -ldyn -lm

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB)

TestDLIB.o: ../TestDLIB.c $(DH)TestDLIB.h
	$(CC) $(CFLAGS) -c ../TestDLIB.c

BSTTests.o: ../BSTTests.c $(DH)BSTree.h $(DH)TestDLIB.h
	$(CC) $(CFLAGS) -c ../BSTTests.c

GenListTests.o: ../GenListTests.c $(DH)GenListAPI.h $(DH)ListItr.h $(LIST)internal.h $(DH)ListFunctions.h $(DH)TestDLIB.h
	$(CC) $(CFLAGS) -c ../GenListTests.c

GenVectorTests.o: ../GenVectorTests.c $(DH)GenVector.h $(DH)TestDLIB.h
	$(CC) $(CFLAGS) -c ../GenVectorTests.c

GenHeapTests.o: ../GenHeapTests.c $(DH)GenHeap.h $(DH)GenVector.h $(DH)TestDLIB.h
	$(CC) $(CFLAGS) -c ../GenHeapTests.c

GenQueueTests.o: ../GenQueueTests.c $(DH)GenQueue.h $(DH)TestDLIB.h
	$(CC) $(CFLAGS) -c ../GenQueueTests.c

HashMapTests.o: ../HashMapTests.c $(DH)HashMapAPI.h $(DH)TestDLIB.h
	$(CC) $(CFLAGS) -c ../HashMapTests.c

clean:
	rm -f $(TARGET) $(OBJS)