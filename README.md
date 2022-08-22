# arcade-machine
An application to showcase and execute Splashkit games  

## Pre-requisites (all operating systems)

+ Install the [SplashKit](https://splashkit.io) SDK using the [guide](https://splashkit.io/articles/installation/)

## Pre-requisites (mingw32 / Windows)

+ Install `make` using `pacman -S make`

## Building arcade machine (using the Makefile)
Compiling Arcade Machine using the Makefile allows incremental building of changed objects.

```bash
cd arcade-machine
make
./ArcadeMachine
```

Subsequent builds (as you change code) can be completed by using just `make`. If you need to run a clean build again, you can use `make clean` first proceeded by `make`.

## Building arcade machine (manually)
+ Compile the application with the command ```skm clang++ src/* -Iinclude -lstdc++fs -o test```  
+ Run the application ```./test```
