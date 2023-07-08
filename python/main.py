import pygame, sys, random, time
from drawing import *
pygame.init()

neighbor_offsets = [(-1,-1),(-1,0),(-1,1),(0,1),(1,1),(1,0),(1,-1),(0,-1)]

class Tile:
    mine = False
    covered = False
    value = 0
    flag = False
    
argc = len(sys.argv)

winW = 1000
winH = 1000
fieldW = 20
fieldH = 20
density = 12


if (argc > 1): fieldW = int(sys.argv[1])
if (argc > 2): fieldH = int(sys.argv[2])
if (argc > 3): density = int(sys.argv[3])

fieldX = (winW - tileSize*fieldW)//2
fieldY = 100

field = []
for x in range(fieldW): field.append([None]*fieldH)

root = pygame.display.set_mode((winW,winH))

pygame.font.init()
font = pygame.font.SysFont('Comic Sans MS', 20)
#text = font.render(f'Height: {fieldH} Width: {fieldW} Density: {density}%',True,(255,255,0))

clock = pygame.time.Clock()

def draw_tile(x, y, tile):
    if tile.covered:
        if tile.flag: surf = flagged_tile
        else: surf = covered_tile
    else:
        surf = n_tiles[tile.value]
    
    root.blit(surf,(x,y))
    
def check_tile(x,y):
    return 0 <= x < fieldW and 0 <= y < fieldH

def gen_field():
    for y in range(fieldH):
        for x in range(fieldW):
            tile = Tile()
            tile.mine = density > random.randint(0,99)
            tile.covered = True
            field[x][y] = tile 

def get_neighbors(x,y):
    arr = []
    for xo,yo in neighbor_offsets:
        x1,y1 = x+xo,y+yo
        if check_tile(x1,y1):
            arr.append((x1,y1))
    return arr
    
def reveal(x,y):
    tile = field[x][y]

    if not tile.covered: return

    tile.covered = False

    if tile.mine:
        print("YOU LOST")
        exit()

    neighs = get_neighbors(x,y)

    for x1,y1 in neighs:
        if field[x1][y1].mine: tile.value += 1

    # xp,yp = x*tileSize+fieldX, y*tileSize+fieldY
    # draw_tile(xp,yp,tile)
    # pygame.display.update()
    # time.sleep(.05)

    if tile.value == 0:
        for x1,y1 in neighs:
            reveal(x1,y1)

def quick_mark(x,y):
    neighs = get_neighbors(x,y)
    
    c = 0
    for x1,y1 in neighs:
        c += field[x1][y1].covered

    if c <= field[x][y].value:
        for x1,y1 in neighs:
            neigh = field[x1][y1]
            if neigh.covered:
                neigh.flag = True


def quick_reveal(x,y):
    neighs = get_neighbors(x,y)
    
    c = 0
    for x1,y1 in neighs:
        c += field[x1][y1].flag

    if c >= field[x][y].value:
        for x1,y1 in neighs:
            neigh = field[x1][y1]
            if neigh.covered and not neigh.flag:
                reveal(x1,y1)

def left_click(x,y):
    if not check_tile(x,y): return
    tile = field[x][y]
    if tile.flag: return
    if tile.covered: reveal(x,y)
    else: quick_reveal(x,y)

def right_click(x,y):
    if not check_tile(x,y): return
    tile = field[x][y]

    if tile.covered: tile.flag ^= 1
    else: quick_mark(x,y)
    
gen_field()

while True:
    dt = clock.tick(60)/1000
    mouse = pygame.mouse.get_pos()
    selectX = (mouse[0]-fieldX)//tileSize
    selectY = (mouse[1]-fieldY)//tileSize

    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                left_click(selectX,selectY)
            elif event.button == 3:
                right_click(selectX,selectY)
    
    root.fill((255,255,255))
    for x in range(fieldW):
        for y in range(fieldH):
            xp,yp = x*tileSize+fieldX, y*tileSize+fieldY
            draw_tile(xp,yp,field[x][y])
            
    #root.blit(text,(200,100))
    pygame.display.update()