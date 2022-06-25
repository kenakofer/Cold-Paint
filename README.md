# Cold Paint

A college final project in C, this 1-2 player survival arcade game packs some depth for its minimal SDL graphics. Prepare for bombs, missiles, metal, wood, powerups, adaptive difficulty, and a slow motion effect for moments of badassery.

https://user-images.githubusercontent.com/6500636/175749979-50a41df4-43a0-446c-867f-c10d0e4c2e18.mp4


Cold Paint is written in C with SDL2. As a sentient paint blob, your goal is to touch as many objects as possible, painting them your color. When the item is destroyed, points are given to whichever paint blob has it colored. However, the paint blobs are vulnerable to the pool of sodium hydroxide (a caustic paint remover).

Control the white blob with the arrow keys, and the blue blob with WASD.

Building:

```
# Install SDL2 and GSL:
sudo apt install libsdl2-dev libgsl-dev
# Run the makefile
make
```


Run with:

```
./cold_paint
```
