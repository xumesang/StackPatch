#include "usart.h"


#include "freertoscve.h"

// Repair the CVE-2018-16601 vulnerability
void patch_cve2018_16601(unsigned int* pStack){ 
	
	
		if(  *(pStack + 12u) > *(uint32_t *)( *(pStack + 1u))-14u  ||  *(pStack + 12u) < 20u  ){
	//	printf("%d\r\n",*(uint32_t *)( *(pStack + 11u)));
		*(pStack + 9u) = -1; //r0 = -1
		 *(pStack + 15u) += 2u; //redirect
	 }else{// simuate original command and back
	  *(pStack + 9u) = 0;    //r0 = 0
	  *(pStack + 15u) += 2u; //back
	 }
	 /*
	uint32_t uxHeaderLength = ( uint32_t ) ( ( *(uint32_t *)(*(pStack + 11u)+0x10) & 0x0Fu ) << 2 );
	// printf("%d\r\n",uxHeaderLength);
	// printf("%d\r\n",*(uint32_t *)(*(pStack + 11u)));
	//	 pxIPHeader->ucVersionHeaderLength   :  *(r2+#0x10)
	if(  uxHeaderLength > *(uint32_t *)( *(pStack + 11u))-14u  ||  uxHeaderLength < 20u  ){
	//	printf("%d\r\n",*(uint32_t *)( *(pStack + 11u)));
		*(pStack + 9u) = -1; //r0 = -1
    *(pStack + 1u) = *(pStack + 17u);
		*(pStack + 15u) = *(pStack + 18u);//return 
		int32_t i;
		for(i = 16; i >= 0; i--){
		  *(pStack + i +2u) = *(pStack + i) ;
		} //memcpy 
		  __set_PSP((uint32_t)(pStack+2u));
		//  pStack += 2u; 
		 //return -1   simuate  return -1
	 }else{// simuate original command and back
	  *(pStack + 1u) =  *(pStack + 10u);     //mov r4,r1
	  *(pStack + 15u) += 2u; //back
	 }*/
}	

// Repair the CVE-2018-16603 vulnerability
void patch_cve2018_16603(unsigned int* pStack){ 
	 //printf("%d\r\n",*(pStack + 9u));

 /*  if( uxHeaderLength > pxNetworkBuffer->xDataLength - ipSIZE_OF_ETH_HEADER ||
        uxHeaderLength < ipSIZE_OF_IPv4_HEADER )*/
		 if( *(pStack + 12u)>*(uint32_t *)( *(pStack + 2u)) -14u|| *(pStack + 12u)<20u){
		   *(pStack + 9u) = 0;
			 *(pStack + 15u)+=4u;
		 }
			 else{
				 *(pStack + 1u) = *(uint32_t *)(*(pStack + 11u)+0x19);
			 *(pStack + 15u)+=2u;
			 }
}	

// Repair the CVE-2018-16528 vulnerability
void patch_cve2018_16528(unsigned int* pStack){  //breakpoint movs r6,r4;
	// printf("%d\r\n",*(uint32_t *)(*(pStack + 1u) + 0x20));
	if( *(pStack + 2u) == 0 ){
	   *(uint32_t *)(*(pStack + 1u) + 0x20) =1;
		
	 }// simuate original command and back
	  *(pStack + 3u) =*(pStack + 1u);    
	 *(pStack + 15u) += 2u; //back
}

// Repair the CVE-2018-16526 vulnerability
void patch_cve2018_16526(unsigned int* pStack){  
  *(uint8_t *)(*(pStack + 2u)) =(*(uint8_t *)(*(pStack + 2u))&0xf0 ) |( ( 20u >> 2 ) & 0x0F );
	 *(pStack + 15u) += 2u; //back
}

// Repair the CVE-2018-16599 vulnerability
void patch_cve2018_16599(unsigned int* pStack){  
    if(*(pStack + 2u) < 0x32 ){
	   *(pStack + 1u)  =  *(pStack + 17u) ;    //r4
		 *(pStack + 2u)  =  *(pStack + 18u) ;    //r5
		 *(pStack + 3u)  =  *(pStack + 19u) ;    //r6
	   *(pStack + 15u) = *(pStack + 20u);//lr
		 int32_t i;
	  	for(i = 16; i >= 0; i--){
		  *(pStack + i +4u) = *(pStack + i) ;
		 } //memcpy 
			__set_PSP((uint32_t)(pStack+4u));
	 }else{
		*(pStack + 3u) =*(pStack + 11u);//mov r6,r2
	  *(pStack + 15u) += 2u; //back
	 }
}

// Repair the CVE-2018-31532 vulnerability
void patch_cve2018_31532(unsigned int* pStack){  
  if(*(pStack + 12u) >= *(pStack + 12u) + 0x19 ){
	   *(pStack + 9u)  = NULL; 
		 *(pStack + 15u) += 4u; //back
	}else{
	  *(pStack + 12u) += 1;//adds r3,r3,1
	  *(pStack + 15u) += 2u; //back
	}
}

// Repair the CVE-2021-31571 vulnerability
void patch_cve2021_31571(unsigned int* pStack){ 
  if((0x20 + *(pStack + 11u) ) <=  *(pStack + 11u)){
	   while(1); //back
	}else{
	  *(pStack + 9u) = NULL;//adds r3,r3,1
	  *(pStack + 15u) += 2u; //back
	}
}

// Repair the CVE-2018-16600 vulnerability
void patch_cve2018_16600(unsigned int* pStack){ 
  if(*(uint8_t *)(*(pStack + 1u) + 8) < 0x20){
	   *(pStack + 9u) = 0;
		*(pStack + 17u) = 0;
		*(pStack + 15u) += 6u; //break
	}else{
	  *(pStack + 9u) = 2;//movs r0,2
	  *(pStack + 15u) += 2u; //back
	}
}

// Repair the CVE-2018-16532 vulnerability
void patch_cve2018_16523(unsigned int* pStack){ 
  if(*(pStack + 1u) + (((*(uint32_t *)(*(pStack + 3u)+0xc) >> 4) - 5) << 2) > ( *(uint32_t *)(*(pStack + 9u)+ 4) + *(uint32_t *)(*(pStack + 9u)+8 ))){
		//	 printf("hhha");
     *(pStack + 1u)  =  *(pStack + 17u) ;    //r4
		 *(pStack + 2u)  =  *(pStack + 18u) ;    //r5
		 *(pStack + 3u)  =  *(pStack + 19u) ;    //r6
	   *(pStack + 4u)  =  *(pStack + 20u) ;    //r7
	   *(pStack + 5u)  =  *(pStack + 21u) ;    //r8
	   *(pStack + 15u) =  *(pStack + 22u);//lr 
	    int32_t i;
	   for(i = 16; i >= 0; i--){
		  *(pStack + i + 6u) = *(pStack + i) ;
		 } //memcpy 
			__set_PSP((uint32_t)(pStack + 6u));
	}else{
		//printf("gege");
	  *(pStack + 10u) = 5;//movs r1,5
	  *(pStack + 15u) += 2u; //back
	}
}

// Repair the CVE-2018-16524 vulnerability
void patch_cve2018_16524(unsigned int* pStack){ 
  	if(*(pStack + 6u) < 2){
		*(pStack + 15u) +=18u; 
	}else{

	 
		*(pStack + 15u) +=12u; 
		 
	 // *(uint16_t *)(*(pStack + 15u)) = 0xF1B9; //back
	}
	 
}
