#include "./SYSTEM/sys/sys.h"
#include "freertoscve.h"
#include "patch_delay_test.h"
#include "led.h"
//CVE2018-16601 
int freertos_cve2018_16601(const IPPacket_t * const pxIPPacket, NetworkBufferDescriptor_t * const pxNetworkBuffer) {

	const IPHeader_t * pxIPHeader = &( pxIPPacket->xIPHeader );
	uint32_t uxHeaderLength = ( uint32_t ) ( ( pxIPHeader->ucVersionHeaderLength & 0x0Fu ) << 2 );
	uint8_t ucProtocol;
	   
	/* fixs: 
	if (uxHeaderLength > pxNetworkBuffer->xDataLength - ipSIZE_OF_ETH_HEADER ||
 	uxHeaderLength < ipSIZE_OF_IPv4_HEADER)
 	{
		return -1;
	}*/
	// do some thing
	return 0;
}



void test_cve2018_16601(void){//errValue 50 0 CorrectValue 7 60
	IPPacket_t pkt = {
		.xIPHeader = {
			.ucVersionHeaderLength = 50,
		}
	};
	NetworkBufferDescriptor_t data = {
		.xDataLength = 0,
	};
	  int i;
		uint32_t startTime,endTime,q;
	  float testtime;
		dwt_init();
			startTime = dwt_get_counter();
	    i = freertos_cve2018_16601(&pkt,&data);
			  __ASM("nop");
				__ASM("nop"); 
			endTime = dwt_get_counter();
		  q = endTime - startTime - 14;
		  testtime = cycles_to_us(q);
	    printf("cpu cycle is %u\r\n",q);
		  printf("delay is %f us\r\n",testtime);
		dwt_deinit();
      
     if( i == -1){ 
		   printf("cve2018-16601 patch success!\r\n");
		 }else{
		   printf("cve2018-16601 still vulnerable\r\n");
		 }
}


static eFrameProcessingResult_t freertos_cve2018_16603( IPPacket_t * const pxIPPacket, NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
eFrameProcessingResult_t eReturn;
IPHeader_t * pxIPHeader = &( pxIPPacket->xIPHeader );
UBaseType_t uxHeaderLength = ( UBaseType_t ) ( ( pxIPHeader->ucVersionHeaderLength & 0x0Fu ) << 2 );
uint8_t ucProtocol;

    /* Bound the calculated header length: take away the Ethernet header size,
    then check if the IP header is claiming to be longer than the remaining
    total packet size. Also check for minimal header field length. */
  /*  if( uxHeaderLength > pxNetworkBuffer->xDataLength - ipSIZE_OF_ETH_HEADER ||
        uxHeaderLength < ipSIZE_OF_IPv4_HEADER )
    {
        return 0;
    }*/

	ucProtocol = pxIPPacket->xIPHeader.ucProtocol;
	/* Check if the IP headers are acceptable and if it has our destination. */
	return 1;
	
}

void test_cve2018_16603(void){//errValue 50 0 CorrectValue 7 60   bkpt:0x200040b0
	IPPacket_t pkt = {
		.xIPHeader = {
			.ucVersionHeaderLength = 7,
		}
	};
	NetworkBufferDescriptor_t data = {
		.xDataLength = 60,
	};
	
	int i;
		uint32_t startTime,endTime,q;
	  float testtime;
		dwt_init();
			startTime = dwt_get_counter();
	    i = freertos_cve2018_16603(&pkt,&data);
			  __ASM("nop");
				__ASM("nop");
			endTime = dwt_get_counter();
		  q = endTime-startTime;
		  testtime = cycles_to_us(q);
	    printf("cpu cycle is %u\r\n",q);
		  printf("delay is %f us\r\n",testtime);
		dwt_deinit();
     
     if( i == 0){
		   printf("cve2018-16603 patch success!\r\n");
		 }else{
		   printf("cve2018-16603 still vulnerable\r\n");
		 }
}

BaseType_t TLS_Connect( void * pvContext )
{
    BaseType_t xResult = 0;
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
	
	TLSContext_t data = {
		.xMbedInitialized = pdFALSE,//pdFALSE
	};
	int i;
		uint32_t startTime,endTime,q;
	  float testtime;
		dwt_init();
			startTime = dwt_get_counter();
	    i = TLS_Connect(&data);;
			  __ASM("nop");
				__ASM("nop");
			endTime = dwt_get_counter();
		  q = endTime-startTime;
		  testtime = cycles_to_us(q);
	    printf("cpu cycle is %u\r\n",q);
		  printf("delay is %f us\r\n",testtime);
		dwt_deinit();
    // int i = TLS_Connect(&data);
     
}

/*cve16526*/
static eFrameProcessingResult_t cve16526( IPPacket_t * const pxIPPacket, NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
eFrameProcessingResult_t eReturn;
IPHeader_t * pxIPHeader = &( pxIPPacket->xIPHeader );
UBaseType_t uxHeaderLength = ( UBaseType_t ) ( ( pxIPHeader->ucVersionHeaderLength & 0x0Fu ) << 2 );
uint8_t ucProtocol;

    /* Bound the calculated header length: take away the Ethernet header size,
    then check if the IP header is claiming to be longer than the remaining
    total packet size. Also check for minimal header field length. */
    if( uxHeaderLength > pxNetworkBuffer->xDataLength - ipSIZE_OF_ETH_HEADER ||
        uxHeaderLength < ipSIZE_OF_IPv4_HEADER )
    {
        return eReleaseBuffer;
    }

	ucProtocol = pxIPPacket->xIPHeader.ucProtocol;
	/* Check if the IP headers are acceptable and if it has our destination. */
	eReturn = eProcessBuffer;

	if( eReturn == eProcessBuffer )
	{
		if( uxHeaderLength > ipSIZE_OF_IPv4_HEADER )
		{
			 
			const size_t optlen = ( ( size_t ) uxHeaderLength ) - ipSIZE_OF_IPv4_HEADER;
			/* From: the previous start of UDP/ICMP/TCP data */
		//	uint8_t *pucSource = ( uint8_t* )(pxNetworkBuffer->pucEthernetBuffer + sizeof( EthernetHeader_t ) + uxHeaderLength);
			/* To: the usual start of UDP/ICMP/TCP data at offset 20 from IP header */
		//uint8_t *pucTarget = ( uint8_t* )(pxNetworkBuffer->pucEthernetBuffer + sizeof( EthernetHeader_t ) + ipSIZE_OF_IPv4_HEADER);
			/* How many: total length minus the options and the lower headers */
			//const size_t  xMoveLen = pxNetworkBuffer->xDataLength - optlen - ipSIZE_OF_IPv4_HEADER - ipSIZE_OF_ETH_HEADER;

		//	memmove( pucTarget, pucSource, xMoveLen );
			pxNetworkBuffer->xDataLength -= optlen;


            /* Fix */
         //   pxIPHeader->ucVersionHeaderLength =
           //     ( pxIPHeader->ucVersionHeaderLength & 0xF0 ) | /* High nibble is the version. */
           //     ( ( ipSIZE_OF_IPv4_HEADER >> 2 ) & 0x0F ); /* Low nibble is the header size, in bytes, divided by four. */
		}
		
		
	}	
	//test
	printf("ucVersionHeaderLength is 0x%08x\r\n",pxIPHeader->ucVersionHeaderLength );
	printf("ucVersionHeaderLength is %d\r\n",pxIPHeader->ucVersionHeaderLength );
    	if(pxIPHeader->ucVersionHeaderLength == 5){
			      printf("cve2018_16526 patch success");
			}
		//do something
	return eReturn;
}

void test_cve2018_16526(void){
	IPPacket_t pkt = {
		.xIPHeader = {
			.ucVersionHeaderLength = 6,//0110
			.ucProtocol = 0x1,
		}
	};
	NetworkBufferDescriptor_t data = {
		.xDataLength = 40,
	};
     int i = cve16526(&pkt,&data);
}

/*cve2018-16523 16524*/
static void cve2018_16523( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
TCPPacket_t * pxTCPPacket;
TCPHeader_t * pxTCPHeader;
const unsigned char *pucPtr;
const unsigned char *pucLast;
//TCPWindow_t *pxTCPWindow;
UBaseType_t uxNewMSS;

	pxTCPPacket = ( TCPPacket_t * ) ( pxNetworkBuffer->pucEthernetBuffer );
	pxTCPHeader = &pxTCPPacket->xTCPHeader;

	pucPtr = pxTCPHeader->ucOptdata;
	pucLast = pucPtr + (((pxTCPHeader->ucTCPOffset >> 4) - 5) << 2);
	//pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;
  
 
   // if( pucLast > ( pxNetworkBuffer->pucEthernetBuffer + pxNetworkBuffer->xDataLength ) )
  //  {   
   //     return;
   // }//fix
   

	while( pucPtr < pucLast )
	{
        UBaseType_t xRemainingOptionsBytes = pucLast - pucPtr;

		if( pucPtr[ 0 ] == TCP_OPT_END )
		{
			break;
		}
		if( pucPtr[ 0 ] == TCP_OPT_NOOP)
		{
        
            pucPtr++;
            continue;
		}
   // printf("haha\r\n");
// if( xRemainingOptionsBytes < 2 )
 //{ 
  //   break;
  //}
	}
	printf("cve2018-16523 165224 still vulnable\r\n");
}

void test_cve2018_16523(void){
	//printf("start test cve2018-16523 func\r\n");
	TCPHeader_t data;
	data.ucOptdata[0] = 1;
	data.ucTCPOffset =1;
	TCPPacket_t prv;
	prv.xTCPHeader = data;
	
	NetworkBufferDescriptor_t testdata;
	testdata.xDataLength = 0;
	testdata.pucEthernetBuffer = &prv;
     printf("test cve2018-16523func\r\n");
     cve2018_16523(&testdata);
}





static void cve2018_16524( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
TCPPacket_t * pxTCPPacket;
TCPHeader_t * pxTCPHeader;
const unsigned char *pucPtr;
const unsigned char *pucLast;
//TCPWindow_t *pxTCPWindow;
UBaseType_t uxNewMSS;

	pxTCPPacket = ( TCPPacket_t * ) ( pxNetworkBuffer->pucEthernetBuffer );
	pxTCPHeader = &pxTCPPacket->xTCPHeader;

	pucPtr = pxTCPHeader->ucOptdata;
	pucLast = pucPtr + (((pxTCPHeader->ucTCPOffset >> 4) - 5) << 2);
	//pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;
  
	while( pucPtr < pucLast )
	{
        UBaseType_t xRemainingOptionsBytes = pucLast - pucPtr;
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

void test_cve2018_16524(void){
	//printf("start test cve2018-16524 func\r\n");
	TCPHeader_t data;
	int i;
	for( i = 0; i<7; i++){
	  data.ucOptdata[i] = 1;
	}
	data.ucOptdata[7] = 0x2;
	data.ucTCPOffset =0x70;
	TCPPacket_t prv;
	prv.xTCPHeader = data;
	
	NetworkBufferDescriptor_t testdata;
	testdata.xDataLength = 0;
	testdata.pucEthernetBuffer = &prv;
	   
     printf("test cve2018-16524func\r\n");
	   printf("if not print still vul,it patched successfully \r\n");
     cve2018_16524(&testdata);
}




/*cve2018-16599*/
static void cve2018_16599( uint8_t *pucUDPPayloadBuffer, size_t xBufferLength, uint32_t ulIPAddress )
{
		uint16_t usFlags, usType, usClass;
		uint8_t *pucSource, *pucTarget;
		uint8_t ucByte;
		uint8_t ucNBNSName[ 17 ];

        /* Check for minimum buffer size. */
    //    if( xBufferLength < sizeof( NBNSRequest_t ) )
      //  {
        //    return;
        //}
	 
	
	//test
		printf("cve2018-16599 still vulnable\r\n");
}
void test_cve2018_16599(void){
	printf("start test_cve2018_16599\r\n");
	uint8_t a=0x1;
	size_t  b = 0;
	uint32_t c = 1;
	cve2018_16599(&a,b,c);
}



//cve2018-16600
static void prvProcessEthernetPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
EthernetHeader_t *pxEthernetHeader;
volatile eFrameProcessingResult_t eReturned; /* Volatile to prevent complier warnings when ipCONSIDER_FRAME_FOR_PROCESSING just sets it to eProcessBuffer. */

//	configASSERT( pxNetworkBuffer );
  eReturned = eProcessBuffer;
	//eReturned = ipCONSIDER_FRAME_FOR_PROCESSING( pxNetworkBuffer->pucEthernetBuffer );
	pxEthernetHeader = ( EthernetHeader_t * ) ( pxNetworkBuffer->pucEthernetBuffer );

	if( eReturned == eProcessBuffer )
	{
		
		switch( pxEthernetHeader->usFrameType )
		{
			case ipARP_FRAME_TYPE :
			
			//if( pxNetworkBuffer->xDataLength < sizeof( ARPPacket_t ) )
      //        {
         //           eReturned = eReleaseBuffer;
					//				  break
          //      }
				eReturned = eReturnEthernetFrame; //other
				break;	
		}
	}
	//dosomething 
	if(eReturned == eReleaseBuffer){
	   printf("cve2018-16600 patch success!\r\n");
	}
}	
void test_cve2018_16600(void){
	EthernetHeader_t data = {
		.usFrameType = ipARP_FRAME_TYPE,
		
	};
	//uint8_t * data1 = &data;
	NetworkBufferDescriptor_t testdata = {
		.pucEthernetBuffer = &data,
		.xDataLength =0,
		
	};
	prvProcessEthernetPacket(&testdata);
}

/*cve2021-31512*/
StreamBufferHandle_t cve2021_31512( size_t xBufferSizeBytes, size_t xTriggerLevelBytes, BaseType_t xIsMessageBuffer )
{
	uint8_t *pucAllocatedMemory;

		
		//configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );
		//configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

		if( xTriggerLevelBytes == ( size_t ) 0 )
		{
			xTriggerLevelBytes = ( size_t ) 1; /*lint !e9044 Parameter modified to ensure it doesn't have a dangerous value. */
		}

		
		
  //  if(xBufferSizeBytes >= (xBufferSizeBytes + 1 + sizeof(StreamBuffer_t))) 
	//	{
		//    pucAllocatedMemory = NULL;
	//	}else{
		    xBufferSizeBytes++;
		    pucAllocatedMemory = (uint8_t *)0x20009000; 
		//}
		/*do some thing*/
    
		return ( StreamBufferHandle_t * ) pucAllocatedMemory; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
}
void test_cve2021_31512(void){
	StreamBufferHandle_t i = cve2021_31512(0xFFFFFFFF,0,1);
	if(i == NULL){
	      printf("cve2021_31512 patch success!\r\n");
	}
}


/*cve2021 31571*/

Queue_t * cve2021_31571( const UBaseType_t uxQueueLength,
                                       const UBaseType_t uxItemSize,
                                       const uint8_t ucQueueType )
{
        Queue_t * pxNewQueue;
        size_t xQueueSizeInBytes;
        uint8_t * pucQueueStorage;

        configASSERT( uxQueueLength > ( UBaseType_t ) 0 );

        
        xQueueSizeInBytes = ( size_t ) ( uxQueueLength * uxItemSize ); 
        

        configASSERT( ( uxItemSize == 0 ) || ( uxQueueLength == ( xQueueSizeInBytes / uxItemSize ) ) );
        
  
       // configASSERT( ( sizeof( Queue_t ) + xQueueSizeInBytes ) >  xQueueSizeInBytes );
        //fix
        
        pxNewQueue = NULL; 

        return pxNewQueue;
}
void test_cve2021_31571(void){
	Queue_t * p= cve2021_31571( 1,1,1 );
}
struct xDHCPMessage
{
	uint8_t ucOpcode;	
	uint32_t ulTransactionID;
	uint32_t ulDHCPCookie;
	
};
typedef struct xDHCPMessage DHCPMessage_t;
#define dhcpCOOKIE		0x63538263ul
#define dhcpREPLY_OPCODE						( 2 )
static BaseType_t prvProcessDHCPReplies( BaseType_t xExpectedMessageType )
{
uint8_t *pucUDPPayload, *pucLastByte;
//struct freertos_sockaddr xClient;
//uint32_t xClientLength = sizeof( xClient );
int32_t lBytes;
DHCPMessage_t *pxDHCPMessage;
uint8_t *pucByte, ucOptionCode, ucLength;
uint32_t ulProcessed, ulParameter;
BaseType_t xReturn = pdFALSE;
const uint32_t ulMandatoryOptions = 2ul; /* DHCP server address, and the correct DHCP message type must be present in the options. */

	lBytes = 0x1;

	if( lBytes > 0 )
	{
		/* Map a DHCP structure onto the received data. */
		pxDHCPMessage->ulDHCPCookie = ( uint32_t ) dhcpCOOKIE ;
    pxDHCPMessage->ucOpcode = ( uint8_t ) dhcpREPLY_OPCODE;
		/* Sanity check. */
		if( ( lBytes >= sizeof( DHCPMessage_t ) ) &&
            ( pxDHCPMessage->ulDHCPCookie == ( uint32_t ) dhcpCOOKIE ) &&
			( pxDHCPMessage->ucOpcode == ( uint8_t ) dhcpREPLY_OPCODE ) )
		{   
			BaseType_t xReturn = pdPASS;
		}
		//dosomething
	return xReturn;
  }	
}
void test_cve2021_16602(void){
	BaseType_t i = prvProcessDHCPReplies(1);
	if(i == pdFALSE){
	   printf("patch success\r\n");
	}else{
	   printf("patch failed\r\n");
	}
}		
