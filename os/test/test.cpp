#include <test.h>
#include <mierulib.h>

#ifdef TEST_MODE

void _TEST_TRUE(bool x, const char *varx, const char *fname, int line) {
    if (!(x == true)) {
        lcd_dprintf("NG:%s:%d: %s\n", fname, line, varx);
    }
}

void _TEST_FALSE(bool x, const char *varx, const char *fname, int line) {
    if (!(x == false)) {
        lcd_dprintf("NG:%s:%d: %s\n", fname, line, varx);
    }
}

#endif

