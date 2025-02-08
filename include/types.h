// include/types.h
#ifndef TYPES_H
#define TYPES_H

// Base types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef unsigned int size_t;
typedef enum { FALSE = 0, TRUE = 1 } bool; 

// Network types
typedef struct {
    uint8_t addr[6];
} mac_addr_t;

typedef struct {
    uint8_t addr[4];
} ip_addr_t;

typedef struct {
    mac_addr_t dest_mac;
    mac_addr_t src_mac;
    uint16_t type;
    uint8_t data[];
} __attribute__((packed)) eth_frame_t;

typedef struct {
    uint8_t version_ihl;
    uint8_t tos;
    uint16_t total_length;
    uint16_t id;
    uint16_t flags_fragment;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    ip_addr_t src_ip;
    ip_addr_t dest_ip;
} __attribute__((packed)) ip_header_t;

#endif