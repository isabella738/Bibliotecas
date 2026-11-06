#ifndef TEXT_EDIT
#define TEXT_EDIT

#define NEGRITO "\033[1m"
#define TRANSLUCIDO "\033[2m"
#define ITALICO "\033[3m"
#define SUBLINHADO "\033[4m"
#define FUNDO_BRANCO "\033[7m"
#define RASURADO "\033[8m"
#define SUB_DUPLO "\033[21m"

#define GRAY "\033[90m"
#define RED "\033[91m"
#define GREEN  "\033[92m"
#define YELLOW "\033[93m"
#define BLUE "\033[94m"
#define MAGENTA "\033[95m"
#define CYAN "\033[96m"
#define WHITE "\033[97m"

#define BG_RED "\033[41m"
#define BG_GREEN  "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

#define RESET  "\033[0m"

const char color[][10] = {
    GRAY,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

const char background[][10] = {
    BG_RED,
    BG_GREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGENTA,
    BG_CYAN,
    BG_WHITE
};

#endif