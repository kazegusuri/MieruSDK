/**********************************************************************/
/* MieruPC Project: Default Command 'pwd'        Arch Lab. TOKYO TECH */
/*  Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.       */
/*  All rights reserved.                                              */
/*  Licenced under the New BSD. See COPYING for detail.               */
/**********************************************************************/

#include <mierulib.h>

void usage(){
    lcd_print("\
pwd\n\
pwd prints current working direcotry\n\
");
}

int main(int argc, char *argv[]){
    int ret;
    char buf[512];
    lcd_init();
    lcd_setscrollmode(1);

    if(argc != 1){
        usage();
        return 0;
    }
    
    ret = sys_getcwd(buf, 512);
    if(ret < 0){
        lcd_printf("pwd error\n");
        return -1;
    }
    lcd_printf("%s\n", buf);
    return 0;
}
