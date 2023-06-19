import pygame, sys
pygame.init()

GREY = (200,200,200)
DARK_GREY = (150,150,150)
BLACK = (0,0,0)

argc = len(sys.argv)

winW = 1000
winH = 1000
fieldW = 20
fieldH = 15
density = 10
tileSize = 25

if (argc > 1): fieldW = int(sys.argv[1])
if (argc > 2): fieldH = int(sys.argv[2])
if (argc > 3): density = int(sys.argv[3])

fieldX = (winW - tileSize*fieldW)//2
fieldY = 100

root = pygame.display.set_mode((winW,winH))

r1 = pygame.Rect(20,10,100,200)

pygame.font.init()
font = pygame.font.SysFont('Comic Sans MS', 20)
#text = font.render(f'Height: {fieldH} Width: {fieldW} Density: {density}%',True,(255,255,0))

clock = pygame.time.Clock()

def draw_tile(tile_rect):
    pygame.draw.rect(root, GREY, tile_rect)
    pygame.draw.rect(root, DARK_GREY, tile_rect, 3)
    pygame.draw.rect(root, BLACK, tile_rect, 1)

while True:
    dt = clock.tick(60)/1000

    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()
        if event.type == pygame.MOUSEBUTTONDOWN:
            pass

    root.fill((255,255,255))
    for x in range(fieldW):
        for y in range(fieldH):
            tile = pygame.Rect(x*tileSize+fieldX,y*tileSize+fieldY,tileSize,tileSize)
            draw_tile(tile)
            
    #root.blit(text,(200,100))
    pygame.display.update()