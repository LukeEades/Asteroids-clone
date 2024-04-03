default: asteroids

asteroids: 
	clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -I./headers ./libraries/libraylib.a ./src/player.c ./src/asteroid.c ./src/main.c -o main
# run: 
# 	asteroids: