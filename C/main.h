typedef struct tile {
    char mine    : 1;
    char covered : 1;
    char flag    : 1;
    char value   : 4;
} Tile;


