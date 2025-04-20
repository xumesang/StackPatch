#include "cvefunc.h"
struct exception_info
{
  uint32_t exc_return;   //*(pStack)
	uint32_t r4;    //*(pStack + 1u)
  uint32_t r5;    //*(pStack + 2u)
	uint32_t r6;    //*(pStack + 3u)
	uint32_t r7;    //*(pStack + 4u)
	uint32_t r8;    //*(pStack + 5u)
	uint32_t r9;    //*(pStack + 6u)
	uint32_t r10;   //*(pStack + 7u)
	uint32_t r11;   //*(pStack + 8u)
	uint32_t r0;    //*(pStack + 9u)
	uint32_t r1;    //*(pStack + 10u)
	uint32_t r2;    //*(pStack + 11u)
	uint32_t r3;    //*(pStack + 12u)
	uint32_t r12;   //*(pStack + 13u)
	uint32_t lr;    //*(pStack + 14u)
	uint32_t pc;    //*(pStack + 15u)
	uint32_t psr;   //*(pStack + 16u)
};

// Repair the CVE-2017-14199 vulnerability
void patch_CVE_2017_14199_1(unsigned int* pStack){
     if(*(pStack + 9u) >= ARRAY_SIZE(ai_arr)) {
   		NET_DBG("getaddrinfo entries overflow");
         *(pStack + 15u) -= 4u;
	}else{  
		     *(pStack + 11u) = *(uint16_t *)(*(pStack + 3u)+8);
	       *(pStack + 15u) += 2u;
	}
}

// Repair the CVE-2017-14199 vulnerability
void patch_CVE_2017_14199_2(unsigned int* pStack){
	   *(pStack + 10u) = *(uint16_t *)(*(pStack + 2u) + 0x1A);
	   *(uint16_t *)(*(pStack + 9u) + 2) = *(pStack + 10u);
	   *(pStack + 9u) = *(pStack + 1u) + 0x20;
     *(pStack + 15u) += 2u;
}

// Repair the CVE-2020-10019 vulnerability
void patch_CVE_2020_10019(unsigned int* pStack){
		if (*(pStack + 10u) > USB_DFU_MAX_XFER_SIZE) {
			*(pStack + 10u) = USB_DFU_MAX_XFER_SIZE;
	     }     
			*(pStack + 9u) = 0;
	    *(pStack + 15u) += 2u;
}

// Repair the CVE-2020-10021 vulnerability
void patch_CVE_2020_10021(unsigned int* pStack){
		if ((*(pStack + 1u) * BLOCK_SIZE) >= memory_size){
		    *(pStack + 15u) += 12u;
		}else{
			   *(pStack + 9u) = *(pStack + 1u) <<9;
		    *(pStack + 15u) += 2u;
		}
}


// Repair the CVE-2020-10023 vulnerability
void patch_CVE_2020_10023(unsigned int* pStack){
		*(pStack + 12u) = *(pStack + 2u) - *(pStack + 5u);
	  *(pStack + 15u) += 2u;
}

// Repair the CVE-2020-10059 vulnerability
void patch_CVE_2020_10059(unsigned int* pStack){
		*(pStack + 12u) = 1;
	  *(pStack + 15u) += 2u;
}

// Repair the CVE-2020-10066 vulnerability
void patch_CVE_2020_10066(unsigned int* pStack){
		*(pStack + 11u)=*(pStack + 2u);
	  *(pStack + 15u) += 2u;
}


// Repair the CVE-2020-10069 vulnerability
void patch_CVE_2020_10069(unsigned int* pStack){
		if(*(pStack + 10u)<2||*(pStack + 9u)<5||*(pStack + 9u)>16){
		   *(pStack + 15u) += 12u;
		}else{
			uint8_t *ptr = (uint8_t *)*(pStack + 1u);
      ptr[1] = (uint8_t)*(pStack + 9u);
			 *(pStack + 15u) += 2u;
//STRB            R0, [R4,#1]
		}
}

// Repair the CVE-2020-10062 vulnerability
void patch_CVE_2020_10062(unsigned int* pStack){
		if( (uint32_t)(*(pStack + 2u)) >0x0FFFFFFF){
			 *(pStack + 9u) = 0xFFFFFFEA;
		   *(pStack + 15u) += 12u;
		}else{
			 *(pStack + 10u) = (uint32_t)(*(pStack + 2u));
			 *(pStack + 15u) += 2u;
         //LDR             R1, [length]
		}
}

//	if (u16_add_overflow(*opt_len, hdr_len, opt_len)) {
	//		return -EINVAL;
	//	}

// Repair the CVE-2020-10063 vulnerability
void patch_CVE_2020_10063(unsigned int* pStack){
		if(*(pStack + 9u) + *(pStack + 10u) < *(pStack + 9u)){
		   *(pStack + 15u) += 6u;
		}else{
			 uint16_t *ptr = (uint16_t *)*(pStack + 1u);
       *ptr = (uint16_t)*(pStack + 9u);//STRH   R0, [R4]
			 *(pStack + 15u) += 2u;

		}
}


// Repair the CVE-2020-10070 vulnerability
void patch_CVE_2020_10070_1(unsigned int* pStack){
		if( *(pStack + 3u) <= *(pStack + 9u) ){
		   *(pStack + 15u) += 6u;
		}else{
			 *(pStack + 4u) = *(pStack + 3u) - *(pStack + 9u);//SUBS     R7, R6, R0
			 *(pStack + 15u) += 2u;

		}
}

// Repair the CVE-2020-10070 vulnerability
void patch_CVE_2020_10070_2(unsigned int* pStack){
		if( *(pStack + 4u)+*(pStack + 10u) < *(pStack + 9u)){
		   *(pStack + 15u) += 6u;
		}else{
			 *(pStack + 10u)=*(pStack + 10u)+*(pStack + 4u);//ADD      R1, R7
			 *(pStack + 15u) += 2u;

		}
}

// Repair the CVE-2023-6749 vulnerability
void patch_CVE_2023_6749(unsigned int* pStack){
		if( *(pStack + 4u)>5){
		   *(pStack + 15u) += 30u;
		}else{
			*(pStack + 9u) = *(pStack + 2u)-1;//SUBS   R0, argc, #1
			 *(pStack + 15u) += 2u;
		}
}

// Repair the CVE-2022-1841 vulnerability
void patch_CVE_2022_1841(unsigned int* pStack){
		if( *(pStack + 1u) > 0){
		   *(pStack + 10u) = 0;//MOVS     R1, #0
			 *(pStack + 15u) += 2u;
		}else{
			 *(pStack + 15u) += 8u;
		}
}

// Repair the CVE-2021-3518 vulnerability
void patch_CVE_2021_3518(unsigned int* pStack){
		if( *(pStack + 10u) > PDU_AC_DATA_SIZE_MAX){
		    *(pStack + 9u) = BT_HCI_ERR_INVALID_PARAM;
			  *(pStack + 15u) += 2u;
		}else{
			  *(pStack + 9u) = 0;
			  *(pStack + 15u) += 2u;
		}
}

// Repair the CVE-2023-5184 vulnerability
void patch_CVE_2023_5184(unsigned int* pStack){
		if( *(pStack + 1u)<0){ 
			  *(pStack + 15u) -= 6u;
		}else{
			  *(pStack + 9u) = *(pStack + 11u)&0xC0000000;
			  *(pStack + 15u) += 4u;
		}
}

// Repair the CVE-2023-4263 vulnerability
//CVE-2023-4263
	//if (payload_len > NRF5_PSDU_LENGTH) {
	//	return -EMSGSIZE;
	//}
void patch_CVE_2023_4263(unsigned int* pStack){
		if( *(pStack + 4u) > NRF5_PSDU_LENGTH){ 
			  *(pStack + 9u) = -EMSGSIZE;
			  *(pStack + 15u) += 30u;
		}else{
		  	*(pStack + 6u) = 1;         //MOV.W    ret, #1
			  *(pStack + 15u) += 2u;
		}
}

// Repair the CVE-2020-17445 vulnerability
//CVE_2020_17445
//if((optlen > len ) || (optlen == 0))
		//	return -1;
void patch_CVE_2020_17445(unsigned int* pStack){
		if( ( *(pStack + 10u) > (uint8_t)*(pStack + 9u) ) || ( *(pStack + 10u) == 0 ) ){ 
			  *(pStack + 9u) = -1;
			  *(pStack + 15u) += 30u;
		}else{
		  	*(pStack + 2u) = (uint8_t)*(pStack + 9u);         //R5=(uint8_t)R0
			  *(pStack + 15u) += 2u;
		}
}

// Repair the CVE-2020-24337 vulnerability
//CVE-2020-24337
// if (f->transport_hdr + f->transport_len > f->buffer + f->buffer_len)
//     return -1;
void patch_CVE_2020_24337(unsigned int* pStack){
		if( *(uint32_t *)(*(pStack + 10u)+0x28)  + *(uint32_t *)(*(pStack + 10u)+0x24) <= 
			*(uint32_t *)(*(pStack + 10u)+4) +*(uint32_t *)(*(pStack + 10u)+0x8) ){ 
			  *(pStack + 9u) = -1;
			  *(pStack + 15u) += 52u;
		}else{
		  	*(pStack + 9u) =  *(uint32_t *)(*(pStack + 10u) + 0x3C);         //    [f,#0x3C]
			  *(pStack + 15u) += 2u;
		}
}

// Repair the CVE-2020-0901 vulnerability
void patch_CVE_2020_0901(unsigned int* pStack){
		if( *(pStack + 10u) == 0 ){ 
			  *(pStack + 15u) += 4u;
		}else{
		  	*(pStack + 9u) = *(uint32_t *)( *(pStack + 11u) + 4 );        //    LDR             R0, [ssl,#4]
			  *(pStack + 15u) += 2u;
		}  
}
