set dotenv-load

build:
  cmake -S . -B build

run:
  cmake --build build --target run

buildrun: build run

build-web:
  emcmake cmake -DPLATFORM=Web -S . -B build-web && cmake --build build-web --target web

build-web-server:
  go build -C web -ldflags="-w -s" -o ../bin/minitetris . && ./bin/minitetris

run-web-server:
  ./bin/minitetris

buildrun-web: build-web build-web-server run-web-server
