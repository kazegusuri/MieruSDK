
LCD = lcd
SRC = lcd.c

PIPE = lcd_pipe


all:
	gcc -Wall -O2 $(SRC) -o $(LCD) `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` `pkg-config --libs gthread-2.0`

run:
	./$(LCD) -s $(PIPE) > /dev/null &

clean:
	rm -f *.o *~ $(LCD) $(CLIENT) $(PIPE)

