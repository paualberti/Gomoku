# Gomoku (5 in a row)

## What is it?

This project is an implementation of the game Gomoku using SDL2.

The rules of the game can be found [here](https://en.wikipedia.org/wiki/Gomoku)

## Structure

### Box

Each square of the grid. Keeps track of each state a square can have.

### Game

The logic of the game are contained here. Uses an array of squares to operate.

### Handler

The logic of the program. Creates and manages user input.

### Other

main() creates the game loop, Handler object. circle() draws a circle.

## Goals

I started this project to experiment with SDL2 and because I wanted to program an AI capable of beating me.

However I only managed to make it play under a set of rules with no depth in the search.
