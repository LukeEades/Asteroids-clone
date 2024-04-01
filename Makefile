default: asteroids

asteroids: 
	clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL ./libraries/libraylib.a main.c -o main