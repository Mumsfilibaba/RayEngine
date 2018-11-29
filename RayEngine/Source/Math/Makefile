BIN_DIR = ../../bin/
OBJ_DIR = $(BIN_DIR)obj/
COMPILE_OPTIONS = -Wall -std=c++11

math: obj vector2.o vector3.o vector4.o matrix2.o matrix3.o matrix4.o color.o colorf.o mathutilities.o point.o randomgenereator.o ray.o rectangle.o

mathutilities.o: MathUtilities.cpp 
	g++ $(COMPILE_OPTIONS) -c MathUtilities.cpp -o $(OBJ_DIR)mathutilities.o -fPIC

vector2.o: Vector2.cpp
	g++ $(COMPILE_OPTIONS) -c Vector2.cpp -o $(OBJ_DIR)vector2.o -fPIC

vector3.o: Vector3.cpp
	g++ $(COMPILE_OPTIONS) -c Vector3.cpp -o $(OBJ_DIR)vector3.o -fPIC

vector4.o: Vector4.cpp
	g++ $(COMPILE_OPTIONS) -c Vector4.cpp -o $(OBJ_DIR)vector4.o -fPIC

matrix2.o: Matrix2.cpp
	g++ $(COMPILE_OPTIONS) -c Matrix2.cpp -o $(OBJ_DIR)matrix2.o -fPIC

matrix3.o: Matrix3.cpp
	g++ $(COMPILE_OPTIONS) -c Matrix3.cpp -o $(OBJ_DIR)matrix3.o -fPIC

matrix4.o: Matrix4.cpp
	g++ $(COMPILE_OPTIONS) -c Matrix4.cpp -o $(OBJ_DIR)matrix4.o -fPIC

color.o: Color.cpp
	g++ $(COMPILE_OPTIONS) -c Color.cpp -o $(OBJ_DIR)color.o -fPIC

colorf.o: ColorF.cpp
	g++ $(COMPILE_OPTIONS) -c ColorF.cpp -o $(OBJ_DIR)colorf.o -fPIC

point.o: Point.cpp
	g++ $(COMPILE_OPTIONS) -c Point.cpp -o $(OBJ_DIR)point.o -fPIC

randomgenereator.o: RandomGenerator.cpp
	g++ $(COMPILE_OPTIONS) -c RandomGenerator.cpp -o $(OBJ_DIR)randomgenereator.o -fPIC

ray.o: Ray.cpp
	g++ $(COMPILE_OPTIONS) -c Ray.cpp -o $(OBJ_DIR)ray.o -fPIC

rectangle.o: Rectangle.cpp
	g++ $(COMPILE_OPTIONS) -c Rectangle.cpp -o $(OBJ_DIR)rectangle.o -fPIC

obj: bin
	mkdir -p $(OBJ_DIR)

bin: 
	mkdir -p $(BIN_DIR)