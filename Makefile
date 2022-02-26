ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: eys.exe
	
eys.exe: eys.o
	gcc -o eys.exe eys.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

eys.o: eys.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c eys.c	
	
	
clean:
	del eys.o
	del eys.exe

