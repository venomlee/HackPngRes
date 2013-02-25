//
//  main.c
//  Hack_MTRes
//
//  Created by Venom on 2/25/13.
//  Copyright (c) 2013 Venom. All rights reserved.
//

#include <stdio.h>
#include <assert.h>

int main(int argc, const char * argv[])
{
    if(argc < 2)
    {
        printf("This HackTool is for Hacking png Res outof its package...\nplease pass a filename to this tool!\n\n");
        return 1;
    }
    
    FILE* fp = fopen(argv[1], "rb");
    if (!fp) {
        printf("file %s cannot be open!\n",argv[1]);
        return 1;
    }
    
    char    tmp[1024];
    int     hkcount = 0;
    size_t  readsize = 0;
    char    head[4] = {0x89,0x50,0x4e,0x47};
    char    end[8] = {0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82};
    int     verifyHead = 0;
    int     verifyEnd = 0;
    int     writePng = 0;
    FILE*   fpWrite = NULL;
    while ((readsize = fread(tmp, 1, 1024, fp))) {
        for (int i=0; i<readsize; i++) {
            if (writePng && fpWrite) {
                fwrite(tmp+i, 1, 1, fpWrite);
            }
            if (verifyHead < 4) {
                if (tmp[i] == head[verifyHead]) {
                    verifyHead++;
                    if (verifyHead == 4) {
                        writePng = 1;
                        hkcount++;
                        char pngName[256];
                        sprintf(pngName, "%3d.png",hkcount);
                        printf("found png header, now creating %s ...\n",pngName);
                        fpWrite = fopen(pngName, "wb");
                        assert(fpWrite);
                        fwrite(head, 4, 1, fpWrite);
                    }
                }else{
                    verifyHead = 0;
                }
            }else if (verifyHead == 4 &&
                verifyEnd < 8) {
                if (tmp[i] == end[verifyEnd]) {
                    verifyEnd++;
                    if (verifyEnd == 8) {
                        writePng = 0;
                        fclose(fpWrite);
                        verifyHead = 0;
                        verifyEnd = 0;
                    }
                }else{
                    verifyEnd = 0;
                }
            }
        }
    }
    
    fclose(fp);
    
    printf("done...check the directory plz");
    return 0;
}

