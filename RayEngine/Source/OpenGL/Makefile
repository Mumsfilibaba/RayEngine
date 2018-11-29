BIN_DIR = ../../bin/
OBJ_DIR = $(BIN_DIR)obj/
COMPILE_OPTIONS = -Wall -std=c++11

opengl: obj glimpl.o glcommon.o gldevice.o gldevicelinux.o gldevicecontext.o glbuffer.o glshader.o glrootlayout.o glpipelinestate.o vaocache.o glswapchain.o glswapchainlinux.o

glimpl.o: GLImpl.cpp
	g++ $(COMPILE_OPTIONS) -c GLImpl.cpp -o $(OBJ_DIR)glimpl.o -fPIC

glcommon.o: GLCommon.cpp
	g++ $(COMPILE_OPTIONS) -c GLCommon.cpp -o $(OBJ_DIR)glcommon.o -fPIC

gldevice.o: GLDevice.cpp
	g++ $(COMPILE_OPTIONS) -c GLDevice.cpp -o $(OBJ_DIR)gldevice.o -fPIC

gldevicelinux.o: GLDeviceLinux.cpp
	g++ $(COMPILE_OPTIONS) -c GLDeviceLinux.cpp -o $(OBJ_DIR)gldevicelinux.o -fPIC

gldevicecontext.o: GLDeviceContext.cpp
	g++ $(COMPILE_OPTIONS) -c GLDeviceContext.cpp -o $(OBJ_DIR)gldevicecontext.o -fPIC

glbuffer.o: GLBuffer.cpp
	g++ $(COMPILE_OPTIONS) -c GLBuffer.cpp -o $(OBJ_DIR)glbuffer.o -fPIC

glshader.o: GLShader.cpp
	g++ $(COMPILE_OPTIONS) -c GLShader.cpp -o $(OBJ_DIR)glshader.o -fPIC

glrootlayout.o: GLRootLayout.cpp
	g++ $(COMPILE_OPTIONS) -c GLRootLayout.cpp -o $(OBJ_DIR)glrootlayout.o -fPIC

glpipelinestate.o: GLPipelineState.cpp
	g++ $(COMPILE_OPTIONS) -c GLPipelineState.cpp -o $(OBJ_DIR)glpipelinestate.o -fPIC

vaocache.o: VAOCache.cpp
	g++ $(COMPILE_OPTIONS) -c VAOCache.cpp -o $(OBJ_DIR)vaocache.o -fPIC

glswapchain.o: GLSwapchain.cpp
	g++ $(COMPILE_OPTIONS) -c GLSwapchain.cpp -o $(OBJ_DIR)glswapchain.o -fPIC

glswapchainlinux.o: GLSwapchainLinux.cpp
	g++ $(COMPILE_OPTIONS) -c GLSwapchainLinux.cpp -o $(OBJ_DIR)glswapchainlinux.o -fPIC

obj: bin
	mkdir -p $(OBJ_DIR)

bin: 
	mkdir -p $(BIN_DIR)