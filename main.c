#include <curses.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define DELAY 60000

bool game_running = true;

typedef enum {UP, DOWN, LEFT, RIGHT} directions;


typedef struct point {
    int x;
    int y;
} point;

typedef struct case_t   {
    point coords;
    bool taken;
} case_t;

typedef struct territory_t  {
    case_t cases[10][10];
} territory_t;

typedef struct snek_t {
    directions direction;
    point body[50];
    int length;
    int head_index;
    int tail_index;
} snek_t;

void terminate_snek()
{
    endwin();
}

void move_snek(directions *direction)
{
    int c = wgetch(stdscr);

    switch(c)
    {
        case KEY_UP:
            if(*direction != DOWN)
                *direction = UP;
            break;
        case KEY_LEFT:
            if(*direction != RIGHT)
            *direction = LEFT;
            break;
        case KEY_DOWN:
            if(*direction != UP)
            *direction = DOWN;
            break;
        case KEY_RIGHT:
            if(*direction != LEFT)
            *direction = RIGHT;
            break;
        case 'q':
            game_running = false;
            break;
        default:
            break;
    }
}

void forward(snek_t* snek)
{
    if((*snek).tail_index % snek->length == 0)
    {
        (*snek).tail_index = 0;
    }

    switch((*snek).direction)
    {
        case UP:
            (*snek).body[(*snek).tail_index].x = (*snek).body[(*snek).head_index].x;
            (*snek).body[(*snek).tail_index].y = (*snek).body[(*snek).head_index].y - 1;
            (*snek).head_index = (*snek).tail_index++;
            break;
        case DOWN:
            (*snek).body[(*snek).tail_index].x = (*snek).body[(*snek).head_index].x;
            (*snek).body[(*snek).tail_index].y = (*snek).body[(*snek).head_index].y + 1;
            (*snek).head_index = (*snek).tail_index++;
            break;
        case LEFT:
            (*snek).body[(*snek).tail_index].x = (*snek).body[(*snek).head_index].x - 1;
            (*snek).body[(*snek).tail_index].y = (*snek).body[(*snek).head_index].y;
            (*snek).head_index = (*snek).tail_index++;
            break;
        case RIGHT:
            (*snek).body[(*snek).tail_index].x = (*snek).body[(*snek).head_index].x + 1;
            (*snek).body[(*snek).tail_index].y = (*snek).body[(*snek).head_index].y;
            (*snek).head_index = (*snek).tail_index++;
            break;
        default:
            break;
    }
}

void display_territory(territory_t* territory)
{
    for(int i = 0 ; i < 10 ; i++)
    {
        for(int j = 0 ; j < 10 ; j++)
            mvprintw(territory->cases[i][j].coords.y, territory->cases[i][j].coords.x, "O");
    }
}

void display_snek(snek_t* snek)
{
    for(int i = 0 ; i < snek->length; i++)
    {
        mvprintw((*snek).body[i].y, (*snek).body[i].x, "o"); 
    }
}

void init_snek(snek_t* snek)
{
    snek->direction = DOWN;
    snek->length = 20;
    snek->head_index = snek->length-1;
    snek->tail_index = 0;
    
    for(int i = 0 ; i < snek->length ; i++)
    {
        snek->body[i].x = 10+i; 
        snek->body[i].y = 10; 
    }
}

int main(int argc, char **argv)
{

    //variable declarations
    int x = 0, y = 0;
    int max_y = 0, max_x = 0;

    snek_t snek;
    init_snek(&snek);

    territory_t first_land;
    //init_territory();
    
    for(int i = 0 ; i < 10 ; i++)
    {
        for(int j = 0 ; j < 10 ; j++)
        {
            first_land.cases[i][j].coords.x = 30+j;
            first_land.cases[i][j].coords.y = 30+i;
            first_land.cases[i][j].taken = true;

        }
    }

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(30);   
    mvprintw(0, 0, "Use arrow keys to navigate");
    refresh();

    while(game_running != false)
    {
        //handle player's moves
        move_snek(&(snek.direction));

        getmaxyx(stdscr, max_y, max_x);
        clear();

        forward(&snek);

        display_territory(&first_land);
        display_snek(&snek);

        refresh();

        usleep(DELAY);
    }

    terminate_snek();
    return 0;
}
