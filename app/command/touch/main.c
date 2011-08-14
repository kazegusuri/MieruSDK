/**********************************************************************/
/* MieruPC Project: Default Command 'touch'      Arch Lab. TOKYO TECH */
/*  Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.       */
/*  All rights reserved.                                              */
/*  Licenced under the New BSD. See COPYING for detail.               */
/**********************************************************************/

#include <mierulib.h>

void usage(){
    lcd_print("\
touch filename \
");
}

int main(int argc, char *argv[]){
    int ret;
    char *filename;

    lcd_init();
    lcd_setscrollmode(1);

    if(argc != 2){
        usage();
        return 1;
    }

    filename = argv[1];

    ret = sys_creat(filename, 0);
    if(ret < 0){
        switch(-ret){
        case  3: // FAT_ERROR_INVALID_FILE_NAME
            lcd_printf("invalid file name (%s)\n",filename);
            break;
        case  2: // FAT_ERROR_FILE_ALREADY_EXIST
            lcd_printf("%s is already exist\n",filename);
            break;
        case 10: // FAT_ERROR_DISC_IS_FULL
            lcd_printf("### ERROR ###\n");
            lcd_printf("DISK IS FULL or SYSTEM ERROR\n");
            break;
        }
    }else{
        lcd_printf("create file %s\n", argv[1]);
        sys_close(ret);
    }

    return 0;
}
