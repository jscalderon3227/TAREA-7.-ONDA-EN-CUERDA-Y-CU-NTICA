all: datos.dat
datos.dat : cuantica.x 
	./cuantica.x 400000 
	python Schröd_animation.py
	rm -f *.x
	rm -f *.dat
cuantica.x : cuantica.c
	cc cuantica.c -o cuantica.x -lm 
clean :
	rm -f *.x
	rm -f *.dat

