# sheep-eating
C++ console game with based object oriented mechanisms. My very first approach to classes, methods, etc.  

The Player is steering the wolf(which is marked as 'W'),by pressing W,A,S,D keys. Wolf is on the board along with five sheeps(You can change the number of spawned sheeps in main.cpp code). 

## How to run it
The game was written on Linux and at this point it works only on this platform.
You only need g++ which is on every distro provided by default probably(if not download it manually)

Then you can build it with:
```
g++ main.cpp -o game.out
```
and run the executable:
```
./game.out
```
## What's the goal of this game:
You are a wolf and as in natural environment you need to eat in order to survive.
Your character has jumped into sheep run(an extensive tract of land on which sheep are pastured)

Your moves cost some energy so you are in hurry in order to live.
Every eaten sheep fills your belly :)

## Rules:

1. Use W, S, A, D to move your character
2. Try to catch some sheep(it adds health points)
3. Every move costs you health
4. If you catch all of the sheeps, you win (try not to starve)

