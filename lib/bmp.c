/******************************************************************************/
/* MieruPC Project: Bitmap Library v1.3                                       */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-07-15 */
/******************************************************************************/

/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*! @file
 *@brief ビットマップ描画ライブラリ
 *
 *ビットマップ描画ライブラリ．
 *このライブラリはOS用のアプリケーションの時のみ使用可能である．
 */

#include <mierulib.h>
#include <bmp.h>

/******************************************************************************/
int bmp_draw(const char *bmp, int x0, int y0, int bmp_x0, int bmp_y0, int xsize, int ysize){
    if(bmp == NULL) return 1;
    int x, y, i;
    rgbquad_t *bmp_palette;
    bmp_file_hdr_t *file_hdr;
    bmp_info_hdr_t *info_hdr;
    int bmp_width, bmp_height;

    file_hdr = (bmp_file_hdr_t *)bmp;
    info_hdr = (bmp_info_hdr_t *)(bmp + sizeof(bmp_file_hdr_t));
    bmp_palette = (rgbquad_t *)(bmp + sizeof(bmp_file_hdr_t) + info_hdr->size);

    bmp_width = info_hdr->width;
    bmp_height = info_hdr->height;
    if( file_hdr->type[0] != 'B' ||
        file_hdr->type[1] != 'M' ||
        x0 < 0     || y0 < 0     ||
        bmp_x0 < 0 || bmp_y0 < 0 ||
        bmp_x0 + xsize > bmp_width   ||
        bmp_y0 + ysize > bmp_height  ||
        x0 + xsize > lcd_getwidth()  ||
        y0 + ysize > lcd_getheight() ) return 1;

    if(info_hdr->bitcount != 8)
        return 1;

    if( info_hdr->compression == 0){ // normal bitmap
        bmp_loadpalette(bmp_palette);
        for(y = ysize - 1; y >= 0; y--){
            i = file_hdr->offset + bmp_width * (bmp_height - bmp_y0 - y - 1) + bmp_x0;
            lcd_setpos(x0, y0 + y);
            for(x = 0; x < xsize; x++){
                lcd_setcolor((uchar)bmp[i++]);
                lcd_drawdot(); // also move right
            }
        }

    }else if( info_hdr->compression == 1){ // BI_RLE (Run-Length-Encoded 8bits/pixel)
        const uchar *p;
        int n;
        bmp_loadpalette(bmp_palette);
        p = (const uchar *)&bmp[file_hdr->offset];
        
        x = y = 0;
        lcd_setpos(x0, y0 + ysize - 1);
        for(;;){
            n = *p++;
            if(n == 0){
                n = *p++;
                if(n == 0 || n == 1){ // end of line
                    y++;
                    if(y == ysize)
                        break;
                    lcd_setpos(x0, y0 + ysize - 1 - y);
                }else if(n == 2){ // offset
                    p++;p++;
                    lcd_dprintf("Offset Error: can't handle this mode.\n");
                }else{ // absolute mode
                    for(i=0;i<n;i++){
                        lcd_setcolor((uchar)*p++);
                        lcd_drawdot();
                    }
                    if(n%2) p++;  // odd size
                }
            }else{ // encoding mode
                lcd_setcolor((uchar)*p++);
                for(i=0;i<n;i++){
                    lcd_drawdot();
                }
            }
        }
    }
    return 0;
}

/******************************************************************************/
int bmp_getwidth(const char *bmp){
    if(bmp == NULL) return -1;
    bmp_info_hdr_t *info_hdr;
    info_hdr = (bmp_info_hdr_t *)(bmp + sizeof(bmp_file_hdr_t));

    return info_hdr->width;
}

/******************************************************************************/
int bmp_getheight(const char *bmp){
    if(bmp == NULL) return -1;
    bmp_info_hdr_t *info_hdr;
    info_hdr = (bmp_info_hdr_t *)(bmp + sizeof(bmp_file_hdr_t));

    return info_hdr->height;
}

/******************************************************************************/
int bmp_getsize(const char *bmp){
    if(bmp == NULL) return -1;
    bmp_file_hdr_t *file_hdr;
    bmp_info_hdr_t *info_hdr;
    file_hdr = (bmp_file_hdr_t *)bmp;
    info_hdr = (bmp_info_hdr_t *)(bmp + sizeof(bmp_file_hdr_t));

    return file_hdr->offset + info_hdr->width * info_hdr->height;
}

/******************************************************************************/
void bmp_loadpalette(const rgbquad_t *palette){
    int i;
    for(i=0;i<256;i++)
        lcd_setpalette(i, palette[i].red, palette[i].green, palette[i].blue);

    return;
}

/******************************************************************************/
