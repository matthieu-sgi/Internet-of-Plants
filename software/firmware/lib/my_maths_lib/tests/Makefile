build:compile
	@echo "Creating shared lib"
	g++ -shared -o iop_maths_lib.so iop_maths_lib.o -lm

compile:
	@echo "Compiling"
	g++  -c -fPIC -o iop_maths_lib.o ../iop_maths_lib.cpp -lm
