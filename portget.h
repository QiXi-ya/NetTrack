#pragma once

#define MIN_DYNAMIC_PORT 100  //�˿���ʼ��
#define MAX_DYNAMIC_PORT 1000
#define BLACKLIST_PORT_COUNT 21

// ���ö˿ں��������ɸ�����Ҫ��չ��
const unsigned short COMMON_PORTS[BLACKLIST_PORT_COUNT] = {
    20,    // FTP
    21,    // FTP
    22,    // SSH
    23,    // Telnet
    25,    // SMTP
    53,    // DNS
    80,    // HTTP
    110,   // POP3
    143,   // IMAP
    443,   // HTTPS
    3306,  // MySQL
    3389,  // RDP
    5432,  // PostgreSQL
    8080,  // HTTP Alt
    8443,  // HTTPS Alt
    27017, // MongoDB
    11211, // Memcached
    25565, // Minecraft
    80,    // HTTP (�ظ�ȷ��)
    443    // HTTPS (�ظ�ȷ��)
};

bool IsPortAvailable(unsigned short port);

unsigned short GenerateDynamicPort();