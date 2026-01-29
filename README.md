# MiniTetris

A Tetris implementation in C++.

## Building

Building this app has the following prerequisites:
- A C++17 or above toolchain (we use GCC 15.2 locally)
- `cmake`

To build the app, run `mkdir build && cmake -S . -B build # alias: just build `

To launch the app, run `cmake --build build --target run # alias: just run`

If you have `just` installed, you can use the recipes in the `Justfile`. `build` will build the app, `run` will launch the app, and `buildrun` will build and then launch the app.

## License

The source code is MIT licensed. Any tetris imagery and branding is property of their copyright owners and I don't claim ownership over them.
