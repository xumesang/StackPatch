#ifndef _FREERTOSCVE_H
#define _FREERTOSCVE_H


#include "usart.h"
#include "FreeRTOS.h"
#define ipMAC_ADDRESS_LENGTH_BYTES ( 6 )
#define ipIP_ADDRESS_LENGTH_BYTES ( 4 )
#define ipSIZE_OF_ETH_HEADER			14u
#define ipSIZE_OF_IPv4_HEADER			20u
#define ipSIZE_OF_TCP_HEADER		20u

   
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
#define ipSIZE_TCP_OPTIONS   12u
typedef struct UDPSOCKET
{
	//List_t xWaitingPacketsList;	/* Incoming packets */
	#if( ipconfigUDP_MAX_RX_PACKETS > 0 )
		UBaseType_t uxMaxPackets; /* Protection: limits the number of packets buffered per socket */
	#endif /* ipconfigUDP_MAX_RX_PACKETS */
	#if( ipconfigUSE_CALLBACKS == 1 )
		FOnUDPReceive_t pxHandleReceive;	/*
											 * In case of a UDP socket:
											 * typedef void (* FOnUDPReceive_t) (Socket_t xSocket, void *pData, size_t xLength, struct freertos_sockaddr *pxAddr );
											 */
		FOnUDPSent_t pxHandleSent;
	#endif /* ipconfigUSE_CALLBACKS */
} IPUDPSocket_t;
typedef struct XSOCKET
{
//	EventBits_t xEventBits;
//	EventGroupHandle_t xEventGroup;

//	ListItem_t xBoundSocketListItem; /* Used to reference the socket from a bound sockets list. */
//	TickType_t xReceiveBlockTime; /* if recv[to] is called while no data is available, wait this amount of time. Unit in clock-ticks */
//	TickType_t xSendBlockTime; /* if send[to] is called while there is not enough space to send, wait this amount of time. Unit in clock-ticks */

	uint16_t usLocalPort;		/* Local port on this machine */
	uint8_t ucSocketOptions;
	uint8_t ucProtocol; /* choice of FREERTOS_IPPROTO_UDP/TCP */
	#if( ipconfigSOCKET_HAS_USER_SEMAPHORE == 1 )
		SemaphoreHandle_t pxUserSemaphore;
	#endif /* ipconfigSOCKET_HAS_USER_SEMAPHORE */
	#if( ipconfigSOCKET_HAS_USER_WAKE_CALLBACK == 1 )
		SocketWakeupCallback_t pxUserWakeCallback;
	#endif /* ipconfigSOCKET_HAS_USER_WAKE_CALLBACK */

	#if( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
		struct xSOCKET_SET *pxSocketSet;
		/* User may indicate which bits are interesting for this socket. */
		EventBits_t xSelectBits;
		/* These bits indicate the events which have actually occurred.
		They are maintained by the IP-task */
		EventBits_t xSocketBits;
	#endif /* ipconfigSUPPORT_SELECT_FUNCTION */
	/* TCP/UDP specific fields: */
	/* Before accessing any member of this structure, it should be confirmed */
	/* that the protocol corresponds with the type of structure */

	union
	{
		IPUDPSocket_t xUDP;
		#if( ipconfigUSE_TCP == 1 )
			IPTCPSocket_t xTCP;
			/* Make sure that xTCP is 8-bytes aligned by
			declaring a 64-bit variable in the same union */
			uint64_t ullTCPAlignment;
		#endif /* ipconfigUSE_TCP */
	} u;
} FreeRTOS_Socket_t;

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

typedef struct xIP_PACKET {
	EthernetHeader_t xEthernetHeader;
	IPHeader_t xIPHeader;
} IPPacket_t;
typedef long BaseType_t;
typedef struct xETH_HEADER EthernetHeader_t;
typedef struct xTCP_HEADER TCPHeader_t;
struct xTCP_PACKET
{
	EthernetHeader_t xEthernetHeader; /*  0 + 14 = 14 */
	IPHeader_t xIPHeader;             /* 14 + 20 = 34 */
	TCPHeader_t xTCPHeader;           /* 34 + 32 = 66 */
};

typedef struct xTCP_PACKET TCPPacket_t;

#define pdFALSE    ( ( BaseType_t ) 0 )
#define pdTRUE     ( ( BaseType_t ) 1 )
#define ipARP_FRAME_TYPE	( 0x0608U )
#define pdPASS     ( pdTRUE )
#define pdFAIL     ( pdFALSE )
typedef BaseType_t ( * NetworkRecv_t ) ( void * pvCallerContext,
                                         unsigned char * pucReceiveBuffer,
                                         size_t xReceiveLength );

/**
 * @brief Defines callback type for sending bytes to the network.
 *
 * @param[in] pvCallerContext Opaque context handle provided by caller.
 * @param[out] pucReceiveBuffer Buffer of data to send.
 * @param[in] xReceiveLength Length of previous parameter in bytes.
 *
 * @return The number of bytes actually sent.
 */
typedef BaseType_t ( * NetworkSend_t ) ( void * pvCallerContext,
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
    BaseType_t xMbedInitialized;

    /* PKCS#11. */
   // CK_FUNCTION_LIST_PTR pxP11FunctionList;
   // CK_SESSION_HANDLE xP11Session;
   // CK_OBJECT_HANDLE xP11PrivateKey;
   // CK_ULONG ulP11ModulusBytes;
} TLSContext_t;

#define dnsNBNS_ENCODED_NAME_LENGTH			32
typedef enum
{
	eReleaseBuffer = 0,		/* Processing the frame did not find anything to do - just release the buffer. */
	eProcessBuffer,			/* An Ethernet frame has a valid address - continue process its contents. */
	eReturnEthernetFrame,	/* The Ethernet frame contains an ARP or ICMP packet that can be returned to its source. */
	eFrameConsumed			/* Processing the Ethernet packet contents resulted in the payload being sent to the stack. */
} eFrameProcessingResult_t;
typedef unsigned long UBaseType_t;
struct xNBNSRequest
{
		uint16_t usRequestId;
		uint16_t usFlags;
		uint16_t ulRequestCount;
		uint16_t usAnswerRSS;
		uint16_t usAuthRSS;
		uint16_t usAdditionalRSS;
		uint8_t ucNameSpace;
	  uint8_t ucName[ dnsNBNS_ENCODED_NAME_LENGTH ];
		uint8_t ucNameZero;
		uint16_t usType;
		uint16_t usClass;
};
struct xARP_HEADER
{
	uint16_t usHardwareType;
	uint16_t usProtocolType;
	uint8_t ucHardwareAddressLength;
	uint8_t ucProtocolAddressLength;
	uint16_t usOperation;
	//xMACAddress_t xSenderHardwareAddress;
	uint32_t ulSenderProtocolAddress;
	//xMACAddress_t xTargetHardwareAddress;
	uint32_t ulTargetProtocolAddress;
};
typedef struct xETH_HEADER xEthernetHeader_t;
typedef struct xARP_HEADER xARPHeader_t;
struct xARP_PACKET
{
	xEthernetHeader_t xEthernetHeader;
	xARPHeader_t xARPHeader;
};
typedef void * StreamBufferHandle_t;
typedef struct xARP_PACKET ARPPacket_t;
typedef struct xNBNSRequest NBNSRequest_t;
#define TCP_OPT_END				0u
#define TCP_OPT_NOOP			1u


typedef struct xSTREAM_BUFFER /*lint !e9058 Style convention uses tag. */
{
	volatile size_t xTail;				/* Index to the next item to read within the buffer. */
	volatile size_t xHead;				/* Index to the next item to write within the buffer. */
	size_t xLength;						/* The length of the buffer pointed to by pucBuffer. */
	size_t xTriggerLevelBytes;			/* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
	//volatile TaskHandle_t xTaskWaitingToReceive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
	//volatile TaskHandle_t xTaskWaitingToSend;	/* Holds the handle of a task waiting to send data to a message buffer that is full. */
	uint8_t *pucBuffer;					/* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
	uint8_t ucFlags;


} StreamBuffer_t;

typedef struct QueueDefinition
{
	int8_t *pcHead;					/*< Points to the beginning of the queue storage area. */
	int8_t *pcTail;					/*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
	int8_t *pcWriteTo;				/*< Points to the free next place in the storage area. */

	union							/* Use of a union is an exception to the coding standard to ensure two mutually exclusive structure members don't appear simultaneously (wasting RAM). */
	{
		int8_t *pcReadFrom;			/*< Points to the last place that a queued item was read from when the structure is used as a queue. */
		UBaseType_t uxRecursiveCallCount;/*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
	} u;

	volatile UBaseType_t uxMessagesWaiting;/*< The number of items currently in the queue. */
	UBaseType_t uxLength;			/*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
	UBaseType_t uxItemSize;			/*< The size of each items that the queue will hold. */

	volatile int8_t cRxLock;		/*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
	volatile int8_t cTxLock;		/*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */


} xQUEUE;
typedef xQUEUE Queue_t;
int freertos_cve2018_16601(const IPPacket_t * const pxIPPacket, NetworkBufferDescriptor_t * const pxNetworkBuffer);
void test_cve2018_16601(void);
static eFrameProcessingResult_t freertos_cve2018_16603( IPPacket_t * const pxIPPacket, NetworkBufferDescriptor_t * const pxNetworkBuffer );

void test_cve2018_16603(void);
BaseType_t TLS_Connect( void * pvContext );
void test_cve2018_16528(void);
void test_cve2018_16526(void);
void test_cve2018_16523(void);
void test_cve2018_16524(void);
void test_cve2018_16599(void);
void test_cve2018_16600(void);
void test_cve2021_31512(void);
void test_cve2021_16602(void);
void test_cve2021_31571(void);


#endif
