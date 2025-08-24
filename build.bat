gcc minesweeper.c minesweeper.h mswpUI.c mswpUI.h -lraylib -lgdi32 -lwinmm
:: -o minesweeper
:: -Wl,--subsystem,windows (disables popup command line window)