#include <Render.h>
#include <Input.h>
#include <ncurses.h>

#ifdef _WINDOWS
#include <windows.h>
#else

#include <unistd.h>

#define Sleep(x) usleep(x)
#endif


int main() {
    Context *context = contextInit();
    drawBoard(context);

    while (true) {
        handleInput(context);
        if (context->gameState->update) {
            drawBoard(context);
            context->gameState->update = false;
        }
        if (context->windowInfo->update) {
            wrefresh(context->windowInfo->handle);
            context->windowInfo->update = false;
        }
        if (context->boardWindowInfo->update) {
            wrefresh(context->boardWindowInfo->handle);
            context->boardWindowInfo->update = false;
        }
        if (context->gameState->sleep > 0) {
            Sleep(context->gameState->sleep);
            context->gameState->sleep = 0;
        }
    }
    return 0;
}
