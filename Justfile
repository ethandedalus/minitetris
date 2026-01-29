set dotenv-load

build:
  cmake -S . -B build

run:
  cmake --build build --target run

buildrun: build run
