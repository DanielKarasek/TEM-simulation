# Makefile
# 11.05.2021
# VUT FIT ICP microscope
# Authors: Filip Osvald xosval04, Daniel Karasek xkaras38

all: update
	make -f QMAKEFILE 

run: all
	./ICP

update: src/ICP.pro 
	/home/makoakopako/Qt/5.15.2/gcc_64/bin/qmake -o QMAKEFILE src/ICP.pro 

doxygen:
	doxygen doc/Doxyfile

clean:
	rm -f ICP *.o *.cpp *.h QMAKEFILE 

pack:
	zip -r 3-xkaras38-xosval04.zip src examples README.txt Makefile doc/Doxyfile
