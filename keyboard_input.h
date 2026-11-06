/*
    Define o sistema operacional usado e contém cabeçalhos das funções que dependem do SO
*/
#ifndef KEYBOARD_INPUT
#define KEYBOARD_INPUT

#if defined(_WIN32) || defined(_WIN64)
    #define WINDOWS
#elif defined(__linux__)
    #define LINUX
#endif

#if defined(LINUX)
    void change_terminal(); //read a character without pressing enter
    void restore_terminal();
#endif

int get_command(char *comando);
void clear_screen();
int sleep_ms(int ms);//time in milisseconds

#endif