all: datos.dat
datos.dat : quantum_Wave.x 
	./quantum_Wave.x 500000 
	python Schröd_animation.py
	rm -f *.x
	rm -f *.dat
quantum_Wave.x : quantum_Wave.c
	cc quantum_Wave.c -o quantum_Wave.x -lm 
clean :
	rm -f *.x
	rm -f *.dat

