all: Wave_Eq.dat

 Wave_Eq.dat : Wave_Eq.x
	./Wave_Eq.x 1000 8000
	python Wave_animation.py
	rm -f *.x
	rm -f *.dat
Wave_Eq.x : Wave_Eq.c
	cc Wave_Eq.c -o Wave_Eq.x -lm 

clean :
	rm -f *.o
