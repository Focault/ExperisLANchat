TARGET= ProtocolTests.out
CC= gcc
CFLAGS= -ansi -g3 -pedantic -Wall -I $(FILEPATH)
OBJS= ProtocolTests.o Protocol.o
FILEPATH= ../

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

ProtocolTests.o: ProtocolTests.c $(FILEPATH)Protocol.h $(FILEPATH)EchatLimits.h
	$(CC) $(CFLAGS) -c ProtocolTests.c

Protocol.o: $(FILEPATH)Protocol.c $(FILEPATH)Protocol.h $(FILEPATH)EchatLimits.h
	$(CC) $(CFLAGS) -c $(FILEPATH)Protocol.c

clean:
	rm -f $(TARGET) $(OBJS)