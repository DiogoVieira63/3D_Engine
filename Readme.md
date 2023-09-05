# 3D Engine

This project is mini scene graph based 3D engine, that uses XML files as scene configurations.

## Build and Run 

    mkdir build
    cd build
    cmake ..
    make
    cd ..
    ./bin/generator plane 10 10 pontos/plane.3d
    ./bin/engine scenes/sistemaSolar.xml
