// Mierulib Templete. This file is treated as public-domain.
// You can use this templete freely to develop your own application.

#include <mierulib.h>

int main(void){
    lcd_init();
    lcd_cls();
    lcd_setscrollmode(1);

    lcd_printf("Hello, templete!\n");
    lcd_printf("templete version %s\n", MLIB_VERSION);
    lcd_printf("mpc_getchar test. Type your keyboard.\n");
    
    uchar key;
    for(;;){
        key = mpc_getchar();

        if(key != 0){
            if(key == MKS_Enter)
                lcd_nextline();
            else
                lcd_printf("[%c,%02x]",key,key);

            if(key == 'q')
                break;
        }
        
        mpc_usleep(1000);
    }
    
    // better to call this functions
    lcd_print("press Enter key to exit");
    mpc_waitkey();
    lcd_cls();
    return 0;
}
