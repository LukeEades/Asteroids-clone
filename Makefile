default: asteroids

asteroids: 
	clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -I./headers ./libraries/libraylib.a ./src/asteroid.c ./src/player.c ./src/main.c -o main
# run: 
# 	asteroids: