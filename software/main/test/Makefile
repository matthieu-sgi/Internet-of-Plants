build:compile
	@echo "Creating shared lib"
	g++ -shared -o libiop.so iop_lib.o -lm

compile:
	@echo "Compiling"
	g++  -c -fPIC -o iop_lib.o iop_lib.cpp -lm
