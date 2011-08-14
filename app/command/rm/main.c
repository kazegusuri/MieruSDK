/**********************************************************************/
/* MieruPC Project: Default Command 'rm'         Arch Lab. TOKYO TECH */
/*  Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.       */
/*  All rights reserved.                                              */
/*  Licenced under the New BSD. See COPYING for detail.               */
/**********************************************************************/

#include <mierulib.h>

void usage(){
    lcd_print("\
rm [filename] (filename)\n\
");
}

int main(int argc, char *argv[]){
    int i, ret;
    
    lcd_init();
    lcd_setscrollmode(1);

    if(argc < 2){
        usage();
    }else{
        for(i = 1; i < argc; i++){
            ret = sys_remove(argv[i]);
            if(ret < 0){
                switch(-ret){
                case 1:  // FAT_ERROR_NO_SUCH_FILE
                    lcd_printf("file not found (%s)\n",argv[i]);
                    break;
                case 3:  // FAT_ERROR_INVALID_FILE_NAME
                    lcd_printf("invalid file name (%s)\n",argv[i]);
                    break;
                case 12: // FAT_ERROR_DIRECTORY_NOT_EMPTY
                    lcd_printf("%s is directory\n",argv[i]);
                    break;
                }
            }else{
                lcd_printf("remove %s\n",argv[i]);
            }
        }
    }
    return 0;
}
