import pygame, sys, random
from drawing import *
pygame.init()

neighbors = [(-1,-1),(-1,0),(-1,1),(0,1),(1,1),(1,0),(1,-1),(0,-1)]

class Tile:
    mine = False
    covered = False
    value = 0
    flag = False
    
argc = len(sys.argv)

winW = 1000
winH = 1000
fieldW = 20
fieldH = 15
density = 10


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
    
def get_tile(x,y):
    if 0 <= x < fieldW and 0 <= y < fieldH:
        return field[x][y]
    return None

def gen_field():
    for y in range(fieldH):
        for x in range(fieldW):
            tile = Tile()
            tile.mine = density > random.randint(0,99)
            tile.covered = True
            field[x][y] = tile

def mark(x,y):
    tile = get_tile(x,y)
    if not tile: return
    tile.flag ^= 1

def reveal(x,y):
    tile = get_tile(x,y)
    if not (tile and tile.covered): return

    tile.covered = False

    if tile.mine:
        print("YOU LOST")
        exit()

    for xo,yo in neighbors:
        x1,y1 = x+xo,y+yo
        neigh = get_tile(x1,y1)
        if neigh:
            tile.value += neigh.mine
            if tile.value == 0: reveal(x1,y1)
    
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
                reveal(selectX,selectY)
            elif event.button == 3:
                mark(selectX,selectY)
    
    root.fill((255,255,255))
    for x in range(fieldW):
        for y in range(fieldH):
            xp,yp = x*tileSize+fieldX, y*tileSize+fieldY
            draw_tile(xp,yp,field[x][y])
            
    #root.blit(text,(200,100))
    pygame.display.update()