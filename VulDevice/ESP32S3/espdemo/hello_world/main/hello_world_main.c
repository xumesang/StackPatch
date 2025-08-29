/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "hal/cpu_hal.h"


typedef struct exception_info
{
	uint32_t exit;//      *( sp + 0u )
	uint32_t pc;//        *( sp + 1u ) 
	uint32_t ps;//        *( sp + 2u )
	uint32_t a0;//        *( sp + 3u )
	uint32_t a1;//        *( sp + 4u )
	uint32_t a2;//        *( sp + 5u )
	uint32_t a3;//        *( sp + 6u )
	uint32_t a4;//        *( sp + 7u )
	uint32_t a5;//        *( sp + 8u )
	uint32_t a6;//        *( sp + 9u )
	uint32_t a7;//        *( sp + 10u )
	uint32_t a8;//        *( sp + 11u )
	uint32_t a9;//        *( sp + 12u )
	uint32_t a10;//        *( sp + 13u )
	uint32_t a11;//        *( sp + 14u )
	uint32_t a12;//        *( sp + 15u )
	uint32_t a13;//        *( sp + 16u )
	uint32_t a14;//        *( sp + 17u )
	uint32_t a15;        //*( sp + 18u ) 
}exception_t;



	
typedef struct xIP_HEADER
{
	uint8_t ucVersionHeaderLength;        /*  0 + 1 =  1 */
	uint8_t ucDifferentiatedServicesCode; /*  1 + 1 =  2 */
	uint16_t usLength;                    /*  2 + 2 =  4 */
	uint16_t usIdentification;            /*  4 + 2 =  6 */
	uint16_t usFragmentOffset;            /*  6 + 2 =  8 */
	uint8_t ucTimeToLive;                 /*  8 + 1 =  9 */
	uint8_t ucProtocol;                   /*  9 + 1 = 10 */
	uint16_t usHeaderChecksum;            /* 10 + 2 = 12 */
	uint32_t ulSourceIPAddress;           /* 12 + 4 = 16 */
	uint32_t ulDestinationIPAddress;      /* 16 + 4 = 20 */
} IPHeader_t;
#define ipMAC_ADDRESS_LENGTH_BYTES ( 6 )
#define ipSIZE_OF_IPv4_HEADER			20u
#define ipSIZE_OF_ETH_HEADER			14u
typedef struct xMAC_ADDRESS
{
	uint8_t ucBytes[ ipMAC_ADDRESS_LENGTH_BYTES ];
} MACAddress_t;

typedef struct xETH_HEADER
{
	MACAddress_t xDestinationAddress; /*  0 + 6 = 6  */
	MACAddress_t xSourceAddress;      /*  6 + 6 = 12 */
	uint16_t usFrameType;              /* 12 + 2 = 14 */
} EthernetHeader_t;
typedef struct xIP_PACKET {
	EthernetHeader_t xEthernetHeader;
	IPHeader_t xIPHeader;
} IPPacket_t;
typedef struct xNETWORK_BUFFER
{
//	ListItem_t xBufferListItem; 	/* Used to reference the buffer form the free buffer list or a socket. */
	uint32_t ulIPAddress;			/* Source or destination IP address, depending on usage scenario. */
	uint8_t *pucEthernetBuffer; 	/* Pointer to the start of the Ethernet frame. */
	size_t xDataLength; 			/* Starts by holding the total Ethernet frame length, then the UDP/TCP payload length. */
	uint16_t usPort;				/* Source or destination port, depending on usage scenario. */
	uint16_t usBoundPort;			/* The port to which a transmitting socket is bound. */
	#if( ipconfigUSE_LINKED_RX_MESSAGES != 0 )
		struct xNETWORK_BUFFER *pxNextBuffer; /* Possible optimisation for expert users - requires network driver support. */
	#endif
} NetworkBufferDescriptor_t;


int freertos_cve2018_16601(const IPPacket_t * const pxIPPacket, NetworkBufferDescriptor_t * const pxNetworkBuffer) {

	const IPHeader_t * pxIPHeader = &( pxIPPacket->xIPHeader );
	uint32_t uxHeaderLength = ( uint32_t ) ( ( pxIPHeader->ucVersionHeaderLength & 0x0Fu ) << 2 );
	uint8_t ucProtocol;

	//asm volatile("ebreak");
		/* fixs: */
	//	if (uxHeaderLength > pxNetworkBuffer->xDataLength - ipSIZE_OF_ETH_HEADER ||
	 //	uxHeaderLength < ipSIZE_OF_IPv4_HEADER)
	//	{
	//		return -1;
	//	}

		// do some thing

		return 0;
    
}




void patch_cve2018_16601(uint32_t *sp){

    if(*(sp + 11)  > *(uint32_t *)(*(uint32_t *)(*(sp + 5) - 40 ) + 8) - 14u ||*(sp + 11)  < 20u){

	  *(sp + 11)  = -1;
      *(sp + 1) += 2u;
  }else{
      //back
      *(sp + 11) = 0;
	  *(sp + 1) += 2u;
  }

}




#define ipSIZE_TCP_OPTIONS   12u
struct xTCP_HEADER
{
	uint16_t usSourcePort;		/* +  2 =  2 */
	uint16_t usDestinationPort;	/* +  2 =  4 */
	uint32_t ulSequenceNumber;	/* +  4 =  8 */
	uint32_t ulAckNr;   	 	/* +  4 = 12 */
	uint8_t  ucTCPOffset;		/* +  1 = 13 */
	uint8_t  ucTCPFlags;		/* +  1 = 14 */
	uint16_t usWindow;			/* +  2 = 15 */
	uint16_t usChecksum;		/* +  2 = 18 */
	uint16_t usUrgent;			/* +  2 = 20 */

	/* the option data is not a part of the TCP header */
	uint8_t  ucOptdata[ipSIZE_TCP_OPTIONS];		/* + 12 = 32 */

};

typedef struct xTCP_HEADER TCPHeader_t;
struct xTCP_PACKET
{
	EthernetHeader_t xEthernetHeader; /*  0 + 14 = 14 */
	IPHeader_t xIPHeader;             /* 14 + 20 = 34 */
	TCPHeader_t xTCPHeader;           /* 34 + 32 = 66 */
};
#define TCP_OPT_END				0u
#define TCP_OPT_NOOP			1u

typedef struct xTCP_PACKET TCPPacket_t;

void cve2018_16524( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
TCPPacket_t * pxTCPPacket;
TCPHeader_t * pxTCPHeader;
const unsigned char *pucPtr;
const unsigned char *pucLast;
//TCPWindow_t *pxTCPWindow;
uint32_t uxNewMSS;

	pxTCPPacket = ( TCPPacket_t * ) ( pxNetworkBuffer->pucEthernetBuffer );
	pxTCPHeader = &pxTCPPacket->xTCPHeader;

	pucPtr = pxTCPHeader->ucOptdata;
	pucLast = pucPtr + (((pxTCPHeader->ucTCPOffset >> 4) - 5) << 2);
	//pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;

	while( pucPtr < pucLast )
	{
        uint32_t xRemainingOptionsBytes = pucLast - pucPtr;
        //printf("%ld\r\n",xRemainingOptionsBytes);
		if( pucPtr[ 0 ] == TCP_OPT_END )
		{
			break;
		}
		if( pucPtr[ 0 ] == TCP_OPT_NOOP)
		{

            pucPtr++;
            continue;
		}
   // fix
// if( xRemainingOptionsBytes < 2 )
// {
//    break;
//  }
		//test
		if(xRemainingOptionsBytes == 1){
		   printf("still vul\r\n");
		}
	}

}


void patch_cve2018_16524(uint32_t *sp){

    if( *(sp + 12) < 2){
    	*(sp + 1) -= 18u;

    }else{
	    *(sp + 9) += 1u;
	    *(sp + 1) += 4u;
    }

}




///cve2018-16528

#define pdFALSE    ( ( BaseType_t ) 0 )
#define pdTRUE     ( ( BaseType_t ) 1 )
#define ipARP_FRAME_TYPE	( 0x0608U )
#define pdPASS     ( pdTRUE )
#define pdFAIL     ( pdFALSE )
typedef uint32_t ( * NetworkRecv_t ) ( void * pvCallerContext,
                                         unsigned char * pucReceiveBuffer,
                                         size_t xReceiveLength );

typedef uint32_t ( * NetworkSend_t ) ( void * pvCallerContext,
                                         const unsigned char * pucData,
                                         size_t xDataLength );
typedef struct TLSContext
{
    const char * pcDestination;
    const char * pcServerCertificate;
    uint32_t ulServerCertificateLength;
    const char ** ppcAlpnProtocols;
    uint32_t ulAlpnProtocolsCount;

    NetworkRecv_t pxNetworkRecv;
    NetworkSend_t pxNetworkSend;
    void * pvCallerContext;

    /* mbedTLS. */
 //   mbedtls_ssl_context mbedSslCtx;
  //  mbedtls_ssl_config mbedSslConfig;
  //  mbedtls_x509_crt mbedX509CA;
  //  mbedtls_x509_crt mbedX509Cli;
  //  mbedtls_pk_context mbedPkCtx;
    uint32_t xMbedInitialized;

    /* PKCS#11. */
   // CK_FUNCTION_LIST_PTR pxP11FunctionList;
   // CK_SESSION_HANDLE xP11Session;
   // CK_OBJECT_HANDLE xP11PrivateKey;
   // CK_ULONG ulP11ModulusBytes;
} TLSContext_t;


 uint32_t TLS_Connect( void * pvContext )
{
    uint32_t xResult = 0;
    TLSContext_t * pCtx = ( TLSContext_t * ) pvContext; /*lint !e9087 !e9079 Allow casting void* to other types. */

   //do some thing
/*fixed*/
   /* if( 0 == xResult )
   {
      pCtx->xMbedInitialized = pdTRUE;
    }*/

   //do some thing
     if(pCtx->xMbedInitialized == pdTRUE){
		   printf("cve2018-16528 patch success");
			// __asm("nop");
		 }
    return xResult;
}


void test_cve2018_16528(void){
	TLSContext_t pCtx  = {
    .xMbedInitialized = 2,
  };
	BaseType_t xResult = TLS_Connect( &pCtx );
}



void patch_cve2018_16528(uint32_t *sp){

    if( *(sp + 11) == 0){
    	*(sp + 1) += 21u;

    }else{
	    *(sp + 11) = (uint32_t *)(*(sp + 10) + 16);
	    *(sp + 1) += 2u;
    }

}




//CVE-2020-10021

#define _u8  unsigned char
#define _i8  signed char
#define _u16 unsigned short
#define _i16 signed short
#define _u32 unsigned long
#define _i32 signed long

typedef signed char         s8_t;
typedef signed short        s16_t;
typedef signed int          s32_t;
typedef signed long long    s64_t;

typedef unsigned char       u8_t;
typedef unsigned short      u16_t;
typedef unsigned int        u32_t;
typedef unsigned long long  u64_t;

#define _volatile volatile
#define _const    const
typedef unsigned short int sa_family_t;
#define __packed __attribute__((__packed__))
struct CBW {
	u32_t Signature;
	u32_t Tag;
	u32_t DataLength;
	u8_t  Flags;
	u8_t  LUN;
	u8_t  CBLength;
	u8_t  CB[16];
} __packed;

struct CSW {
	u32_t Signature;
	u32_t Tag;
	u32_t DataResidue;
	u8_t  Status;
} __packed;
static struct CBW cbw;
static struct CSW csw;
static u32_t addr;
enum Status {
	CSW_PASSED,
	CSW_FAILED,
	CSW_ERROR,
};
#define BLOCK_SIZE	512
static void sendCSW(void)
{
	return;
}

#pragma GCC push_options
#pragma GCC optimize ("O0")

static bool infoTransfer(void)
{
	u32_t n;

	/* Logical Block Address of First Block */
	n = (cbw.CB[2] << 24) | (cbw.CB[3] << 16) | (cbw.CB[4] <<  8) |
				 (cbw.CB[5] <<  0);


	//fix
	//	if ((n * BLOCK_SIZE) >= memory_size) {
	//	csw.Status = CSW_FAILED;
	//	sendCSW();
	//	return false;
	//}



	addr = n * BLOCK_SIZE;

   //omit...
if (!cbw.DataLength) {              /* host requests no data*/

		csw.Status = CSW_FAILED;
		sendCSW();
		return false;
	}
  //omit...
  printf("addr is %d\r\n",addr);
	return true;
}
#pragma GCC pop_options



u32_t memory_size = 2 * BLOCK_SIZE;

void patch_cve2020_10021(uint32_t *sp){
    u32_t  abc = *(sp + 11u) << 9 ;
	if (abc >= memory_size){
		        *(sp + 11u) = -1;
		        *(sp + 1u) += 41u;
			}else{
				  *(sp + 12u) = *(sp + 11u) << 9;
			    *(sp + 1u) += 3u;
			}

 }









//CVE-2020-10023
size_t strlen(const char *s)
{
	size_t n = 0;

	while (*s != '\0') {
		s++;
		n++;
	}

	return n;
}
static inline u16_t shell_strlen(const char *str)
{
	return str == NULL ? 0U : (u16_t)strlen(str);
}

static inline int isspace(int c)
{
	return (int)(c == (int)' ' || ((unsigned)c-(unsigned)'\t') < 5U);
}


void *memmove(void *d, const void *s, size_t n)
{
	char *dest = d;
	const char *src  = s;

	if ((size_t) (dest - src) < n) {
		/*
		 * The <src> buffer overlaps with the start of the <dest> buffer.
		 * Copy backwards to prevent the premature corruption of <src>.
		 */

		while (n > 0) {
			n--;
			dest[n] = src[n];
		}
	} else {
		/* It is safe to perform a forward-copy */
		while (n > 0) {
			*dest = *src;
			dest++;
			src++;
			n--;
		}
	}

	return d;
}




void shell_spaces_trim(char *str)
{
	u16_t len = shell_strlen(str);
	u16_t shift = 0U;

	if (!str) {
		return;
	}

	for (u16_t i = 0; i < len - 1; i++) {
		if (isspace((int)str[i])) {
			for (u16_t j = i + 1; j < len; j++) {
				if (isspace((int)str[j])) {
					shift++;
					continue;
				}

				if (shift > 0) {
					/* +1 for EOS */
					memmove(&str[i + 1],
						&str[j],
						//len - shift + 1);
					len - j + 1);
					len -= shift;
					shift = 0U;
				}
				break;
			}
		}
	}
}

void patch_cve2020_10023(uint32_t *sp){
    *( sp + 15u) = *( sp + 16u ) - *( sp + 7u );
	
	  *(sp + 1u) += 3u;
			

 }

//CVE-2020-10062

#define MQTT_MAX_LENGTH_BYTES 4
#define MQTT_LENGTH_CONTINUATION_BIT 0x80
#define EINVAL 22
#define MQTT_MAX_PAYLOAD_SIZE 0x0FFFFFFF
#define MQTT_TRC printf
struct buf_ctx {
	u8_t *cur;
	u8_t *end;
};

int packet_length_decode(struct buf_ctx *buf, u32_t *length)
{
	u8_t shift = 0U;
	u8_t bytes = 0U;

	*length = 0U;
	do {
		if (bytes > MQTT_MAX_LENGTH_BYTES) {
		
			return -EINVAL;
		}
		bytes++;
	} while ((*(buf->cur++) & MQTT_LENGTH_CONTINUATION_BIT) != 0U);
//
//	if (*length > MQTT_MAX_PAYLOAD_SIZE) {
//		return -EINVAL;
//	}
//
	MQTT_TRC("length:0x%08x", *length);

	return 0;
}
void patch_cve2020_10062(uint32_t *sp){
  
		if (  *(sp + 11u)> MQTT_MAX_PAYLOAD_SIZE) {
		*(sp + 5u) =  -EINVAL;

    *(sp + 1u) += 44u;
	}else{
    *(sp + 11u) = (uint32_t *)(*(sp + 6u) +0 );
   // s32i.n	a8, a3, 0;
    *(sp + 1u) += 2u;

  }
 }        

//2020-10063


static int read_u8(u8_t *data, u16_t offset, u16_t *pos,
		   u16_t max_len, u8_t *value)
{
	if (max_len - offset < 1) {
		return -EINVAL;
	}
	*value = data[offset++];
	*pos = offset;
	return max_len - offset;
}
static u8_t option_header_get_delta(u8_t opt)
{
	return (opt & 0xF0) >> 4;
}
static u8_t option_header_get_len(u8_t opt)
{
	return opt & 0x0F;
}

static int decode_delta(u8_t *data, u16_t offset, u16_t *pos, u16_t max_len,
			u16_t opt, u16_t *opt_ext, u16_t *hdr_len)
{
	int ret = 0;

	return ret;
}

static inline bool u16_add_overflow(u16_t a, u16_t b, u16_t *result)
{
	u16_t c = a + b;

	*result = c;

	return c < a;
}
struct coap_option {
	u16_t delta;
#if defined(CONFIG_COAP_EXTENDED_OPTIONS_LEN)
	u16_t len;
	u8_t value[CONFIG_COAP_EXTENDED_OPTIONS_LEN_VALUE];
#else
	u8_t len;
	u8_t value[12];
#endif
};
#define COAP_MARKER		0xFF
#define COAP_OPTION_NO_EXT 12

 int parse_option(u8_t *data, u16_t offset, u16_t *pos,
			u16_t max_len, u16_t *opt_delta, u16_t *opt_len,
			struct coap_option *option)
{
	u16_t hdr_len;
	u16_t delta;
	u16_t len;
	u8_t opt;
	int r;
	r = read_u8(data, offset, pos, max_len, &opt);
	if (r < 0) {
		return r;
	}
	*opt_len += 1U;
	if (opt == COAP_MARKER) {
		return r > 0 ? 0 : -EINVAL;
	}
	delta = option_header_get_delta(opt);
	len = option_header_get_len(opt);
	if (r == 0 && delta > COAP_OPTION_NO_EXT) {
		return -EINVAL;
	}
	if (delta > COAP_OPTION_NO_EXT) {
		r = decode_delta(data, *pos, pos, max_len,
				 delta, &delta, &hdr_len);
		if ((r < 0) || (r == 0 && len > COAP_OPTION_NO_EXT)) {
			return -EINVAL;
		}
		*opt_len += hdr_len;//
		if (u16_add_overflow(*opt_len, hdr_len, opt_len)) {
			return -EINVAL;
		}
	}
	if (len > COAP_OPTION_NO_EXT) {
			return -EINVAL;
		}
		*opt_len += hdr_len;//
	//	if (u16_add_overflow(*opt_len, hdr_len, opt_len)) {
	//		return -EINVAL;
	//	}

	*opt_delta += delta;//
	*opt_len += len;//
	//if (u16_add_overflow(*opt_delta, delta, opt_delta) ||
	 //  u16_add_overflow(*opt_len, len, opt_len)) {
	//	return -EINVAL;
	//}
	if (r == 0) {
		if (len == 0U) {

			return -EINVAL;
		}
	} else {
		*pos += len;//
	  //if (u16_add_overflow(*pos, len, pos)) {
		//	return -EINVAL;
		//}
		r = max_len - *pos;
	}
	return r;
}

void patch_cve2020_10063(uint32_t *sp){
  *(sp + 11u) =  *(sp + 11u) + *(sp + 12u);
  	u16_t c = *(sp + 11u) + *(sp + 12u);
	   if( c < *(sp + 11u)) {
		*(sp + 5u) =  -EINVAL;
    *(sp + 1u) += 44u;
	}else{
   
     *(sp + 11u) =  *(sp + 11u) + *(sp + 12u);
    *(sp + 1u) += 3u;
  }
 }        

 // CVE_2020_17445
#define NRF5_PSDU_LENGTH 10
#define NRF5_FCS_LENGTH 5
	#define PICO_IPV6_EXTHDR_OPT_PAD1 0
#define PICO_IPV6_EXTHDR_OPT_PADN 1
#define PICO_IPV6_EXTHDR_OPT_SRCADDR 201
int pico_ipv6_process_destopt(uint8_t *destopt, uint8_t *f, uint32_t opt_ptr)
 {
     uint8_t *option = NULL;
     uint8_t len = 0, optlen = 0;
     option = (destopt + 2); /* Skip Dest_opts header */
     len = (uint8_t)(((*(destopt + 1) + 1) << 3) - 2); /* len in bytes, minus nxthdr and len byte */
   //  if((optlen > len ) || (optlen == 0))
 		//	return -1;
 	  while (len) {
         optlen = (uint8_t)(*(option + 1) + 2);

         switch (*option)
         {
         case PICO_IPV6_EXTHDR_OPT_PAD1:
             break;

         case PICO_IPV6_EXTHDR_OPT_PADN:
             break;

         case PICO_IPV6_EXTHDR_OPT_SRCADDR:
             // ipv6_dbg("IPv6: home address option with length %u\n", optlen);
             break;

         default:
             // THE CODE HERE IS OMITTED FOR BREVITY
             break;
         }
         opt_ptr += optlen;
         option += optlen;
         len = (uint8_t)(len - optlen);
     }
     return 0;
 }

 #pragma GCC push_options
#pragma GCC optimize ("O0")
void patch_cve2020_17445(uint32_t *sp){

	   if((*(sp + 13u) >  (*(sp + 12u) ) -2) || (*(sp + 13u) == 0)) {
		*(sp + 5u) =  -1;
    *(sp + 1u) += 30u;
	}else{
     *(sp + 12u) =  *(sp + 12u) -2; //addi.n	a9, a9,-2
    *(sp + 1u) += 2u;
  }
 }      
 #pragma GCC push_options



//cve-2020-10024


#define _SVC_CALL_SYSTEM_CALL    3

static inline uintptr_t arch_syscall_invoke0(uintptr_t call_id)
{
	register u32_t ret __asm__("a0");
	register u32_t r6 __asm__("a6");
     r6  = call_id;



	return ret;
}

static volatile int expected_reason = -1;
static volatile bool expect_fault;

enum k_fatal_error_reason {
	K_ERR_CPU_EXCEPTION,
	K_ERR_SPURIOUS_IRQ,
	K_ERR_STACK_CHK_FAIL,
	K_ERR_KERNEL_OOPS,
	K_ERR_KERNEL_PANIC
};

#define INT_MAX		2147483647
#define UINT_MAX	0xFFFFFFFFU

 void test_bad_syscall(void)
{
	expect_fault = true;
	expected_reason = K_ERR_KERNEL_OOPS;
	arch_syscall_invoke0(INT_MAX);

//	expect_fault = true;
//	expected_reason = K_ERR_KERNEL_OOPS;

	//arch_syscall_invoke0(UINT_MAX);

}



void patch_CVE_2020_10024(uint32_t *sp){
	   *(sp + 14u) = 3;

	    uint32_t result = 0xFFFFFFFEU + 1;
		if( result < 0xFFFFFFFFU ){
			  while(1);
		}else{
			  *(sp + 0u) += 2u;
              return;
		}
}


 //cve-2020-10028


 struct _snode {
 	struct _snode *next;
 };

 typedef struct _snode sys_snode_t;
 struct device {
 const char *name;const void *config;
       const void *driver_api;
       void * const data;
 };
 struct gpio_callback;
 typedef void (*gpio_callback_handler_t)(struct device *port,
 					struct gpio_callback *cb,
 					u32_t pins);

 struct gpio_callback {
 	/** This is meant to be used in the driver and the user should not
 	 * mess with it (see drivers/gpio/gpio_utils.h)
 	 */
 	sys_snode_t node;

 	/** Actual callback function being called when relevant. */
 	gpio_callback_handler_t handler;

 	/** A mask of pins the callback is interested in, if 0 the callback
 	 * will never be called. Such pin_mask can be modified whenever
 	 * necessary by the owner, and thus will affect the handler being
 	 * called or not. The selected pins must be configured to trigger
 	 * an interrupt.
 	 */
 	u32_t pin_mask;
 };



 struct gpio_driver_api {
 	int (*config)(struct device *port, int access_op, u32_t pin, int flags);
 	int (*write)(struct device *port, int access_op, u32_t pin,
 		     u32_t value);
 	int (*read)(struct device *port, int access_op, u32_t pin,
 		    u32_t *value);
 	int (*manage_callback)(struct device *port, struct gpio_callback *cb,
 			       bool set);
 	int (*enable_callback)(struct device *port, int access_op, u32_t pin);
 	int (*disable_callback)(struct device *port, int access_op, u32_t pin);
 	u32_t (*get_pending_int)(struct device *dev);
 };
 #define ENOTSUP 35

 static inline int z_impl_gpio_enable_callback(struct device *port,
 					     int access_op, u32_t pin)
 {
 	const struct gpio_driver_api *api =
 		(const struct gpio_driver_api *)port->driver_api;

 	if (api->enable_callback == NULL) {
 		return -ENOTSUP;
 	}

 	return api->enable_callback(port, access_op, pin);
 }





int z_vrfy_gpio_enable_callback(struct device *port,
 					     int access_op, u32_t pin)
 {

   //  Z_OOPS(Z_SYSCALL_DRIVER_GPIO(port, enable_callback));

 	return z_impl_gpio_enable_callback((struct device *)port, access_op,
 					  pin);
 }

 void patch_CVE_2020_10028(uint32_t *sp){
        uint32_t *gpio_enable_callback_ptr =  *(sp + 10);
		if( *gpio_enable_callback_ptr == 0 ){
			 *(sp + 10) = -EINVAL;
			 *(sp + 1u) += 14u;
		}else{
			*(sp + 6) = *(sp + 9)  + 4;
			  *(sp + 1u) += 4u;
		}
}




//2020-17443
struct __attribute__((packed)) pico_ip6
{
    uint8_t addr[16];
};
typedef uint64_t pico_time;
struct pico_frame {

    /* Connector for queues */
    struct pico_frame *next;
    unsigned char *buffer;
    uint32_t buffer_len;
    unsigned char *start;
    uint32_t len;
    uint32_t *usage_count;
    uint8_t *datalink_hdr;
    uint8_t *net_hdr;
    uint16_t net_len;
    uint8_t *transport_hdr;
    uint16_t transport_len;
    uint8_t *app_hdr;
    uint16_t app_len;

    struct pico_device *dev;
    pico_time timestamp;
    /* Failures due to bad datalink addressing. */
    uint16_t failure_count;
    /* Protocol over IP */
    uint8_t proto;
    /* PICO_FRAME_FLAG_* */
    uint8_t flags;
    /* Pointer to payload */
    unsigned char *payload;
    uint16_t payload_len;
#if defined(PICO_SUPPORT_IPV4FRAG) || defined(PICO_SUPPORT_IPV6FRAG)
    /* Payload fragmentation info */
    uint16_t frag;
#endif

#if defined(PICO_SUPPORT_6LOWPAN)
    uint32_t hash;
    union pico_ll_addr src;
    union pico_ll_addr dst;
#endif

    struct pico_socket *sock;
    void *info;
    int8_t priority;
    uint8_t transport_flags_saved;

    void (*notify_free)(uint8_t *);

    uint8_t send_ttl; /* Special TTL/HOPS value, 0 = auto assign */
    uint8_t send_tos; /* Type of service */
};
#define MAX_DEVICE_NAME 16
struct pico_device {
    char name[MAX_DEVICE_NAME];
    uint32_t hash;
    uint32_t overhead;
    uint32_t mtu;
 //   struct pico_ethdev *eth; /* Null if non-ethernet */
  //  enum pico_ll_mode mode;
   // struct pico_queue *q_in;
  //  struct pico_queue *q_out;
   // int (*link_state)(struct pico_device *self);
  //  int (*send)(struct pico_device *self, void *buf, int len); /* Send function. Return 0 if busy */
   // int (*poll)(struct pico_device *self, int loop_score);
  //  void (*destroy)(struct pico_device *self);
  #ifdef PICO_SUPPORT_TICKLESS
    void (*wfi)(struct pico_device *self, int timeout);
  #endif
   // int (*dsr)(struct pico_device *self, int loop_score);
   // int __serving_interrupt;
    /* used to signal the upper layer the number of events arrived since the last processing */
    volatile int eventCnt;
  #ifdef PICO_SUPPORT_IPV6
    struct pico_nd_hostvars hostvars;
  #endif
  //  struct pico_stack *stack;
};
#define MAX_PROTOCOL_NAME 16
struct pico_protocol {
    char name[MAX_PROTOCOL_NAME];
    uint32_t hash;

    uint16_t proto_number;
    struct pico_queue *q_in;
    struct pico_queue *q_out;
    struct pico_frame *(*alloc)(struct pico_protocol *self, struct pico_device *dev, uint16_t size); /* Frame allocation. */
    int (*push)(struct pico_protocol *self, struct pico_frame *p);    /* Push function, for active outgoing pkts from above */
    int (*process_out)(struct pico_protocol *self, struct pico_frame *p);  /* Send loop. */
    int (*process_in)(struct pico_protocol *self, struct pico_frame *p);  /* Recv loop. */
    uint16_t (*get_mtu)(struct pico_protocol *self);
};

static struct pico_frame *pico_ipv6_alloc(struct pico_protocol *self, struct pico_device *dev, uint16_t size)
{
    struct pico_frame *f = NULL;

    return f;
}

struct pico_protocol pico_proto_ipv6 = {
    .name = "ipv6",
    .alloc = pico_ipv6_alloc,

};
enum pico_err_e {
    PICO_ERR_NOERR = 0,
    PICO_ERR_EPERM = 1,
    PICO_ERR_ENOENT = 2,
    /* ... */
    PICO_ERR_EINTR = 4,
    PICO_ERR_EIO = 5,
    PICO_ERR_ENXIO = 6,
    /* ... */
    PICO_ERR_EAGAIN = 11,
    PICO_ERR_ENOMEM = 12,
    PICO_ERR_EACCESS = 13,
    PICO_ERR_EFAULT = 14,
    /* ... */
    PICO_ERR_EBUSY = 16,
    PICO_ERR_EEXIST = 17,
    /* ... */
    PICO_ERR_EINVAL = 22,
    /* ... */
    PICO_ERR_ENONET = 64,
    /* ... */
    PICO_ERR_EPROTO = 71,
    /* ... */
    PICO_ERR_ENOPROTOOPT = 92,
    PICO_ERR_EPROTONOSUPPORT = 93,
    /* ... */
    PICO_ERR_EOPNOTSUPP = 95,
    PICO_ERR_EADDRINUSE = 98,
    PICO_ERR_EADDRNOTAVAIL = 99,
    PICO_ERR_ENETDOWN = 100,
    PICO_ERR_ENETUNREACH = 101,
    /* ... */
    PICO_ERR_ECONNRESET = 104,
    /* ... */
    PICO_ERR_EISCONN = 106,
    PICO_ERR_ENOTCONN = 107,
    PICO_ERR_ESHUTDOWN = 108,
    /* ... */
    PICO_ERR_ETIMEDOUT = 110,
    PICO_ERR_ECONNREFUSED = 111,
    PICO_ERR_EHOSTDOWN = 112,
    PICO_ERR_EHOSTUNREACH = 113,
};
typedef enum pico_err_e pico_err_t;
volatile pico_err_t pico_err;


int pico_icmp6_send_echoreply(struct pico_frame *echo)
{
    struct pico_frame *reply = NULL;
    struct pico_icmp6_hdr *ehdr = NULL, *rhdr = NULL;
    struct pico_ip6 src;
    struct pico_ip6 dst;
  //   if(echo->transport_len <8){
	//	 return -1;
	//	 }
    reply = pico_proto_ipv6.alloc(&pico_proto_ipv6, echo->dev, (uint16_t)(echo->transport_len));
    if (!reply) {
        pico_err = PICO_ERR_ENOMEM;
        return -1;
    }

    //...
    return 0;
}

void patch_CVE_2020_17443(uint32_t *sp){

		if( *(sp + 15u) < 8 ){
      
			 *(sp + 1u) += 16u;
		}else{
			 *(sp + 13u) = *(sp + 12u)  | *(sp + 12u);
			  *(sp + 1u) += 3u;
		}
}

//CVE-2017-2784
typedef uint32_t mbedtls_mpi_uint;
typedef struct
{
    int s;              /*!<  integer sign      */
    size_t n;           /*!<  total # of limbs  */
    mbedtls_mpi_uint *p;          /*!<  pointer to limbs  */
}
mbedtls_mpi;
#define MBEDTLS_MPI_MAX_LIMBS                             10000
#define MBEDTLS_ERR_MPI_ALLOC_FAILED                      -0x0010
#define ciL    (sizeof(mbedtls_mpi_uint))

static void mbedtls_mpi_zeroize( mbedtls_mpi_uint *v, size_t n ) {
    volatile mbedtls_mpi_uint *p = v; while( n-- ) *p++ = 0;
}
static void mbedtls_free( mbedtls_mpi_uint *v) {
     mbedtls_mpi_uint *p = v;
     while( 1 ) *p++ = 0;
}



int mbedtls_mpi_grow( mbedtls_mpi
	*X, size_t nblimbs )
{
    mbedtls_mpi_uint *p = 2;

    if( nblimbs > MBEDTLS_MPI_MAX_LIMBS )
        return( MBEDTLS_ERR_MPI_ALLOC_FAILED );

    if( X->n < nblimbs )
    {
       //...

        if( X->p != NULL )
        {
           // memcpy( p, X->p, X->n * ciL );
            mbedtls_mpi_zeroize( X->p, X->n );
            mbedtls_free( X->p ); //
        }

        X->n = nblimbs;
        X->p = p;
    }

    return( 0 );
}

void patch_CVE_2017_2784(uint32_t *sp){
			  *(sp + 1u) += 7u;

}
void app_main(void){
//    u8_t abc[2] = {0,1};
//    void *str = abc;
//    u8_t abc2[2] = {0,1};
//    u8_t *str2 = abc;
//    int hhhh = 2;
    mbedtls_mpi_uint *pin = 2;
//    struct device port = {
//    .data = str,

//    };
  //  

 //  test_bad_syscall();
 //   test_cve2018_16601();
   // test_cve2018_16528();
//bool abcd =  infoTransfer();

    exception_t a = {
       
       .a0 = 2u,
       .a1 = 2u,
       .a2 = 2u,
       .a3 = 2u,
       .a4 = 2u,
       .a5 = 2u,
       .a6 = 2u,
       .a7 = 2u,
       .a8 = 2u,
       .a9 = 2u,
       .a10 = 2u,
       .a11= 2u,
       .a12 = 2u,
       .a13 = 2u,
       .a14 = 2u,
       .a15 = 2u,
       .pc = 50u,
    };
    struct exception_info *p = &a;
    uint32_t *sp = (uint32_t*)p;
    
    int i =1;
    int cntStart,cntEnd;
    while(1){
    printf("No.%d test\r\n",i);
    printf("\r\n");
    //__asm("break 0,0");
   // cpu_hal_set_cycle_count(1u);
    cntStart = cpu_hal_get_cycle_count();//2


    
    patch_CVE_2017_2784(sp);
    __asm("nop");
    
    
    //vTaskDelay(1000 / portTICK_PERIOD_MS);
    cntEnd = cpu_hal_get_cycle_count();
    // printf("cntStart is %d\r\n",cntStart);
    //  printf("\r\n");
    // printf("cntEnd is %d\r\n",cntEnd);
    //  printf("\r\n");
    //printf("successsssss\r\n");
      printf("cycle is %d\r\n",cntEnd-cntStart - 2);
      printf("\r\n");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
   //  int n = 65530;
   // int m = cve(1,n);
   // printf("m = %d\r\n",m);
   i++;
    }
 
  
}



