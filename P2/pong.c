#include <ncurses.h>
#include <unistd.h>

#define DELAY 8000

typedef struct {
    //definimos la estructura
    short int x, y, c; 
    bool horizontal, vertical;
} 
object;

int main() {
    //creamos obj pantalla
    object pantalla; 
    int i = 0, cont = 0; 
    int end = 0;
    
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    keypad(stdscr,true);
    noecho();
    curs_set(0);

    getmaxyx(stdscr,pantalla.y,pantalla.x);
    //creamos objetos ambos jugadores y pelota dentro de pantalla
    object jugador1 = {pantalla.x - 2, pantalla.y / 2, 0, 0, 0};
    object jugador2 = {1, pantalla.y / 2, 0, 0, 0};
    object pelota = {pantalla.x / 2, pantalla.y / 2, 0, 0, 0};

    mvprintw(0,0, "Jugador 1 controles. 's' hacia abajo, 'w' hacia arriba.\n"
                "Jugador 2 controles. Las flechas del teclado.\n"
                "Pulsa cualquier tecla para empezar, 'p' para pausar y 'esc' para salir.");

    getch();
    for (nodelay(stdscr,1); !end; usleep(DELAY)) {
        if (++cont % 16 == 0) {
        if ((pelota.y == pantalla.y - 1) || (pelota.y == 1))
            pelota.vertical =! pelota.vertical;
        if ((pelota.x >= pantalla.x - 2) || (pelota.x <= 2)) {
            pelota.horizontal =! pelota.horizontal;
            if ((pelota.y == jugador1.y - 1) || (pelota.y == jugador2.y - 1)) {
                pelota.vertical = false;
            } else if ((pelota.y == jugador1.y + 1) || (pelota.y == jugador2.y + 1)) {
            pelota.vertical = true;
            } else if ((pelota.y != jugador1.y) && (pelota.y != jugador2.y)) {
            (pelota.x >= pantalla.x - 2) ? jugador1.c++ : jugador2.c++;
            pelota.x = pantalla.x / 2;
            pelota.y = pantalla.y / 2;
            }
        }


        pelota.x = pelota.horizontal ? pelota.x + 1 : pelota.x - 1;
        pelota.y = pelota.vertical ? pelota.y + 1 : pelota.y - 1;

        if (jugador1.y <= 1)
            jugador1.y = pantalla.y - 2;
        if (jugador1.y >= pantalla.y - 1)
            jugador1.y = 2;
        if (jugador2.y <= 1)
            jugador2.y = pantalla.y - 2;
        if (jugador2.y >= pantalla.y - 1)
            jugador2.y = 2;
        }


        switch (getch()) {
        //teclas utilizadas
        case KEY_DOWN: jugador1.y++; break;
        case KEY_UP:   jugador1.y--; break;
        case 'w':      jugador2.y--; break;
        case 's':      jugador2.y++; break;
        case 'p':      getchar(); break;
        case 0x1B:    endwin(); end++; break;
        }


        erase();
        mvprintw(2, pantalla.x / 2 - 2,"%i | %i", jugador1.c, jugador2.c);
        mvvline(0, pantalla.x / 2, ACS_VLINE, pantalla.y);
        attron(COLOR_PAIR(1));
        mvprintw(pelota.y, pelota.x, "o");
        
        for(i = -1; i < 2; i++){
        mvprintw(jugador1.y + i, jugador1.x, "|");
        mvprintw(jugador2.y + i, jugador2.x, "|");}
        attroff(COLOR_PAIR(1));
    }
}
