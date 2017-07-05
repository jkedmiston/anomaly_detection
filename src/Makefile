TARGET=flag_purchases.exe
MAIN = main.o
OBJECTS = misc.o node.o purchase.o date_object.o
LFLAGS = 
CC=g++
CFLAGS=-Wall -c

default: $(TARGET)
release: CFLAGS += -O3
release: $(TARGET)

debug: CFLAGS += -g -D DEBUG
debug: $(TARGET)

$(TARGET):	$(OBJECTS) $(MAIN)
	$(CC) -o $(TARGET) $(OBJECTS) $(MAIN) $(LFLAGS)

main.o: main.cc
	$(CC) $(CFLAGS) main.cc

misc.o: misc.cc
	$(CC) $(CFLAGS) misc.cc

node.o: node.cc
	$(CC) $(CFLAGS) node.cc

purchase.o: purchase.cc
	$(CC) $(CFLAGS) purchase.cc

date_object.o: date_object.cc
	$(CC) $(CFLAGS) date_object.cc
