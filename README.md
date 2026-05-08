# Cube 1 Engine

A slightly modified Cube 1 Engine to work on modern systems.

Tweaks:
- Use STB Libraries instead of SDL_Image
- Legacy OpenGL fixes for macOS
- Use CMake instead of `make` as a build system
- Ported to SDL3 / SDL3 Mixer with few workarounds

## Building

Dependencies: SDL 3, SDL_mixer 3, OpenGL, zlib
```sh
mkdir build && cd build
cmake ..
cmake --build .
```

This produces `cube_client` and `cube_server` in the build directory. To build
only one target:
```sh
cmake .. -DBUILD_CLIENT=OFF   # server only
cmake .. -DBUILD_SERVER=OFF   # client only
```

Place assets, assuming the
["cube_2005_08_29_unix.tar.gz"](https://sourceforge.net/projects/cube/files/cube/2005_08_29/cube_2005_08_29_unix.tar.gz/download) is available in the root repository:
```
tar -xzf cube_2005_08_29_unix.tar.gz
```
You may now start the game using the assets downloaded:
```
cd cube;../build/src/cube_client
```

## License

You may use the cube source code if you abide by the ZLIB license
http://www.opensource.org/licenses/zlib-license.php
(very similar to the BSD license), see `LICENSE` for more details.

Cube game engine source code, any release.
Wouter van Oortmerssen aka Aardappel
http://strlen.com

For additional authors/contributors, see the cube binary distribution
readme.html
