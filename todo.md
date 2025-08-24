# TO-DO:
## Unfinished:
- Finish the GUI
- Add the *timer*
- Add the *bomb counter*
- Fix chording out of bounds
- Fix chording sometimes taking multiple clicks on the same chord to open up all the cells 
- Add detection for *wrong chords*
- Add the ability to *generate a new board with different parameters* (through keyboard input) (how does raylib handle having multiple windows open at once?)
- Add the ability to *change the style* of the minesweeper board. Dunno how raylib handles opening files through a window. Maybe it doesn't.
- Add the ability to *rescale* the window.

## Finished:
- Fix *checkBombsInRadius()* cause it doesn't work like 1% of the time - Idk why - It's a mysterious function

    ^^^ yeah no kidding %1 of the time - the problem had to do with exactly ONE specific cell. cell index 0 (the top-left-most cell, the first cell from the top left)

    the bomb counting checks failed when they should've succeeded when there was a bomb at location (0, 0)

    the reason? `>`s instead of `>=`s in the `TOP LEFT`, `TOP MIDDLE`, `TOP RIGHT`, `LEFT` and `RIGHT` bomb checks

- Add the ability to *restart the game* (through keyboard input)

    ^^^ simple addition

- Fix *prevCell* funkiness (I literally cannot take my mind off of this shitty little bug like UGHHHHH. I won't be able to work on chording or anything else until I fix this. it's too glaring of a problem)

    ^^^ FINALLY! The solution was simple.
        
    I moved 
    ```c
    if (board[prevCell].texture == CELL_BEING_CLICKED && prevCell != cellUnderMouse)
    {
        board[prevCell].texture = CELL_DEFAULT;
    }
    ```
    to the top of the mouse-board collision check. I also added the `&& prevCell != cellUnderMouse` part.

- Add the ability to *try again*, aka restart without rerandomizing the board (via keyboard input, of course)

    ^^^ (01.05.2025) simple addition
 
## Also
`SetWindowTitle()` is a pretty cool raylib function. I should use it to display information or smth. seems useful.