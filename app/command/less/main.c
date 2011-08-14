/**********************************************************************/
/* MieruPC Project: Default Command 'less'       Arch Lab. TOKYO TECH */
/*  Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.       */
/*  All rights reserved.                                              */
/*  Licenced under the New BSD. See COPYING for detail.               */
/**********************************************************************/

#include <mierulib.h>

#define MAX_LINES 2048

void usage(){
    lcd_print("\
less [filename]\n\
");
}

int print_part(char *buf, unsigned int pos1, unsigned int pos2){
     char tmp;
     tmp = buf[pos2];
     buf[pos2] = 0;
     lcd_print(&buf[pos1]);
     buf[pos2] = tmp;
     return 0;
}

int main(int argc, char *argv[]){
    int fd, ret;
    int size;
    char *buf;
    int i;
    int width, height;
    int top, bottom;
    uchar key;

    int lines = 0;
    unsigned short int pos[MAX_LINES];
    int count = 0;
    int head;

    int oldx, oldy;

    lcd_init();
    lcd_setscrollmode(1);
    oldx = lcd_getcurx();
    oldy = lcd_getcury();

    if(argc != 2){
        usage();
        return 1;
    }

    width = lcd_getwidth()/8;
    height = lcd_getheight()/16;

    fd = sys_open(argv[1], O_RDONLY);
    if(fd < 0){
        lcd_printf("can't open file (%s)\n",argv[1]);
        return 1;
    }

    size = sys_lseek(fd, 0, SEEK_END);
    sys_lseek(fd, 0, SEEK_SET);
    buf = mpc_malloc(size + 1);
    if(buf == NULL){
        lcd_printf("very large file (%d)\n", size);
        sys_close(fd);
    }

    ret = sys_read(fd, buf, size);
    sys_close(fd);
    if(ret < 0){
        lcd_printf("can't read file.\n");
    }
    buf[ret] = 0;

    pos[0] = 0;
    for(i=0;i<ret;i++){
        count++;
        if( buf[i] == '\n' || count == width){
            //if(buf[i] =='\n') buf[i] = 0;

            pos[lines+1] = i + 1;
            lines++;
            count = 0;

            if(lines == MAX_LINES){
                lcd_printf("too long lines(over %d).\n", MAX_LINES);
                return 1;
            }
        }
    }

    lcd_setshowpage(1);
    lcd_setdrawpage(1);
    lcd_clearpage(1);
    lcd_locate(0, 0);
    lcd_setscrollmode(0);

    if(count > 0){
        pos[lines+1] = ret;
        lines++; // this is safe.
    }

    head = height;
    if(lines < height)
        head = lines;

    top = 0;
    bottom = head - 1;

    for(i=0;i < head; i++){
        print_part(buf, pos[i], pos[i+1]);
    }
    
    for(;;){
        key = mpc_getchar(NULL);
        if(key != 0){
            if(key == 'q'){
                break;

            }else if(key == 'j'){
                if(bottom + 1 >= lines) continue;

                top++;
                bottom++;
                lcd_vscroll(1,1);
                lcd_locate(0, height-1);
                print_part(buf, pos[bottom], pos[bottom+1]);

            }else if(key == 'k'){
                if(top <= 0) continue;

                top--;
                bottom--;
                lcd_vscroll(-1,1);
                lcd_locate(0, 0);
                print_part(buf, pos[top], pos[top+1]);

            }
        }
    }


    lcd_cls();
    lcd_setshowpage(0);
    lcd_setdrawpage(0);
    lcd_locate(oldx, oldy);

    return 0;
}
