/**
 * File:   rtcp.h
 * Author: Ericson Joseph
 *
 * Created on September 25, 2018, 11:45 PM
 */

#ifndef __RTCP_H
#define __RTCP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// Is Source Description RTCP Packet (SDES)
bool RTCP_isSDES(uint8_t *buffer, uint16_t size);

//  Read CNAME from SDES
void RTCP_readCNAME(char *descBuffer, uint8_t *frameBuffer);

#ifdef __cplusplus
}
#endif

#endif /* __RTCP_H */
