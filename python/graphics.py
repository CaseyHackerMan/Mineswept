import pygame, sys

tileSize = 24
bannerHeight = 50
field_pos = (0,bannerHeight)
WHITE = pygame.Color("white")
PINK = pygame.Color("fuchsia")
RED = pygame.Color("red")
YELLOW = pygame.Color("yellow")
GREEN = pygame.Color("darkgreen")
CYAN = pygame.Color("cyan4")
BLUE = pygame.Color("blue")
GREY = pygame.Color("grey60")
DARK_GREEN = pygame.Color("darkgreen")
DARK_BLUE = pygame.Color("darkblue")
DARK_RED = pygame.Color("darkred")
LIGHT_GREY = pygame.Color("grey85")
DARK_GREY = pygame.Color("grey40")
BLACK = pygame.Color("black")

num_colors = [BLUE, DARK_GREEN, RED, DARK_BLUE, DARK_RED, CYAN, BLACK, DARK_GREY, PINK]

def add_vec(v1,v2): return (v1[0]+v2[0],v1[1]+v2[1])
def sub_vec(v1,v2): return (v1[0]-v2[0],v1[1]-v2[1])
def mul_vec(v,x): return (v[0]*x,v[1]*x)
def div_vec(v,x): return (v[0]//x,v[1]//x)

pygame.font.init()
font = pygame.font.SysFont('Comic Sans MS', 20)

empty_tile = pygame.Surface((tileSize,tileSize))
empty_tile.fill(LIGHT_GREY)
pygame.draw.rect(empty_tile,BLACK,empty_tile.get_rect(),1)

covered_tile = pygame.Surface((tileSize,tileSize))
covered_tile.fill(GREY)
pygame.draw.rect(covered_tile,DARK_GREY,covered_tile.get_rect(),3)
pygame.draw.rect(covered_tile,BLACK,covered_tile.get_rect(),1)

flagged_tile = pygame.Surface.copy(covered_tile)
pygame.draw.polygon(flagged_tile, RED, [(8,4),(17,8),(8,12)])
pygame.draw.line(flagged_tile, BLACK, (8,4), (8,19))

smiley_tile = pygame.Surface.copy(covered_tile)
pygame.draw.circle(smiley_tile, YELLOW, (12,12), 9)
pygame.draw.circle(smiley_tile, BLACK, (9,10), 1)
pygame.draw.circle(smiley_tile, BLACK, (15,10), 1)
pygame.draw.lines(smiley_tile, BLACK, 0, [(7,14),(10,16),(13,16),(16,14)])

frowny_tile = pygame.Surface.copy(covered_tile)
pygame.draw.circle(frowny_tile, YELLOW, (12,12), 9)
pygame.draw.line(frowny_tile, BLACK, (7,8),(9,10))
pygame.draw.line(frowny_tile, BLACK, (7,10),(9,8))
pygame.draw.line(frowny_tile, BLACK, (14,8),(16,10))
pygame.draw.line(frowny_tile, BLACK, (14,10),(16,8))
pygame.draw.circle(frowny_tile, BLACK, (12,16), 2)

mine_tile = pygame.Surface.copy(empty_tile)
pygame.draw.line(mine_tile, BLACK, (5,18), (18,5), 3)
pygame.draw.line(mine_tile, BLACK, (5,5), (18,18), 3)

n_tiles = [empty_tile]
for i in range(9):
    n_surf = font.render(str(i+1), True, num_colors[i])
    tile_surf = pygame.Surface.copy(empty_tile)
    tile_surf.blit(n_surf,div_vec(sub_vec((tileSize,tileSize-4),n_surf.get_size()),2))
    n_tiles.append(tile_surf)

if __name__ == "__main__":
    pygame.image.save(empty_tile, "assets/empty_tile.bmp")
    pygame.image.save(covered_tile, "assets/covered_tile.bmp")
    pygame.image.save(mine_tile, "assets/mine_tile.bmp")
    pygame.image.save(flagged_tile, "assets/flagged_tile.bmp")
    pygame.image.save(n_tiles[1], "assets/one_tile.bmp")
    pygame.image.save(n_tiles[2], "assets/two_tile.bmp")
    pygame.image.save(n_tiles[3], "assets/three_tile.bmp")
    pygame.image.save(n_tiles[4], "assets/four_tile.bmp")
    pygame.image.save(n_tiles[5], "assets/five_tile.bmp")
    pygame.image.save(n_tiles[6], "assets/six_tile.bmp")
    pygame.image.save(n_tiles[7], "assets/seven_tile.bmp")
    pygame.image.save(n_tiles[8], "assets/eight_tile.bmp")
    pygame.image.save(n_tiles[9], "assets/nine_tile.bmp")

    root = pygame.display.set_mode((500,100))
    root.fill(WHITE)

    x = 10
    root.blit(smiley_tile, (x,0)); x += 30
    root.blit(frowny_tile, (x,0)); x += 30
    root.blit(covered_tile, (x,0)); x += 30
    root.blit(flagged_tile, (x,0)); x += 30
    root.blit(mine_tile, (x,0)); x += 30
    for tile in n_tiles:
        root.blit(tile, (x,0)); x += 30

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT: sys.exit()
        pygame.display.update()