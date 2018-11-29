BIN_DIR = ./bin/
OBJ_DIR = $(BIN_DIR)obj/
SRC_DIR = ./Source/

COMPILE_OPTIONS = -Wall -std=c++11

OPENGL_OBJ = $(OBJ_DIR)glimpl.o $(OBJ_DIR)glcommon.o $(OBJ_DIR)gldevice.o $(OBJ_DIR)gldevicelinux.o $(OBJ_DIR)gldevicecontext.o $(OBJ_DIR)glbuffer.o $(OBJ_DIR)glshader.o $(OBJ_DIR)glrootlayout.o $(OBJ_DIR)glpipelinestate.o $(OBJ_DIR)vaocache.o $(OBJ_DIR)glswapchain.o $(OBJ_DIR)glswapchainlinux.o
MATH_OBJ = $(OBJ_DIR)vector2.o $(OBJ_DIR)vector3.o $(OBJ_DIR)vector4.o $(OBJ_DIR)matrix2.o $(OBJ_DIR)matrix3.o $(OBJ_DIR)matrix4.o $(OBJ_DIR)color.o $(OBJ_DIR)colorf.o $(OBJ_DIR)mathutilities.o $(OBJ_DIR)point.o $(OBJ_DIR)randomgenereator.o $(OBJ_DIR)ray.o $(OBJ_DIR)rectangle.o
OBJECT_FILES = $(OPENGL_OBJ) $(MATH_OBJ) $(OBJ_DIR)linuxSystemImpl.o $(OBJ_DIR)linuxWindowImpl.o $(OBJ_DIR)event.o $(OBJ_DIR)linuxKeyCodes.o $(OBJ_DIR)stringutilities.o $(OBJ_DIR)textureutilities.o $(OBJ_DIR)engineutilities.o $(OBJ_DIR)linuxKeyboardImpl.o $(OBJ_DIR)timestamp.o $(OBJ_DIR)clock.o $(OBJ_DIR)linuxMouseImpl.o $(OBJ_DIR)linuxClipboardImpl.o $(OBJ_DIR)linuxSensorsImpl.o $(OBJ_DIR)linuxTouchScreenImpl.o $(OBJ_DIR)linuxDebugImpl.o $(OBJ_DIR)image.o $(OBJ_DIR)linux.o

libRayEngine.so: bin math opengl linuxSystemImpl.o linuxWindowImpl.o linuxKeyCodes.o linuxKeyboardImpl.o event.o stringutilities.o engineutilities.o textureutilities.o timestamp.o clock.o linuxMouseImpl.o linuxClipboardImpl.o linuxSensorsImpl.o linuxTouchScreenImpl.o linuxDebugImpl.o image.o linux.o
	g++ -shared $(OBJECT_FILES) -o $(BIN_DIR)libRayEngine.so -lX11 -lGL

linuxSystemImpl.o: $(SRC_DIR)Linux/LinuxSystemImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxSystemImpl.cpp -o $(OBJ_DIR)linuxSystemImpl.o -fPIC 

linuxWindowImpl.o: $(SRC_DIR)Linux/LinuxWindowImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxWindowImpl.cpp -o $(OBJ_DIR)linuxWindowImpl.o -fPIC

linuxKeyCodes.o: $(SRC_DIR)Linux/LinuxKeyCodes.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxKeyCodes.cpp -o $(OBJ_DIR)linuxKeyCodes.o -fPIC

linuxKeyboardImpl.o: $(SRC_DIR)Linux/LinuxKeyboardImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxKeyboardImpl.cpp -o $(OBJ_DIR)linuxKeyboardImpl.o -fPIC

linuxMouseImpl.o: $(SRC_DIR)Linux/LinuxMouseImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxMouseImpl.cpp -o $(OBJ_DIR)linuxMouseImpl.o -fPIC

linuxClipboardImpl.o: $(SRC_DIR)Linux/LinuxClipboardImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxClipboardImpl.cpp -o $(OBJ_DIR)linuxClipboardImpl.o -fPIC

linuxSensorsImpl.o: $(SRC_DIR)Linux/LinuxSensorsImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxSensorsImpl.cpp -o $(OBJ_DIR)linuxSensorsImpl.o -fPIC

linuxTouchScreenImpl.o: $(SRC_DIR)Linux/LinuxTouchScreenImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxTouchScreenImpl.cpp -o $(OBJ_DIR)linuxTouchScreenImpl.o -fPIC

linuxDebugImpl.o: $(SRC_DIR)Linux/LinuxDebugImpl.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/LinuxDebugImpl.cpp -o $(OBJ_DIR)linuxDebugImpl.o -fPIC

linux.o: $(SRC_DIR)Linux/Linux.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Linux/Linux.cpp -o $(OBJ_DIR)linux.o -fPIC

event.o: $(SRC_DIR)System/Event.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)System/Event.cpp -o $(OBJ_DIR)event.o -fPIC

timestamp.o: $(SRC_DIR)System/TimeStamp.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)System/TimeStamp.cpp -o $(OBJ_DIR)timestamp.o -fPIC

clock.o: $(SRC_DIR)System/Clock.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)System/Clock.cpp -o $(OBJ_DIR)clock.o -fPIC

image.o: $(SRC_DIR)System/Image.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)System/Image.cpp -o $(OBJ_DIR)image.o -fPIC

stringutilities.o: $(SRC_DIR)Utilities/StringUtilities.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Utilities/StringUtilities.cpp -o $(OBJ_DIR)stringutilities.o -fPIC

engineutilities.o: $(SRC_DIR)Utilities/EngineUtilities.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Utilities/EngineUtilities.cpp -o $(OBJ_DIR)engineutilities.o -fPIC

textureutilities.o: $(SRC_DIR)Utilities/TextureUtilities.cpp obj
	g++ $(COMPILE_OPTIONS) -c $(SRC_DIR)Utilities/TextureUtilities.cpp -o $(OBJ_DIR)textureutilities.o -fPIC

math:
	cd Source/Math && make

opengl:
	cd Source/OpenGL && make

obj: bin
	mkdir -p $(OBJ_DIR)

bin: 
	mkdir -p $(BIN_DIR)

.PHONY: clean
clean:
	rm -r $(OBJECT_FILES) $(BIN_DIR)libRayEngine.so