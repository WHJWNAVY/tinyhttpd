all: httpd

httpd: httpd.c
	#gcc -W -Wall -lsocket -lpthread -o httpd httpd.c
	gcc -W -Wall -o httpd httpd.c -lm -lpthread

clean:
	rm httpd
