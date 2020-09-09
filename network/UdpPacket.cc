#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#define IP_PROTO_UDP 17

typedef struct _UdpHeader {
    uint16_t srcPort;
    uint16_t dstPort;
    uint16_t dataLen; // header size + data size
    uint16_t checkSum;
} __attribute__((packed)) UdpHeader, *PUdpHeader;

typedef struct _UdpPacket {
    UdpHeader header;
    uint8_t data[1];
} UdpPacket, *PUdpPacket;

#define P( x ) printf( "Add %04x %0x\n", (x), sum );

uint16_t CheckSumUdp2( PUdpHeader header ) {
    header->checkSum = htons( 0 );
    uint32_t sum1 = 0;
    uint32_t sum2 = 0;
    uint16_t count = ntohs( header->dataLen ) + 8;
    uint8_t *data = ( (uint8_t*)header ) - 8;
    for( int i = 0; i < count; i += 2 ) {
printf( "add %02x", *data );
        sum1 += *data++;
printf( "%02x ", *data );
        sum2 += *data++;
printf( "%0x\n", (sum1<<8) + sum2 );
    }
    sum1 += 0;
    sum2 += 17;
    sum1 += 0;
    sum2 += count - 8;
    printf( "sum1 = %0x\n", sum1 );
    printf( "sum2 = %0x\n", sum2 );
    uint32_t sum = (sum1<<8) + sum2;
    
    while( sum >> 16 ) sum = ( sum & 0xFFFF ) + ( sum >> 16 );
    sum = (~sum & 0xFFFF);
printf( "sum = %x\n", sum );
    return header->checkSum = htons( sum );
}

uint16_t CheckSumUdp(PUdpHeader header) {
	header->checkSum = htons(0);
	
	uint16_t count = ntohs(header->dataLen);

	uint32_t sum1 = 0;
	uint32_t sum2 = 17 + count;

	uint8_t *data = ((uint8_t*)header) - 8;
	count += 8;
	while (count > 1) {
		sum1 += *data++;
		sum2 += *data++;
		count -= 2;
	}
	if (count > 0) sum1 += *data;

	uint32_t sum = (sum1 << 8) + sum2;

	while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
	sum = (~sum & 0xFFFF);
	return header->checkSum = htons(sum);
}

typedef struct _IPv4Header {
    uint8_t version : 4;
    uint8_t headLen : 4;
    uint8_t typeOfService;
    uint16_t totalLen;  // header size + data size
    uint16_t id;
    uint16_t flag : 3;
    uint16_t fragOffset : 13;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checkSum;
    struct in_addr srcIP;
    struct in_addr dstIP;
} __attribute__((packed)) IPv4Header, *PIPv4Header;

uint16_t CheckSumIPv4( PIPv4Header header ) {
    header->checkSum = htons( 0 );
    uint16_t *data = (uint16_t*)header;
    uint32_t sum = 0;
    for( int i = 0; i < 10; ++i ) sum += ntohs( data[i] );
    while( sum >> 16 ) sum = ( sum & 0xFFFF ) + ( sum >> 16 );
	sum = (~sum & 0xFFFF);
    return header->checkSum = htons( sum );
}

int FillDomain( uint8_t *data, const char *domain ) {
    const char *p = domain;
    int len_pos = 0;
    int cur_pos = 1;

    while( *p ) {
        data[cur_pos++] = *p;
        if( *p == '.' ) {
            data[len_pos] = (uint8_t)(cur_pos - len_pos - 2);
            len_pos = cur_pos - 1;
        }
        ++p;
    }
    data[len_pos] = (uint8_t)(cur_pos - len_pos - 1);
    data[cur_pos++] = 0;
    return cur_pos;
}


uint16_t CheckSumUdp3( PUdpHeader header ) {

    header->checkSum = 0;

    uint32_t sum = 0;

    uint16_t count = ntohs( header->dataLen );

    // pseudo header
    sum += IP_PROTO_UDP; // protocol udp
    sum += count;

    uint8_t *data = ( (uint8_t*)header ) - 8;
    count += 8;

    while( count > 1 ) {
	sum += ntohs(*(uint16_t*)data);
P( *(uint16_t*)data );
	data += 2;
        count -= 2;
    }

    if( count > 0 ) {
	sum += ntohs( *(uint8_t*)data );
P( *(uint8_t*)data )
    }


    while( sum >> 16 ) sum = ( sum & 0xFFFF ) + ( sum >> 16 );
    sum = (~sum & 0xFFFF);
    return header->checkSum = htons( sum );
}

int main() {

        uint8_t buffer[] = { 
0x0a, 0x00, 0x00, 0x83,
0x0a, 0x00, 0x00, 0x02,
0xd1, 0x17, 0x00, 0x35,
0x00, 0x23, 0x8a, 0x84,
0x36, 0x30, 0x01, 0x00,
0x00, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x05, 0x73, 0x6f, 0x67,
0x6f, 0x75, 0x03, 0x63,
0x6f, 0x6d, 0x00,
0x00, 0x01, 0x00, 0x01,
0x00
};

	uint8_t buffer2[] = { 0x45, 0x00, 0x00, 0x3a,
0xb3, 0x57, 0x00, 0x00,
0x40, 0x11, 0x76, 0x27,
0xc0, 0xa8, 0x60, 0xc1,
0xc0, 0xa8, 0x6f, 0x22

};

	PUdpHeader udpHeader = (PUdpHeader)(buffer + 8);

	printf( "dataLen = %0x\n", ntohs( udpHeader->dataLen ) );

	printf( "CS1 %0x\n", CheckSumUdp( udpHeader ));
	printf( "CS2 %0x\n", CheckSumUdp2( udpHeader ));
	printf( "CS3 %0x\n", CheckSumUdp3( udpHeader ));

	PIPv4Header ipHeader = (PIPv4Header)buffer2;
	printf( "%0x\n", CheckSumIPv4( ipHeader ) );
	printf( "%0x %0x\n", buffer2[10], buffer2[11] );

uint8_t buffer3[64] = {0};
int len = FillDomain( buffer3, "baidu.com" );
	printf( "len = %d\n", len );
	for( int i = 0; i < len; ++i ) printf( "%0x ", buffer3[i] );

	return 0;
}
