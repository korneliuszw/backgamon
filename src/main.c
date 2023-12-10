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
    Ctx *Ctx = CtxInit();
    drwb(Ctx);
    while (true) {
        handleInput(Ctx);
        if (Ctx->gs->update) {
            drwb(Ctx);
            Ctx->gs->update = false;
        }
        if (Ctx->wminf->update) {
            wrefresh(Ctx->wminf->handle);
            Ctx->wminf->update = false;
        }
        if (Ctx->wbinf->update) {
            wrefresh(Ctx->wbinf->handle);
            Ctx->wbinf->update = false;
        }
    }
    return 0;
}
