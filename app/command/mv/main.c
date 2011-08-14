/**********************************************************************/
/* MieruPC Project: Default Command 'mv'         Arch Lab. TOKYO TECH */
/*  Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.       */
/*  All rights reserved.                                              */
/*  Licenced under the New BSD. See COPYING for detail.               */
/**********************************************************************/

#include <mierulib.h>

void usage(){
    lcd_print("\
mv [src_path] [dst_path]\n\
");
}

int main(int argc, char *argv[]){
    int ret;

    lcd_init();
    lcd_setscrollmode(1);

    if(argc != 3){
        usage();
        return 0;
    }
    
    ret = sys_rename(argv[1], argv[2]);
    lcd_printf("move %s to %s\n",argv[1], argv[2]);
    if(ret < 0){
        switch(-ret){
        case 1:         // FAT_ERROR_NO_SUCH_FILE
            lcd_printf("file not found\n");
            break;
        case 2:         // FAT_ERROR_FILE_ALREADY_EXIST
            lcd_printf("file already exist\n");
            break;
        case 3:         // FAT_ERROR_INVALID_FILE_NAME
            lcd_printf("invalid file name\n");
            break;
        default:
            lcd_printf("undefined error\n");
            break;
        }
    }
    return 0;
}
