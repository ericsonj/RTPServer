/**
 * File:   netmanager.c
 * Author: Ericson Joseph
 *
 * Created on September 27, 2018,  9:22 PM
 */

#include "netmanager.h"
#include "sys_time.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_ROUTES 20
#define NUM_DEVICES 20
#define MAX_TIME_REMOVE 60000 // 60 seg

static device_t devicesList[NUM_DEVICES];
static rtp_route_t routeTable[NUM_ROUTES];

void NE_Init() {
    for (int i = 0; i < NUM_DEVICES; i++) {
        strcpy(devicesList[i].key, "");
        memset(&devicesList[i], 0, sizeof(struct sockaddr_in));
        devicesList[i].isFree = true;
        devicesList[i].time   = 0;
    }
}

bool NE_exist(char *key) {
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (strcmp(devicesList[i].key, key) == 0) {
            return true;
        }
    }
    return false;
}

void NE_put(char *key, struct sockaddr_in addr) {
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (devicesList[i].isFree) {
            strcpy(devicesList[i].key, key);
            devicesList[i].addr   = addr;
            devicesList[i].isFree = false;
            devicesList[i].time   = millis();
            return;
        }
    }
}

device_t *NE_get(char *key) {
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (strcmp(devicesList[i].key, key) == 0) {
            return &devicesList[i];
        }
    }
    return NULL;
}

void NE_touch(char *key, struct sockaddr_in addr) {
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (strcmp(devicesList[i].key, key) == 0) {
            devicesList[i].time = millis();
            devicesList[i].addr = addr;
        }
    }
}

void NE_removeOlds() {
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (!devicesList[i].isFree) {
            long long int max_time = millis() - devicesList[i].time;
            if (max_time > MAX_TIME_REMOVE) {
                printf("Device %s remove.\n", devicesList[i].key);
                NE_remove(devicesList[i].key);
            }
        }
    }
}

void NE_remove(char *key) {
    char tmp_key[50];
    strcpy(tmp_key, key);
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (strcmp(devicesList[i].key, tmp_key) == 0) {
            strcpy(devicesList[i].key, "");
            memset(&devicesList[i], 0, sizeof(struct sockaddr_in));
            devicesList[i].isFree = true;
            devicesList[i].time   = 0;
            return;
        }
    }
}

void NE_initTableRoute() {

    for (int i = 0; i < NUM_ROUTES; ++i) {
        strcpy(routeTable[i].ne_a, "");
        strcpy(routeTable[i].ne_b, "");
    }

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("router_list.txt", "r");
    if (fp == NULL) {
        return;
    }

    int i = 0;
    printf("Route table:\n");
    while ((read = getline(&line, &len, fp)) != -1 && i < NUM_ROUTES) {
        sscanf(line, "%s %s", routeTable[i].ne_a, routeTable[i].ne_b);
        printf("%s > %s\n", routeTable[i].ne_a, routeTable[i].ne_b);
        i++;
    }
    fflush(stdout);

    fclose(fp);
    if(line){
        free(line);
    }

}

rtp_route_t *NE_getRoute(char *KEY_ne_a) {
    for (int i = 0; i < NUM_ROUTES; ++i) {
        if (strcmp(routeTable[i].ne_a, KEY_ne_a) == 0) {
            return &routeTable[i];
        }
    }
    return NULL;
}

device_t *NE_getDscNe(struct sockaddr_in *addr) {
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (devicesList[i].addr.sin_addr.s_addr == addr->sin_addr.s_addr &&
            devicesList[i].addr.sin_port == addr->sin_port) {
            rtp_route_t *route = NE_getRoute(devicesList[i].key);
            if (route != NULL) {
                device_t *dscNE = NE_get(route->ne_b);
                if (dscNE != NULL) {
                    return dscNE;
                }
            }
        }
    }
    return NULL;
}
