#ifndef _PROTOCAL_HPP_
#define _PROTOCAL_HPP_

#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

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

void *memcat(unsigned char *destination, const void * source, size_t num) { // Only unsigned char supported!
    memcpy(destination, &source, num);
    destination = destination + num;
    return destination;
}

int pack_message(message_pk *pack, message_t message) {
    uint16_t tmp;
    pack->data[0] = '\0';
    unsigned char *ptr = pack->data;
    if (message.version == 1) {
        memcat(ptr, &(message.flag), sizeof(message.flag));
        memcat(ptr, &(message.version), sizeof(message.version));

        tmp = htons(message.name_len);
        memcat(ptr, &(tmp), sizeof(tmp));
        memcat(ptr, message.name, message.name_len);

        tmp = htons(message.mesg_len);
        memcpy(ptr, &tmp, sizeof(message.mesg_len));
        memcpy(ptr, &tmp, message.name_len);
        *ptr = '\0';
        pack->len = ptr - pack->data;
        return pack->len;
    } else if (message.version == 2) {
        // TODO
        return -1;
    }

    return -1;
}

message_t *unpack_message(message_t *message, message_pk *pack) {
    printf("Unpacking message...\n");
    uint8_t flag = *(pack->data);
    if (flag != 1) {
        printf("Flag is not in correct format.\n");
        return message;
    }
    uint8_t version = *(pack->data + sizeof(flag));
    printf("Flag = %d, Ver = %d\n", (int) flag, (int) version);
    printf("pack length = %d\n", pack->len);
    if (version == 1) {
        message->flag = flag;
        message->version = version;
        unsigned char *ptr = pack->data + sizeof(flag) + sizeof(version);
        message->name_len = ntohs(*(uint16_t*) ptr);
        printf("name_len = %d\n", (int) message->name_len);
        ptr = ptr + sizeof(uint16_t);
        memcpy(message->name, ptr, message->name_len);
        message->name[message->name_len] = '\0';
        ptr = ptr + sizeof(unsigned char) * message->name_len;
        printf("name = %s\n", (char*) message->name);

        message->mesg_len = ntohs(*(uint16_t*) ptr);
        printf("mesg_len = %d\n", (int) message->mesg_len);
        ptr = ptr + sizeof(uint16_t);
        memcpy(message->mesg, ptr, message->mesg_len);
        message->name[message->name_len] = '\0';
        printf("mesg = %s\n", (char*) message->mesg);

        return message;
    } else if (version == 2) {

        // TODO
    }

    return message;
}

message_pk *genpack(char *s, int len, message_pk *pack) {
    pack->len = len;
    memcpy(pack->data, s, len);
    return pack;
}

#endif
