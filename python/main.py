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

fieldW = 20
fieldH = 20
density = 15

if (argc > 1): fieldW = int(sys.argv[1])
if (argc > 2): fieldH = int(sys.argv[2])
if (argc > 3): density = int(sys.argv[3])

winW = fieldW*tileSize
winH = fieldH*tileSize+50

field = []
# for x in range(fieldW): field.append([None]*fieldH)

correct_flags = 0
covered_tiles = fieldH*fieldW
mines = 0
flags = 0
playing = True
start_time = None
won = False

for x in range(fieldW):
    field.append([Tile() for y in range(fieldH)])

root = pygame.display.set_mode((winW,winH))

pygame.font.init()
font = pygame.font.SysFont('Comic Sans MS', 15)
large_font = pygame.font.SysFont('Comic Sans MS', 25)

clock = pygame.time.Clock()

def draw_tile(x,y):
    tile = field[x][y]
    if tile.covered:
        if tile.flag: surf = flagged_tile
        else: surf = covered_tile
    else:
        if tile.mine: surf = mine_tile
        else: surf = n_tiles[tile.value]
    
    root.blit(surf,(x*tileSize,y*tileSize+50))
    pygame.display.update()
    
def check_tile(x,y):
    return 0 <= x < fieldW and 0 <= y < fieldH

def gen_field():
    global correct_flags, covered_tiles, mines, start_time, playing, flags
    correct_flags = 0
    covered_tiles = fieldH*fieldW
    mines = 0
    flags = 0
    playing = True
    draw_banner()
    for y in range(fieldH):
        for x in range(fieldW):
            tile = field[x][y]
            tile.mine = density > random.randint(0,99)
            mines += tile.mine
            tile.covered = True
            tile.value = 0
            tile.flag = False
            draw_tile(x,y)
    start_time = time.time()

def draw_banner():
    global start_time, flags, mines
    banner_rect = pygame.Rect((0,0),(winW,50))
    
    if playing:
        if start_time is not None:
            t = round(time.time()-start_time,2)
            time_text = font.render(f'Time: {t}',True,BLACK)
        else:
            time_text = font.render(f'Time:',True,BLACK) 

        if flags is not None:
            m = mines-flags
            mines_text = font.render(f'Mines: {m}', True, BLACK if (flags <= mines) else RED) 
        else:
            mines_text = font.render(f'Mines:', True, BLACK) 
        pygame.draw.rect(root,LIGHT_GREY,banner_rect, 0, 6)
        pygame.draw.rect(root,BLACK,banner_rect, 2, 6)
        root.blit(time_text,(10,1))
        root.blit(mines_text,(10,26))
    else:
        if won: text = large_font.render("YUHHH", True, GREEN)
        else: text = large_font.render("NAHHH", True, RED)
        pos = div_vec(sub_vec(banner_rect.size,text.get_size()),2)
        root.blit(text, pos)
        
def get_neighbors(x,y):
    arr = []
    for xo,yo in neighbor_offsets:
        x1,y1 = x+xo,y+yo
        if check_tile(x1,y1):
            arr.append((x1,y1))
    return arr
    
def reveal(x,y):
    global covered_tiles
    tile = field[x][y]

    if not tile.covered: return

    tile.covered = False
    covered_tiles -= 1

    neighs = get_neighbors(x,y)
    for x1,y1 in neighs:
        if field[x1][y1].mine: tile.value += 1

    draw_tile(x,y)

    if tile.mine:
        lose()
        return
    elif covered_tiles == mines:
        win()

    if tile.value == 0:
        for x1,y1 in neighs:
            reveal(x1,y1)

def quick_flag(x,y):
    neighs = get_neighbors(x,y)
    
    c = 0
    for x1,y1 in neighs:
        c += field[x1][y1].covered

    if c <= field[x][y].value:
        for x1,y1 in neighs:
            tile = field[x1][y1]
            if tile.covered and not tile.flag:
                flag(x1,y1)

def flag(x,y):
    global correct_flags, flags
    tile = field[x][y]
    tile.flag = True
    flags += 1
    if tile.mine:
        correct_flags += 1
        if correct_flags == mines and flags == correct_flags:
            win()
    draw_tile(x,y)

def unflag(x,y):
    global correct_flags, flags
    tile = field[x][y]
    tile.flag = False
    flags -= 1
    if tile.mine: correct_flags -= 1
    elif correct_flags == mines and flags == correct_flags:
        win()
    
    draw_tile(x,y)


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

    if tile.covered:
        if tile.flag: unflag(x,y)
        else: flag(x,y)
    else: quick_flag(x,y)

def win():
    global playing, won
    won = True
    playing = False

def lose():
    global playing, won
    won = False
    playing = False
    
root.fill((255,255,255))
gen_field()

while True:
    dt = clock.tick(30)/1000
    mouse = pygame.mouse.get_pos()
    selectX = mouse[0]//tileSize
    selectY = (mouse[1]-50)//tileSize

    if playing:
        for event in pygame.event.get():
            if event.type == pygame.QUIT: sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    left_click(selectX,selectY)
                elif event.button == 3:
                    right_click(selectX,selectY)
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    gen_field()
                    
    else:
        for event in pygame.event.get():
            if event.type == pygame.QUIT: sys.exit()
            if event.type == pygame.KEYDOWN:
                gen_field()
    draw_banner()
    pygame.display.update()
        