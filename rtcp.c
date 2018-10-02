/**
 * File:   rtcp.c
 * Author: Ericson Joseph
 *
 * Created on September 25, 2018, 11:49 PM
 */

#include "rtcp.h"
#include <stdio.h>
#include <string.h>

bool RTCP_isSDES(uint8_t *buffer, uint16_t size) {
    bool isValidSDES = false;
    if (size > 2) {
        if (buffer[0] == 0x81 && buffer[1] == 0xca) {
            isValidSDES = true;
        }
    }
    return isValidSDES;
}

void RTCP_readCNAME(char *descBuffer, uint8_t *frameBuffer) {
    uint8_t cname_len = frameBuffer[9] + 1;
    char cname[cname_len];
    for (int i = 0; i < cname_len; i++) {
        cname[i] = frameBuffer[10 + i];
    }
    cname[cname_len - 1] = '\0'; 
    strcpy(descBuffer, cname);
}
