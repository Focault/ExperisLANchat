TARGET= ../libdyn.so
CC= gcc
CFLAGS= -ansi -g3 -pedantic -Wall -fPIC -I $(DH)
OBJS= FuncGenList.o ListItr.o internal.o ListFunctions.o FuncBST.o funcGenVector.o FuncGenHeap.o FuncGenQueue.o HashMap.o
LIST= ../GenDoubleList/
BST= ../GenBST/
VEC= ../GenericVector/
HEAP= ../GenHeap/
QUEUE= ../GenQueue/
HASH= ../HashMap/
DH= ../DLIB-H/

$(TARGET): $(OBJS)
	$(CC) -shared -o $(TARGET) $(OBJS)

FuncGenList.o: $(LIST)FuncGenList.c $(DH)GenListAPI.h $(LIST)internal.h
	$(CC) $(CFLAGS) -c $(LIST)FuncGenList.c

ListItr.o: $(LIST)ListItr.c $(DH)GenListAPI.h $(DH)ListItr.h $(LIST)internal.h
	$(CC) $(CFLAGS) -c $(LIST)ListItr.c

internal.o: $(LIST)internal.c $(LIST)internal.h
	$(CC) $(CFLAGS) -c $(LIST)internal.c

ListFunctions.o: $(LIST)ListFunctions.c $(DH)ListFunctions.h $(DH)ListItr.h
	$(CC) $(CFLAGS) -c $(LIST)ListFunctions.c

FuncBST.o: $(BST)FuncBST.c $(DH)BSTree.h
	$(CC) $(CFLAGS) -c $(BST)FuncBST.c

funcGenVector.o: $(VEC)funcGenVector.c $(DH)GenVector.h
	$(CC) $(CFLAGS) -c $(VEC)funcGenVector.c

FuncGenHeap.o: $(HEAP)FuncGenHeap.c $(DH)GenHeap.h $(DH)GenVector.h
	$(CC) $(CFLAGS) -c $(HEAP)FuncGenHeap.c

FuncGenQueue.o: $(QUEUE)FuncGenQueue.c $(DH)GenQueue.h
	$(CC) $(CFLAGS) -c $(QUEUE)FuncGenQueue.c

HashMap.o: $(HASH)HashMap.c $(DH)HashMapAPI.h $(DH)ListItr.h $(DH)ListFunctions.h
	$(CC) $(CFLAGS) -c $(HASH)HashMap.c -lm

clean:
	rm -f $(TARGET) $(OBJS)