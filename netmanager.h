/**
 * File:   netmanager.h
 * Author: Ericson Joseph
 *
 * Created on September 27, 2018,  8:34 PM
 */

#ifndef __NETMANAGER_H
#define __NETMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>

#define KEY_LENGTH 50

typedef struct {
    char key[KEY_LENGTH];
    struct sockaddr_in addr;
    bool isFree;
    long long int time;
} device_t;

typedef struct {
    char ne_a[KEY_LENGTH];
    char ne_b[KEY_LENGTH];
} rtp_route_t;

void NE_Init();

bool NE_exist(char *key);

void NE_put(char *key, struct sockaddr_in addr);

device_t *NE_get(char *key);

    void NE_touch(char *key, struct sockaddr_in addr);

void NE_removeOlds();

void NE_remove(char *key);

void NE_initTableRoute();

rtp_route_t *NE_getRoute(char *KEY_ne_a);

device_t *NE_getDscNe(struct sockaddr_in* addr);

#ifdef __cplusplus
}
#endif

#endif /* __NETMANAGER_H */
