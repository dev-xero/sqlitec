# SQLiteC

C implementation of a SQLite clone, with a full REPL mode.

## Building

The project uses cmake to compile the source file. Install make if you haven't already.

```bash
sudo apt upgrade && sudo apt-get install cmake
```

Then compiling, make a `builds` folder to contain the executable

```bash
mkdir -p build && cd ./build
```

Initialize cmake and build
```bash
cmake ..
cmake --build .
```
