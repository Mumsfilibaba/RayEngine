INCLUDE_DIR = -I../../RayEngine/Include/
LIBRARY_DIR = -L../../RayEngine/bin
LDFLAGS= "-Wl,-rpath,../../RayEngine/bin/"
LIBS = -lRayEngine
COMPILE_OPTIONS = -Wall -std=c++11

main: main.cpp lib
	g++ $(INCLUDE_DIR) $(LIBRARY_DIR) $(LDFLAGS) $(COMPILE_OPTIONS) main.cpp -o main $(LIBS)

lib:
	cd ../../RayEngine && make

.PHONY: clean
clean: cleanLib
	rm -r main

.PHONY: cleanLib
cleanLib: 
	cd ../../RayEngine && make clean