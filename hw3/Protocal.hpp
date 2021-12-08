#ifndef _PROTOCAL_HPP_
#define _PROTOCAL_HPP_

#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct message_t {
    uint8_t flag;
    uint8_t version;
    uint16_t name_len;
    uint16_t mesg_len;
    unsigned char name[2048];
    unsigned char mesg[2048];
};

struct message_pk {
    uint16_t len;
    unsigned char data[2048];
};

void *memcat(unsigned char *&destination, const void * source, size_t num) { // Only unsigned char supported!
    memcpy(destination, source, num);
    destination = destination + num;
    return destination;
}

unsigned char base64_code(uint8_t c) {
    if (0 <= c && c < 26) {
        return 'A' + c;
    } else if (26 <= c && c < 52) {
        return 'a' + c - 26;
    } else if (52 <= c && c < 62) {
        return '0' + c - 52;
    } else if (c == 62) {
        return '+';
    } else if (c == 63) {
        return '/';
    }
    return '=';
}

uint8_t base64_uncode(uint8_t c) {
    if ('A' <= c && c <= 'Z') {
        return c - 'A';
    } else if ('a' <= c && c <= 'z') {
        return 26 + c - 'a';
    } else if ('0' <= c && c <= '9') {
        return 52 + c - '0';
    } else if (c == '+') {
        return 62;
    } else if (c == '/') {
        return 63;
    }
    return 64;
}

int base64_encode(unsigned char *buff, unsigned char *arr, int len) {
    uint8_t code = 0;
    int code_bit = 0;
    unsigned char *ptr = buff;
    // printf("base64 encoding...\n");
    // printf("%s\n", arr);
    for (int i=0; i < len; i++) {
        for (int j = 0; j < 8; j++) {
            code <<= 1;
            code |= ((arr[i] >> (7 - j)) & 1);
            // printf("%d", (int) ((code >> (7 - j)) & 1));
            code_bit ++;

            if (code_bit == 6) {
                unsigned char c = base64_code(code);
                // printf(" >>> \'%c\'\n", (char) c);
                memcat(ptr, &c, sizeof(c));
                code = 0;
                code_bit = 0;
            }
        }
    }
    if (code_bit == 2) {
        code <<= 4;
        unsigned char c = base64_code(code);
        memcat(ptr, &c, sizeof(c));
        *ptr++ = '=';
        *ptr++ = '=';
    } else if (code_bit == 4) {
        code <<= 2;
        unsigned char c = base64_code(code);
        memcat(ptr, &c, sizeof(c));
        *ptr++ = '=';
    }
    *ptr = '\0';

    return ptr - buff;
}

int base64_decode(unsigned char *buff, unsigned char *&arr) {
    unsigned char *ptr = buff;
    unsigned char code = 0;
    int code_bit = 0;
    // printf("base64 decoding...\n");
    for (; *arr != '\n'; arr++) {
        if (*arr == '=') {
            continue;
        }
        uint8_t b64 = base64_uncode(*arr);
        for (int j=0; j < 6; j++) {
            code <<= 1;
            code |= ((b64 >> (5 - j)) & 1);
            // printf("%d", (int) ((b64 >> (5 - j)) & 1));
            code_bit ++;

            if (code_bit == 8) {
                *ptr++ = code;
                // printf(" >>> \'%c\'\n", code);
                code_bit = 0;
                code = 0;
            }
        }
    }
    *ptr = '\0';
    arr++;

    return ptr - buff;
}

int pack_message(message_pk *pack, message_t message) {
    uint16_t tmp;
    unsigned char buff[2048];
    pack->data[0] = '\0';
    pack->len = 0;
    unsigned char *ptr = pack->data;
    if (message.version == 1) {
        memcat(ptr, &(message.flag), sizeof(message.flag));
        memcat(ptr, &(message.version), sizeof(message.version));

        tmp = htons(message.name_len);
        memcat(ptr, &tmp, sizeof(tmp));
        memcat(ptr, message.name, message.name_len);

        tmp = htons(message.mesg_len);
        memcat(ptr, &tmp, sizeof(tmp));
        memcat(ptr, message.mesg, message.mesg_len);
        *ptr = '\0';
        pack->len = ptr - pack->data;
        return pack->len;
    } else if (message.version == 2) {
        memcat(ptr, &(message.flag), sizeof(message.flag));
        memcat(ptr, &(message.version), sizeof(message.version));

        int len = base64_encode(buff, message.name, message.name_len);
        memcat(ptr, buff, len);
        *ptr++ = '\n';
        len = base64_encode(buff, message.mesg, message.mesg_len);
        memcat(ptr, buff, len);
        *ptr++ = '\n';
        *ptr = '\0';

        pack->len = ptr - pack->data;
        return pack->len;
    }

    return -1;
}

message_t *unpack_message(message_t *message, message_pk *pack) {
    // printf("Unpacking message...\n");
    uint8_t flag = *(pack->data);
    if (flag != 1) {
        printf("Flag is not in correct format.\n");
        return message;
    }
    uint8_t version = *(pack->data + sizeof(flag));
    // printf("Flag = %d, Ver = %d\n", (int) flag, (int) version);
    // printf("pack length = %d\n", pack->len);
    if (version == 1) {
        message->flag = flag;
        message->version = version;
        unsigned char *ptr = pack->data + sizeof(flag) + sizeof(version);
        message->name_len = ntohs(*(uint16_t*) ptr);
        // printf("name_len = %d\n", (int) message->name_len);
        ptr = ptr + sizeof(uint16_t);
        memcpy(message->name, ptr, message->name_len);
        message->name[message->name_len] = '\0';
        ptr = ptr + sizeof(unsigned char) * message->name_len;
        // printf("name = %s\n", (char*) message->name);

        message->mesg_len = ntohs(*(uint16_t*) ptr);
        // printf("mesg_len = %d\n", (int) message->mesg_len);
        ptr = ptr + sizeof(uint16_t);
        memcpy(message->mesg, ptr, message->mesg_len);
        message->mesg[message->mesg_len] = '\0';
        // printf("mesg = %s\n", (char*) message->mesg);

        return message;
    } else if (version == 2) {
        message->flag = flag;
        message->version = version;
        unsigned char *ptr = pack->data + sizeof(flag) + sizeof(version);
        message->name_len = base64_decode(message->name, ptr);
        message->mesg_len = base64_decode(message->mesg, ptr);
    }

    return message;
}

message_pk *genpack(char *s, int len, message_pk *pack) {
    pack->len = len;
    memcpy(pack->data, s, len);
    pack->data[len] = '\0';
    return pack;
}

#endif
