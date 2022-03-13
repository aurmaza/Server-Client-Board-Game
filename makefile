slidingpuzzle-v4:		slidingpuzzle-v4.o sp-game.o sp-tcp-client.o sp-tcp-server.o
			gcc slidingpuzzle-v4.o sp-game.o sp-tcp-client.o sp-tcp-server.o -o slidingpuzzle-v4

slidingpuzzle-v4.o:		slidingpuzzle-v4.c
						gcc -c slidingpuzzle-v4.c

sp-tcp-client.o:		sp-tcp-client.c commands.h
						gcc -c sp-tcp-client.c

sp-tcp-server.o:		sp-tcp-server.c commands.h
						gcc -c sp-tcp-server.c 

sp-game.o:				sp-game.c sp-game.h commands.h
						gcc -c sp-game.c


clean:
						rm sp-tcp-server.o sp-tcp-client.o slidingpuzzle-v4.o sp-game.o slidingpuzzle-v4