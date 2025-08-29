//See LICENSE for license details.
#include "systick.h"
#include <unistd.h>
#include "riscv_encoding.h"
#include "n200_func.h"

__attribute__((weak)) uintptr_t handle_nmi()
{
 // write(1, "nmi\n", 5);
  printf("hhh");
  _exit(1);
  return 0;
}



//__attribute__((section(".test_address_2"))) void patch(uintptr_t *sp){
//	*(sp + 0) += 2u;
//
//}
struct patchtable
{
	unsigned int vuladdress;
	unsigned int patchaddress;
};
struct patchtable patchs[64] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0}};

unsigned int SearchPos(unsigned int bkptaddress,int n) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (patchs[mid].vuladdress > bkptaddress) {
            right = mid - 1;
        }

    }
    return 0x0;
}
__attribute__((section(".test_address_1"))) void handle_trap(uintptr_t epc,uintptr_t *sp)
//__attribute__((weak)) uintptr_t handle_trap(uintptr_t mcause,uintptr_t epc,uintptr_t *sp)
{
	/*1.
  if(*(uint32_t *)(*(sp + 8) - 24 ) > *(uint32_t *)(*(uint32_t *)(*(sp + 8) - 40 ) + 8) - 14u ||*(uint32_t *)(*(sp + 8) - 24 ) < 20u){

	  *(sp + 15) = -1;
	 //  *(sp + 16) = *(sp + 0);
	   *(sp + 0) += 4u;
  }else{
      //back
	  *(sp + 0) += 2u;
  }
//  _exit(mcause);
  return 0;*/
	uint32_t patchaddr = SearchPos(0x80005000,64);
	if( patchaddr == 0x0){
		*(sp + 0) += 2u;
	}

	 /*

switch(epc){
		   case 0x80005c8:
		    	//printf("1");
		       patch(sp);
			   break;
		   case 0x8000002:
		    	printf("2");
		        break;
		    case 0x8000003:
		    	printf("3");
		    	break;
		    case 0x8000004:
			    printf("4");
			    break;
			case 0x8000005:
				printf("5");
		        break;
			case 0x8000006:
				printf("6");
				break;
		    case 0x8000007:
		    	printf("7");
		        break;
		    case 0x8000008:
		    	printf("8");
		        break;
		    case 0x8000009:
		    	printf("9");
		    	break;
		    case 0x8000010:
			    printf("10");
			    break;
			case 0x8000011:
				printf("11");
		        break;
			case 0x8000012:
				printf("12");
				break;
			case 0x8000013:
				printf("13");
				break;
			case 0x8000014:
				printf("14");
				break;
			case 0x8000015:
				printf("15");
				break;
			case 0x8000016:
				printf("16");
				break;
			case 0x8000017:
				printf("17");
				break;
			case 0x8000018:
				printf("18");
				break;
			case 0x8000019:
				printf("19");
				break;
			case 0x8000020:
				printf("20");
				break;
			case 0x8000021:
				printf("21");
				break;
			case 0x8000022:
				printf("22");
				break;
		    case 0x8000023:
		    	printf("23");
		        break;
		    case 0x8000024:
		    	printf("24");
		        break;
		    case 0x8000025:
		    	printf("25");
		    	break;
			case 0x8000026:
				printf("26");
				break;
			case 0x8000027:
				printf("27");
				break;
			case 0x8000028:
				printf("28");
				break;
			case 0x8000029:
				printf("29");
				break;
			case 0x8000030:
				printf("30");
				break;
			case 0x8000031:
				printf("31");
				break;
			case 0x8000032:
				printf("32");
				break;
		    case 0x8000033:
		    	printf("33");
		        break;
		    case 0x8000034:
		    	printf("34");
		        break;
		    case 0x8000035:
		    	printf("35");
		    	break;
			case 0x8000036:
				printf("36");
				break;
			case 0x8000037:
				printf("37");
				break;
			case 0x8000038:
				printf("38");
				break;
			case 0x8000039:
				printf("39");
				break;
			case 0x8000040:
				printf("40");
				break;
			case 0x8000041:
				printf("41");
				break;
			case 0x8000042:
				printf("42");
				break;
		    case 0x8000043:
		    	printf("43");
		        break;
		    case 0x8000044:
		    	printf("44");
		        break;
		    case 0x8000045:
		    	printf("45");
		    	break;
			case 0x8000046:
				printf("46");
				break;
			case 0x8000047:
				printf("47");
				break;
			case 0x8000048:
				printf("48");
				break;
			case 0x8000049:
				printf("49");
				break;
			case 0x8000050:
				printf("50");
				break;
		    case 0x8000051:
		    	printf("51");
		        break;
		    case 0x8000052:
		    	printf("52");
		        break;
		    case 0x8000053:
		    	printf("53");
		    	break;
		    case 0x8000054:
			    printf("54");
			    break;
			case 0x8000055:
				printf("55");
		        break;
			case 0x8000056:
				printf("56");
				break;
		    case 0x8000057:
		    	printf("57");
		        break;
		    case 0x8000058:
		    	printf("58");
		        break;
		    case 0x8000059:
		    	printf("59");
		    	break;
		    case 0x8000060:
			    printf("60");
			    break;
			case 0x8000061:
				printf("61");
		        break;
			case 0x8000062:
				printf("62");
				break;
			case 0x8000063:
				printf("63");
				break;
			case 0x8000064:
				printf("64");
				break;
			case 0x8000065:
				printf("65");
				break;
			case 0x8000066:
				printf("66");
				break;
			case 0x8000067:
				printf("67");
				break;
			case 0x8000068:
				printf("68");
				break;
			case 0x8000069:
				printf("69");
				break;
			case 0x8000070:
				printf("70");
				break;
			case 0x8000071:
				printf("71");
				break;
			case 0x8000072:
				printf("72");
				break;
		    case 0x8000073:
		    	printf("73");
		        break;
		    case 0x8000074:
		    	printf("74");
		        break;
		    case 0x8000075:
		    	printf("75");
		    	break;
			case 0x8000076:
				printf("76");
				break;
			case 0x8000077:
				printf("77");
				break;
			case 0x8000078:
				printf("78");
				break;
			case 0x8000079:
				printf("79");
				break;
			case 0x8000080:
				printf("80");
				break;
			case 0x8000081:
				printf("81");
				break;
			case 0x8000082:
				printf("82");
				break;
		    case 0x8000083:
		    	printf("83");
		        break;
		    case 0x8000084:
		    	printf("84");
		        break;
		    case 0x8000085:
		    	printf("85");
		    	break;
			case 0x8000086:
				printf("86");
				break;
			case 0x8000087:
				printf("87");
				break;
			case 0x8000088:
				printf("88");
				break;
			case 0x8000089:
				printf("89");
				break;
			case 0x8000090:
				printf("90");
				break;
			case 0x8000091:
				printf("91");
				break;
			case 0x8000092:
				printf("92");
				break;
		    case 0x8000093:
		    	printf("93");
		        break;
		    case 0x8000094:
		    	printf("94");
		        break;
		    case 0x8000095:
		    	printf("95");
		    	break;
			case 0x8000096:
				printf("96");
				break;
			case 0x8000097:
				printf("97");
				break;
			case 0x8000098:
				printf("98");
				break;
			case 0x8000099:
				printf("99");
				break;

		    default:
		    	printf("123");

		}*/
    /*
	switch(epc){
		    case 0x8000001:
		    	__asm("li a3,0x20005000");
				__asm("jalr  ra, a3, 0");
		        break;
		    case 0x8000002:
		    	__asm("li a3,0x20006000");
		    	__asm("jalr  ra, a3, 0");
		    	break;
		    case 0x8000003:
		    	__asm("li a3,0x20007000");
		    	__asm("jalr  ra, a3, 0");
		    	break;
		    case 0x8000004:
		    	__asm("li a3,0x20008000");
		    	__asm("jalr  ra, a3, 0");
		    	break;
		    default:
		    	while(1);

	}*/
}




