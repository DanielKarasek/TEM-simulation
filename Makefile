# Makefile
# 11.05.2021
# VUT FIT ICP microscope
# Authors: Filip Osvald xosval04, Daniel Karasek xkaras38

all: update
	make -f QMAKEFILE 

run: all
	./TEM

update: src/TEM.pro 
	qmake -o QMAKEFILE src/TEM.pro 

doxygen:
	doxygen doc/Doxyfile

clean:
	rm -f TEM *.o *.cpp *.h QMAKEFILE 

pack:
	zip -r 3-xkaras38-xosval04.zip src examples README.txt Makefile doc/Doxyfile
