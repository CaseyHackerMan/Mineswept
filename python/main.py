import pygame, sys, random, time
from graphics import *

class Tile:
    mine = False
    covered = False
    flag = False
    value = 0

class Game:
    def __init__(self, tiles):
        self.tile_count = tiles
        self.reset()

    def reset(self):
        self.correct_flags = 0
        self.covered_tiles = self.tile_count
        self.mines = 0
        self.flags = 0
        self.playing = True
        self.won = False
        self.timer = 0
        self.reset_time()
        
    def reset_time(self):
        self.start_time = time.time()

    def win(self):
        self.playing = False
        self.won = True
    
    def lose(self):
        self.playing = False
        self.won = False

    def get_time(self):
        if self.playing:
            self.timer = time.time()-self.start_time
        return self.timer
    
argc = len(sys.argv)
fieldW = int(sys.argv[1]) if (argc > 1) else 20
fieldH = int(sys.argv[2]) if (argc > 2) else 20
density = int(sys.argv[3]) if (argc > 3) else 15
 
winW = fieldW*tileSize
winH = fieldH*tileSize+bannerHeight

field = [[Tile() for _ in range(fieldH)] for _ in range(fieldW)]

game = Game(fieldW*fieldH)

pygame.init()
pygame.font.init()
clock = pygame.time.Clock()
font = pygame.font.SysFont('Comic Sans MS', 15)
large_font = pygame.font.SysFont('Comic Sans MS', 25)
root = pygame.display.set_mode((winW,winH))

def draw_tile(x,y):
    global root
    tile = field[x][y]
    if tile.covered:
        if tile.flag: surf = flagged_tile
        else: surf = covered_tile
    else:
        if tile.mine: surf = mine_tile
        else: surf = n_tiles[tile.value]
    
    root.blit(surf,(x*tileSize,y*tileSize+bannerHeight))
    pygame.display.update()
    time.sleep(0.005)
    
def check_tile(x,y):
    return 0 <= x < fieldW and 0 <= y < fieldH

def gen_field():
    global game
    draw_banner()
    game.reset()
    for y in range(fieldH):
        for x in range(fieldW):
            tile = field[x][y]
            redraw_f = tile.flag or not tile.covered
            tile.mine = density > random.randint(0,99)
            game.mines += tile.mine
            tile.covered = True
            tile.value = 0
            tile.flag = False
            if redraw_f: draw_tile(x,y)
    game.reset_time()

def draw_banner():
    global game, root
    banner_rect = pygame.Rect((0,0),(winW,bannerHeight))
    pygame.draw.rect(root,LIGHT_GREY,banner_rect, 0, 6)
    pygame.draw.rect(root,BLACK,banner_rect, 2, 6)

    t = round(game.get_time(),2)
    time_text = font.render(f'Time: {t}',True,BLACK)

    m = game.mines-game.flags
    mines_text = font.render(f'Mines: {m}', True, BLACK if (game.flags <= game.mines) else RED) 
    
    root.blit(time_text,(10,1))
    root.blit(mines_text,(10,26))

    if not game.playing:
        if game.won: text = large_font.render("YUHHH", True, GREEN)
        else: text = large_font.render("NAHHH", True, RED)
        pos = div_vec(sub_vec(banner_rect.size,text.get_size()),2)
        root.blit(text, pos)
        
neighbor_offsets = [(-1,-1),(-1,0),(-1,1),(0,1),(1,1),(1,0),(1,-1),(0,-1)]
random.shuffle(neighbor_offsets)
def get_neighbors(x,y):
    arr = []
    for xo,yo in neighbor_offsets:
        x1,y1 = x+xo,y+yo
        if check_tile(x1,y1):
            arr.append((x1,y1))
    return arr
    
def reveal(x,y):
    global game
    tile = field[x][y]
    if not tile.covered: return

    tile.covered = False
    game.covered_tiles -= 1

    neighs = get_neighbors(x,y)
    for x1,y1 in neighs:
        if field[x1][y1].mine: tile.value += 1

    draw_tile(x,y)

    if tile.mine:
        game.lose()
        return
    elif game.covered_tiles == game.mines:
        game.win()

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
    global game
    tile = field[x][y]
    tile.flag = True
    game.flags += 1
    if tile.mine:
        game.correct_flags += 1
        all_mines_flagged = game.correct_flags == game.mines
        all_flags_correct = game.flags == game.correct_flags
        if all_mines_flagged and all_flags_correct:
            game.win()
    draw_tile(x,y)

def unflag(x,y):
    tile = field[x][y]
    tile.flag = False
    game.flags -= 1
    if tile.mine: game.correct_flags -= 1
    else:
        all_mines_flagged = game.correct_flags == game.mines
        all_flags_correct = game.flags == game.correct_flags
        if all_mines_flagged and all_flags_correct:
            game.win()
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

root.fill((255,255,255))
gen_field()

while True:
    dt = clock.tick(30)/1000
    mouse = pygame.mouse.get_pos()

    selection = div_vec(sub_vec(mouse,field_pos),tileSize)

    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()
        if event.type == pygame.MOUSEBUTTONDOWN and game.playing:
            if event.button == 1:
                left_click(*selection)
            elif event.button == 3:
                right_click(*selection)
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                gen_field()

    draw_banner()
    pygame.display.update()
        