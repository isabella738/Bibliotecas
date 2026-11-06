/*
    Keyboard input functions read a character without pressing enter.

    For LINUX: call change_terminal in the beggining of function main and restore terminal at the
    end. To read input without blocking the program, paste:
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK)
    in function main.

    For WINDOWS, _getch() read input without enter e _khbit read without blocking the program.
*/

#include "keyboard_input.h"
#include <stdlib.h>

#if defined (LINUX)
    #include <termios.h>
    #include <unistd.h>

    struct termios velho_terminal, novo_terminal;
    void change_terminal(){
        tcgetattr(STDIN_FILENO, &velho_terminal);
        novo_terminal = velho_terminal;

        novo_terminal.c_lflag &= ~(ICANON | ECHO);

        tcsetattr(STDIN_FILENO, TCSANOW, &novo_terminal);
    }
    void restore_terminal(){
        tcsetattr(STDIN_FILENO, TCSANOW, &velho_terminal);
    }
#elif defined (WINDOWS)
    #include <conio.h>
    #include <windows.h>
#endif 

int get_command(char *comando){
    #if defined(WINDOWS)
        if(_kbhit()) {
            (*comando) = _getch();
            return 1;
        }
    #elif defined(LINUX)
        char c;
        if(read(STDIN_FILENO, &c, 1) > 0){ 
            (*comando) = c;
            return 1;
        }
    #endif
    return 0;
}

void clear_screen(){
    #if defined(LINUX)
        system("clear");
    #else
        system("cls");
    #endif
}

int sleep_ms(int ms){
    #if defined(LINUX)
        usleep(1000*ms);
    #else
        Sleep(ms);
    #endif
}
