import pygame, sys

tileSize = 25
WHITE = pygame.Color("white")
BLUE = pygame.Color("blue")
GREY = pygame.Color("grey60")
BLACK = pygame.Color("black")
RED = pygame.Color("red")
CYAN = pygame.Color("cyan4")
PINK = pygame.Color("fuchsia")
DARK_GREEN = pygame.Color("darkgreen")
DARK_BLUE = pygame.Color("darkblue")
DARK_RED = pygame.Color("darkred")
LIGHT_GREY = pygame.Color("grey85")
DARK_GREY = pygame.Color("grey40")
YELLOW = pygame.Color("yellow")

num_colors = [BLUE, DARK_GREEN, RED, DARK_BLUE, DARK_RED, CYAN, BLACK, DARK_GREY, PINK]

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
pygame.draw.polygon(flagged_tile, RED, [(8,5),(17,9),(8,13)])
pygame.draw.line(flagged_tile, BLACK, (8,5), (8,20))

smiley_tile = pygame.Surface.copy(covered_tile)
pygame.draw.circle(smiley_tile, YELLOW, (12.5,12.5), 9)
pygame.draw.circle(smiley_tile, BLACK, (9,10), 1)
pygame.draw.circle(smiley_tile, BLACK, (16,10), 1)
# pygame.draw.arc(smiley_tile, BLACK, (16,10), 1)

n_tiles = [empty_tile]

for i in range(9):
    n_surf = font.render(str(i+1), True, num_colors[i])
    tile_surf = pygame.Surface.copy(empty_tile)
    tile_surf.blit(n_surf,((tileSize-n_surf.get_width())//2, (tileSize-2-n_surf.get_height())//2))
    n_tiles.append(tile_surf)

if __name__ == "__main__":
    root = pygame.display.set_mode((500,100))
    root.fill(WHITE)

    x = 10
    root.blit(smiley_tile, (x,0))
    x += 30
    root.blit(covered_tile, (x,0))
    x += 30
    root.blit(flagged_tile, (x,0))
    x += 30
    for tile in n_tiles:
        root.blit(tile, (x,0))
        x += 30

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT: sys.exit()
        pygame.display.update()