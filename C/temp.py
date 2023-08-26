import pygame

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

for _ in list(globals()):
    obj = globals()[_]
    if isinstance(obj, pygame.Color):
        print(f'#define {_} (SDL_Color) {"{"+str(obj)[1:-1]+"}"}')
