sea_battle: main.c arrangement/*.c core/*.c core/combat/*.c graphics/*.c
	gcc -I. main.c arrangement/*.c core/*.c core/combat/*.c graphics/*.c -lSDL2 -lSDL2_image -lSDL2_mixer -o sea_battle

clean:
	rm -f sea_battle

run: sea_battle
	./sea_battle