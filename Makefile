CC=gcc
CFLAGS=-c
LDFLAGS=`pkg-config --libs --cflags gtk+-2.0  glib-2.0 dbus-glib-1 libnm-util libnm-glib`
#LDFLAGS=`pkg-config --libs --cflags gtk+-2.0`

SOURCES=main.c carrick-item.h carrick-item.c network.h network.c
EXECUTABLE=testui
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) 
	$(CC) $(LDFLAGS) $(SOURCES) -o $@ -g 
	
clean:
	rm $(EXECUTABLE) *.o
