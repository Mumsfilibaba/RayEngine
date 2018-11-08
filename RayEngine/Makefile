BIN_DIR = ./bin/
OBJ_DIR = $(BIN_DIR)obj/
SRC_DIR = ./Source/

COMPILE_OPTIONS = -Wall -std=c++11

MATH_OBJ = $(OBJ_DIR)vector2.o $(OBJ_DIR)vector3.o $(OBJ_DIR)vector4.o $(OBJ_DIR)matrix2.o $(OBJ_DIR)matrix3.o $(OBJ_DIR)matrix4.o $(OBJ_DIR)color.o $(OBJ_DIR)colorf.o $(OBJ_DIR)mathutilities.o $(OBJ_DIR)point.o $(OBJ_DIR)randomgenereator.o $(OBJ_DIR)ray.o $(OBJ_DIR)rectangle.o
OBJECT_FILES = $(MATH_OBJ) $(OBJ_DIR)linuxSystemImpl.o $(OBJ_DIR)linuxWindowImpl.o $(OBJ_DIR)event.o $(OBJ_DIR)linuxKeyCodes.o $(OBJ_DIR)stringutilities.o $(OBJ_DIR)linuxKeyboardImpl.o $(OBJ_DIR)timestamp.o $(OBJ_DIR)clock.o $(OBJ_DIR)sensors.o

libRayEngine.so: bin math linuxSystemImpl.o linuxWindowImpl.o linuxKeyCodes.o linuxKeyboardImpl.o event.o stringutilities.o timestamp.o clock.o sensors.o
	g++ -shared $(OBJECT_FILES) -o $(BIN_DIR)libRayEngine.so -lX11

linuxSystemImpl.o: $(SRC_DIR)Linux/LinuxSystemImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxSystemImpl.cpp -o $(OBJ_DIR)linuxSystemImpl.o -fPIC 

linuxWindowImpl.o: $(SRC_DIR)Linux/LinuxWindowImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxWindowImpl.cpp -o $(OBJ_DIR)linuxWindowImpl.o -fPIC

linuxKeyCodes.o: $(SRC_DIR)Linux/LinuxKeyCodes.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxKeyCodes.cpp -o $(OBJ_DIR)linuxKeyCodes.o -fPIC

linuxKeyboardImpl.o: $(SRC_DIR)Linux/LinuxKeyboardImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxKeyboardImpl.cpp -o $(OBJ_DIR)linuxKeyboardImpl.o -fPIC

event.o: $(SRC_DIR)System/Event.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)System/Event.cpp -o $(OBJ_DIR)event.o -fPIC

timestamp.o: $(SRC_DIR)System/TimeStamp.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)System/TimeStamp.cpp -o $(OBJ_DIR)timestamp.o -fPIC

clock.o: $(SRC_DIR)System/Clock.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)System/Clock.cpp -o $(OBJ_DIR)clock.o -fPIC

sensors.o: $(SRC_DIR)System/Sensors.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)System/Sensors.cpp -o $(OBJ_DIR)sensors.o -fPIC

stringutilities.o: $(SRC_DIR)Utilities/StringUtilities.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Utilities/StringUtilities.cpp -o $(OBJ_DIR)stringutilities.o -fPIC

math:
	cd Source/Math && make

obj: bin
	mkdir -p $(OBJ_DIR)

bin: 
	mkdir -p $(BIN_DIR)

.PHONY: clean
clean:
	rm -r $(OBJECT_FILES) $(BIN_DIR)libRayEngine.so