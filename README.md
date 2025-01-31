# Gomoku (5 in a row)

## What is it?

This project is an implementation of the game Gomoku using SDL2.<br>
The rules of the game can be found [here](https://en.wikipedia.org/wiki/Gomoku)

* Primary click with the mouse to make a move
* Press \<space\> to make an engine move
* Press \<cr\> (or \<return\>) to make the engine play itself till the end

## Dependencies

* SDL2 library. Install via `sudo apt-get install libsdl2-dev`
* g++, make

## Try it by yourself

### Set a path

```
path=<your_path>
```

```
path=$HOME/Gomoku/
```

### Installation

```
git clone --filter=blob:none https://github.com/paualberti/Gomoku $path
cd $path
```

### Build process

```
make all
```

## TODO

* [x] Easy to build
* [x] Graphical interface
* [x] Computer play
* [ ] Alpha-beta engine search
