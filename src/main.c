#include <Render.h>
#include <ncurses.h>

int main() {
    Context *context = contextInit();
    while (true) {
        if (context->update) {
            refresh();
        }
    }
    return 0;
}
