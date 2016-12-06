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

typedef struct snek_t {
    directions direction;
    point body[10];
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
    if((*snek).tail_index % 10 == 0)
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

int main(int argc, char **argv)
{

    //variable declarations
    int x = 0, y = 0;
    int max_y = 0, max_x = 0;

    snek_t snek;
    snek.direction = DOWN;
    snek.head_index = 9;
    snek.tail_index = 0;
    
    for(int i = 0 ; i < 10 ; i++)
    {
        snek.body[i].x = 10+i; 
        snek.body[i].y = 10; 
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

        for(int i = 0 ; i < 10 ; i++)
        {
            mvprintw(snek.body[i].y, snek.body[i].x, "o"); 
        }

        refresh();

        usleep(DELAY);
    }

    terminate_snek();
    return 0;
}
