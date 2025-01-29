# Gomoku (5 in a row)

## Try it by yourself

<details><summary>Set a path</summary>

```
path=<your_path>
```

```
path=$HOME/Gomoku/
```

</details>

<details><summary>Clone the project</summary>

```
git clone https://github.com/paualberti/Gomoku $path
cd $path
```

</details>

## What is it?

This project is an implementation of the game Gomoku using SDL2.<br>
The rules of the game can be found [here](https://en.wikipedia.org/wiki/Gomoku)

* Primary click with the mouse to make a move
* Press \<space\> to make an engine move
* Press \<cr\> (or \<return\>) to make the engine play itself till the end

## Dependencies

* SDL2 library. Install via `sudo apt-get install libsdl2-dev`
* g++, make

## TODO

* [x] Easy to build
* [x] Graphical interface
* [x] Computer play
* [ ] Alpha-beta engine search
