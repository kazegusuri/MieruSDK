/**********************************************************************/
/* MieruPC Project: Default Command 'emin'       Arch Lab. TOKYO TECH */
/*  Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.       */
/*  All rights reserved.                                              */
/*  Licenced under the New BSD. See COPYING for detail.               */
/**********************************************************************/

#include <mierulib.h>

#define MAX_LINES_IN_STRING 8
#define MAX_LENGTH 128

ushort WindowHeight;
ushort WindowWidth;

typedef struct _point{
    short x;
    short y;
}Point;

typedef struct _string{
    ushort num;
    char string[MAX_LENGTH];
    ushort line_num;
    struct _string *next;
    struct _string *prev;
}String;

typedef struct _cursor{
    Point relative;
    Point absolute;
    String *str;
}Cursor;

int quit = 0;
Cursor cursor;
String *head;
char filename[64];

int status_x, status_y;

void String_print_line(String *str, int pos);

void StatusMode_in(){
    int i;
    lcd_copypage(2, 1);
    lcd_setshowpage(1);
    lcd_setdrawpage(1);
    status_x = lcd_getcurx();
    status_y = lcd_getcury();
    lcd_setforeground(COL_BLACK);
    lcd_setbackground(COL_WHITE);

    lcd_locate(0, WindowHeight - 1);
    for(i=0;i<WindowWidth;i++)
        lcd_puts(" ");
}

void StatusMode_out(){
    lcd_setforeground(COL_WHITE);
    lcd_setbackground(COL_BLACK);

    lcd_locate(status_x, status_y);
    lcd_setshowpage(2);
    lcd_setdrawpage(2);
}

void StatusMode_print(char *str){
    int i;
    lcd_locate(0, WindowHeight - 1);
    for(i=0;i<WindowWidth;i++)
        lcd_puts(" ");
    lcd_locate(0, WindowHeight - 1);
    lcd_puts(str);
}

int Cursor_move_previous_line(Cursor *cur){
    if(cur->absolute.y == 0) return 1;

    cur->absolute.y--;
    cur->str = cur->str->prev;

    return 0;
}

int Cursor_move_next_line(Cursor *cur){
    if(cur->str->next == NULL) return 1;

    cur->absolute.y++;
    cur->str = cur->str->next;

    return 0;
}

void Cursor_move_head(Cursor *cur){
    int i, scroll;
    int move = cur->absolute.x / WindowWidth;


    cur->relative.y -= move;
    if(cur->relative.y < 0){
        scroll = -1 * cur->relative.y;
        for(i = 0; i < scroll; i++){
            lcd_vscroll(-1, 1);
            lcd_locate(0,0);
            String_print_line(cur->str,  scroll - i - 1 );
        }
        cur->relative.y = 0;
    }

    cur->absolute.x = 0;
    cur->relative.x = 0;
}

void Cursor_move_end(Cursor *cur){
    int current_line = cur->absolute.x / WindowWidth + 1;
    int scroll = cur->str->line_num - current_line;
    int i;

    cur->relative.y += scroll;
    if(cur->relative.y >= WindowHeight){
        scroll = cur->relative.y - WindowHeight + 1;
        for(i = 0;i < scroll; i++){
            lcd_vscroll(1, 1);
            lcd_locate(0, WindowHeight - 1);
            String_print_line(cur->str, cur->str->line_num - scroll + i);
        }
        cur->relative.y = WindowHeight - 1;
    }

    cur->absolute.x = cur->str->num;
    cur->relative.x = cur->absolute.x % WindowWidth;
}

void Cursor_move_pos(Cursor *cur, int pos){
    if(pos > cur->str->num)
        cur->absolute.x = cur->str->num;
    else
        cur->absolute.x = pos;
}

int Cursor_move_left(Cursor *cur){
    int ret;
    if(cur->absolute.x == 0){
        ret = Cursor_move_previous_line(cur);
        if(ret) return 1;

        Cursor_move_pos(cur, cur->str->num);
    }else{
        cur->absolute.x--;
    }

    cur->relative.x--;
    if(cur->relative.x < 0){
        cur->relative.x = cur->absolute.x % WindowWidth;
        cur->relative.y--;
        if(cur->relative.y < 0){
            cur->relative.y = 0;
            lcd_vscroll(-1, 1);
            lcd_locate(0,0);
            String_print_line(cur->str, cur->absolute.x / WindowWidth);
        }
    }

    return 0;
}

int Cursor_move_right(Cursor *cur){
    int ret;
    if(cur->absolute.x >= cur->str->num){
        ret = Cursor_move_next_line(cur);
        if(ret) return 1;
        
        Cursor_move_pos(cur, 0);
    }else{
        cur->absolute.x++;
    }

    cur->relative.x++;
    if(cur->relative.x == WindowWidth || cur->absolute.x == 0){
        cur->relative.x = cur->absolute.x % WindowWidth;
        cur->relative.y++;
        if(cur->relative.y == WindowHeight){
            cur->relative.y = WindowHeight - 1;
            lcd_vscroll(1, 1);
            lcd_locate(0, WindowHeight - 1);
            String_print_line(cur->str, cur->absolute.x / WindowWidth);
        }
    }

    return 0;
}

int Cursor_move_up(Cursor *cur){
    int current_line = cur->absolute.x / WindowWidth + 1;
    int ret;
    if(current_line > 1){
        cur->absolute.x -= WindowWidth;
    }else{
        ret = Cursor_move_previous_line(cur);
        if(ret) return 1;
        Cursor_move_pos(cur, (cur->str->line_num - 1) * WindowWidth + cur->relative.x);
    }

    cur->relative.x = cur->absolute.x % WindowWidth;
    cur->relative.y--;
    if(cur->relative.y < 0){
        cur->relative.y = 0;
        lcd_vscroll(-1, 1);
        lcd_locate(0,0);
        String_print_line(cur->str, cur->absolute.x / WindowWidth);
    }

    return 0;
}

int Cursor_move_down(Cursor *cur){
    int current_line = cur->absolute.x / WindowWidth + 1;
    int ret;
    if(current_line < cur->str->line_num){
        Cursor_move_pos(cur, cur->absolute.x + WindowWidth);
    }else{
        ret = Cursor_move_next_line(cur);
        if(ret) return 1;
        Cursor_move_pos(cur, cur->relative.x);
    }
    
    cur->relative.x = cur->absolute.x % WindowWidth;
    cur->relative.y++;
    if(cur->relative.y == WindowHeight){
        cur->relative.y = WindowHeight - 1;
        lcd_vscroll(1, 1);
        lcd_locate(0, WindowHeight - 1);
        String_print_line(cur->str, cur->absolute.x / WindowWidth);
    }

    return 0;
}

void Cursor_set(Cursor *cur){
    lcd_locate(cur->relative.x, cur->relative.y);
}


void Cursor_on(Cursor *cur){
    lcd_locate(cur->relative.x, cur->relative.y);
    lcd_setcolor(COL_BLUE);
    lcd_print("@");
    lcd_setcolor(COL_WHITE);
}

void Cursor_off(Cursor *cur){
    char c = cur->str->string[cur->absolute.x];

    lcd_locate(cur->relative.x, cur->relative.y);
    lcd_setcolor(COL_WHITE);
    if(c == 0) c = ' ';
    lcd_printf("%c", c);
}

String *String_create(void){
    String *s;
    s = (String *)mpc_malloc(sizeof(String));
    if(s == NULL) return NULL;

    mpc_memset(s->string, 0, sizeof(String));
    s->num = 0;
    s->line_num = 1;
    s->next = NULL;
    s->prev = NULL;
    return s;
}

void String_free(String *str){
    str->next = str->prev = NULL;
    mpc_free(str);
}

void String_print_line(String *str, int line){
    if(line >= str->line_num) return;

    int i;
    char *p = &str->string[line * WindowWidth];
    for(i=0; i < WindowWidth; i++){
        if(*p != 0){
            lcd_printf("%c", *p);
            p++;
        }else{
            lcd_printf(" ");
        }
    }
}

int String_insert_str(String *s, int pos, char *str, int n){
    if(s->num + n > MAX_LENGTH - 1 || s->num < pos) return 1;
    if(n <= 0) return 0;

    int i;
    for(i = s->num - 1; i >= pos; i--){
        s->string[i+n] = s->string[i];
    }

    for(i = 0; i < n; i++){
        s->string[pos + i] = str[i];
    }

    s->num += n;
    s->line_num = s->num / WindowWidth + 1;

    return 0;
}

int String_insert_char(String *s, int pos, char c){
    return String_insert_str(s, pos, &c, 1);
}

int String_delete_region(String *s, int pos, int n){
    if(pos + n > s->num || s->num < pos) return 1;

    int i;
    for(i = pos; i < s->num - n; i++){
        s->string[i] = s->string[i + n];
    }

    for(i = s->num - n; i < s->num; i++){
        s->string[i] = 0;
    }

    s->num -= n;
    s->line_num = s->num / WindowWidth + 1;

    return 0;
}

int String_delete_pos(String *s, int pos){
    return String_delete_region(s, pos, 1);
}

int String_merge(String *s1, String *s2){
    if(s1 == NULL || s2 == NULL) return 1;

    // size check
    if(s1->num + s2->num >= MAX_LENGTH - 1) return 1;

    if(s2->next != NULL)
        s2->next->prev = s2->prev;
    if(s2->prev != NULL)  // no chance of NULL
        s2->prev->next = s2->next;

    mpc_strcpy(&s1->string[s1->num], s2->string);

    s2->next = s2->prev = NULL;
    s1->num += s2->num;
    s1->line_num = s1->num / WindowWidth + 1;

    return 0;
}

int String_split(String *s, int pos){
    if(s->num < pos) return 1;

    String *back;
    int size;

    back = String_create();
    if(back == NULL) return 1;

    size = s->num - pos;
    mpc_memcpy(back->string, &s->string[pos], size);
    back->num = size;
    back->line_num = back->num / WindowWidth + 1;

    mpc_memset(&s->string[pos], 0, size);
    s->num -= size;
    s->line_num = s->num / WindowWidth + 1;

    back->next = s->next;
    if(back->next->prev != NULL)
        back->next->prev = back;
        
    s->next = back;
    back->prev = s;

    return 0;
}

void View_redraw(Cursor *cur){
    int current_line;
    int line_count;
    int i;
    String *str;
    lcd_clear();

    str = cur->str;
    // find top line
    current_line = cur->relative.y;
    current_line -= cur->absolute.x / WindowWidth;

    while(current_line > 0){
        str = str->prev;
        current_line -= str->line_num;
    }

    //
    line_count = current_line;
    for(;;){
        if(line_count == WindowHeight) break;
    
        for(i=0; i < str->line_num; i++){
            if(line_count >= 0){
                lcd_locate(0, line_count);
                String_print_line(str, i);
                lcd_nextline();
            }
            //mpc_waitkey();

            line_count++;
            if(line_count == WindowHeight) break;
        }

        if(str->next == NULL) break;
        str = str->next;
        
    }
}

int File_open(char *name){
    char buf[128];
    int fd;
    int ret;
    String *str;
    int start, end;
    head = String_create();
    str = head;

    mpc_strcpy(filename, name);

    fd = sys_open(name, O_RDONLY);
    if(fd < 0) return 1;

    for(;;){
        ret = sys_read(fd, buf, 127);
        if(ret < 0) return 1;
        if(ret == 0) break;

        buf[ret] = 0;

        start = 0;
        for(;;){
            for(end=start;end<ret;end++)
                if(buf[end] == '\n') break;
            buf[end] = 0;
        

            String_insert_str(str, str->num, &buf[start], mpc_strlen(&buf[start]));
            if( end == ret ) break;
            String_split(str, str->num);
            str = str->next;
            start = end + 1;
        }
    }
    sys_close(fd);

    return 0;
}

int File_close(){
    String *cur, *next;

    for(cur = head; cur != NULL;){
        next = cur->next;
        String_free(cur);
        cur = next;
    }
    head = NULL;
    return 0;
}

int File_store(){
    int fd;
    int ret;
    char buf[64];
    String *str;

    StatusMode_in();
    mpc_snprintf(buf, 64, "wrote %s", filename);
    StatusMode_print(buf);

    fd = sys_open(filename, O_WRONLY | O_CREAT | O_TRUNK);
    if(fd < 0){
        ret = 1;
        goto out;
    }

    for(str = head; str != NULL; str = str->next){
        sys_write(fd, str->string, str->num);
        if(str->next != NULL)
            sys_write(fd, "\n", 1);
    }

    sys_close(fd);
    ret = 0;

out:;
    if(ret)
        StatusMode_print("Error");
    else
        StatusMode_print("Success");
    mpc_usleep(500000);
    StatusMode_out();

    return ret;
}

void init(char *name){

    WindowWidth = lcd_getwidth()/8;
    WindowHeight = lcd_getheight()/16;

    cursor.relative.x = 0;
    cursor.relative.y = 0;
    cursor.absolute.x = 0;
    cursor.absolute.y = 0;

    File_open(name);
    cursor.str = head;

    View_redraw(&cursor);
    lcd_locate(0, 0);
}

void emin_enter(){
    String_split(cursor.str, cursor.absolute.x);
    Cursor_move_right(&cursor);
    View_redraw(&cursor);
}

int emin_delete(){
    return 0;
}

int emin_backspace(){
    if( cursor.absolute.x == 0 && cursor.absolute.y == 0) return 1;
    int i;
    int current_line;
    int ret;

    if( cursor.absolute.x > 0 ){
        String_delete_pos(cursor.str, cursor.absolute.x - 1);

        if( ((cursor.str->num + 1) % WindowWidth) == 0 ){
            View_redraw(&cursor);
        }
        Cursor_move_left(&cursor);
        current_line = cursor.absolute.x / WindowWidth;

        for(i=0; i < cursor.str->line_num - current_line; i++){
            lcd_locate(0, cursor.relative.y + i);
            String_print_line(cursor.str, i + current_line);
        }
    }else{
        Cursor_move_left(&cursor);
        ret = String_merge(cursor.str, cursor.str->next);
        if(ret){
            Cursor_move_right(&cursor);
            return 1;
        }
        View_redraw(&cursor);
    }

    return 0;
}

int emin_type(uchar key){
    if(cursor.str->num >= MAX_LENGTH - 1) return 1;

    int current_line = cursor.absolute.x / WindowWidth;
    int i;
    
    String_insert_char(cursor.str, cursor.absolute.x, key);
    
    if( (cursor.str->num % WindowWidth) == 0 ){
        View_redraw(&cursor);
    }
    Cursor_move_right(&cursor);
    current_line = cursor.absolute.x / WindowWidth;

    for(i=0; i < cursor.str->line_num - current_line; i++){
        lcd_locate(0, cursor.relative.y + i);
        String_print_line(cursor.str, i + current_line);
    }

    return 0;
}

int emin_command(uchar type){
    uchar key;
    uint option;
    for(;;){
        key = mpc_getchar(&option);

        if(key != 0){
            if(type == 'x'){
                if(mpc_oncontrol()){
                    if(key == 'f'){

                    }else if(key == 's'){
                        File_store();
                    }else if(key == 'c'){
                        quit = 1;
                    }
                }else{
                    if(key == 'c'){
                        quit = 1;
                    }else if(key == 's'){
                        File_store();
                    }
                }
            }
            break;
        }
    }
    return 0;
}

int main(int argc, char *argv[]){
    int oldx, oldy;
    lcd_init();
    lcd_setcursor(1);

    if( argc != 2 ){
        lcd_printf("emin [filename]\n");
        return 0;
    }

    oldx = lcd_getcurx();
    oldy = lcd_getcury();
    lcd_setshowpage(2);
    lcd_setdrawpage(2);
    lcd_locate(0,0);
    init(argv[1]);

    uchar key;
    while(!quit){
        key = mpc_getchar();

        if(key != 0){
            if( !mpc_oncontrol() && !mpc_onalt() ){
                if(key == MKS_Enter){
                    emin_enter();

                }else if(key == MKS_BackSpace || key == MKS_Delete){
                    emin_backspace();

                }else if(0x20 <= key && key < 0x7f){
                    emin_type(key);
                }

            }else if( mpc_oncontrol() ){
                if(key == 'f'){
                    Cursor_move_right(&cursor);
                }else if(key == 'b'){
                    Cursor_move_left(&cursor);
                }else if(key == 'p'){
                    Cursor_move_up(&cursor);
                }else if(key == 'n'){
                    Cursor_move_down(&cursor);
                }else if(key == 'a'){
                    Cursor_move_head(&cursor);
                }else if(key == 'e'){
                    Cursor_move_end(&cursor);
                }else if(key == 'l'){
                    View_redraw(&cursor);
                }else if(key == 'h'){
                    emin_backspace();
                }else if(key == 'x'){
                    emin_command(key);
                }
                

            }else if( mpc_onalt() ){

            }
            
            //Cursor_on(&cursor);
            Cursor_set(&cursor);
        }
        
        mpc_usleep(1000);
    }

    lcd_cls();
    lcd_setshowpage(0);
    lcd_setdrawpage(0);
    lcd_locate(oldx, oldy);

    return 0;
}
