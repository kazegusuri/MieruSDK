/**********************************************************************/
/* MieruPC Project: Default Command 'cat'        Arch Lab. TOKYO TECH */
/*  Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.       */
/*  All rights reserved.                                              */
/*  Licenced under the New BSD. See COPYING for detail.               */
/**********************************************************************/

#include <mierulib.h>

void usage(){
    lcd_print("\
cat [filename]\n\
");
}

int main(int argc, char *argv[]){
    int fd, ret;
    char buf[128];

    lcd_init();
    lcd_setscrollmode(1);

    if(argc != 2){
        usage();
        return 1;
    }

    fd = sys_open(argv[1], O_RDONLY);
    if(fd < 0){
        lcd_printf("can't open file (%s)\n",argv[1]);
        return 1;
    }
    
    for(;;){ 
        ret = sys_read(fd, buf, 128);
        if(ret <= 0) break;
        buf[ret] = 0;
        lcd_print(buf);
    }

    sys_close(fd);
    return 0;
}
