#ifndef TEXT_EDIT
#define TEXT_EDIT

#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define UNDERLINED "\033[4m"
#define CROSSEDOUT "\033[8m"
#define DOUBLE_UNDRLN "\033[21m"

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

extern const char colors[][10];

extern const char background[][10];

#endif