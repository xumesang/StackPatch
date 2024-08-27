#include "freertos_demo.h"
#include "usart.h"
#include "led.h"
#include "freertoscve.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "patch_delay_test.h"
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   64
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         3
#define TASK1_STACK_SIZE   64
TaskHandle_t    task1_handler;
void task1( void * pvParameters );

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         2
#define TASK2_STACK_SIZE   64
TaskHandle_t    task2_handler;
void patchtask( void * pvParameters );
/******************************************************************************************************/
#define  DEM_CR_TRCENA                        (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

#define  DWT_CYCCNT                              *(volatile uint32_t *)0xE0001004
#define  DWT_CR                                        *(volatile uint32_t *)0xE0001000
#define  DEM_CR                                        *(volatile uint32_t *)0xE000EDFC
#define  DBGMCU_CR                               *(volatile uint32_t *)0xE0042004

uint32_t DWT_TestTable[10];

void dwt_tim_init(void){
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;   
    DWT_CYCCNT = (uint32_t)0u;
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}


void dwt_trace_test(void){    

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk |CoreDebug_DEMCR_MON_EN_Msk;
    /*
    DWT->FUNCTION0 = 0;
    DWT->COMP0 = (uint32_t)&DWT_TestTable[8];
    DWT->MASK0 = 2;
    DWT->FUNCTION0 = 7;
    
    DWT->FUNCTION1 = 0;
    DWT->COMP1 = (uint32_t)&DWT_TestTable[3];
    DWT->MASK1 = 2;
    DWT->FUNCTION1 = 7;
    
    DWT->FUNCTION2 = 0;
    DWT->COMP2 = (uint32_t)&DWT_TestTable[6];
    DWT->MASK2 = 2;
    DWT->FUNCTION2 = 7;*/
//https://blog.csdn.net/K_O_Carnivist/article/details/80547121    csdn
    DWT->FUNCTION3 = 0;
    DWT->COMP3 = 0x080022d8;
   // DWT->MASK3 = 2;
    DWT->FUNCTION3 =4;
}


void freertos_demo(void)
{    
    xTaskCreate((TaskFunction_t         )   start_task,
                (char *                 )   "start_task",
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   START_TASK_PRIO,
                (TaskHandle_t *         )   &start_task_handler );
    vTaskStartScheduler();
}


void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* 进入临界区 */
    xTaskCreate((TaskFunction_t         )   task1,
                (char *                 )   "task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &task1_handler );
                
    xTaskCreate((TaskFunction_t         )   patchtask,
                (char *                 )   "patchtask",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIO,
                (TaskHandle_t *         )   &task2_handler );
          
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

volatile uint32_t  patch_address = 0x08000000;
volatile uint32_t  patch_dispatch_address = 0x08000000;
/* 任务一，实现LED每500ms翻转一次 */
void task1( void * pvParameters )
{   uint32_t  i = 1;
	  const TickType_t xDelay = pdMS_TO_TICKS(50);

	
   while(1){
		  //	ptr=(void(*)())0x20008001;
			//  ptr();
        
				//test_cve2018_16600();
       // test_cve2018_16603();
			   //test_cve2018_16528();
			 //  test_cve2018_16526();
		     //test_cve2021_31512();
			//test_cve2018_16599();
			//test_cve2021_3159071();
			//test_cve2018_16524();
			//MeasureTimeStart();
			//int a = MeasureTimeEnd();
		
	  //  test_cve2018_16603();
		
		
	//	printf("%u\r\n",startTime);
		printf("no.%u\r\n",i);
		test_cve2018_16601();
	    i++;
	//		 __ASM("bkpt 0x1");
		   // __asm("nop");
			 vTaskDelay(xDelay);
			//vTaskDelayUntil(&xLastWakeTime,xDelay);
	 }

}

/* 任务二，tansmit hello */
volatile uint32_t break_point_instruction_addr;
volatile uint32_t back_instruction_address;

//extern void patchfunc(void) __attribute__((section(".ARM.__at_0x20008000")));
//volatile uint8_t link;
//uint8_t patch[256]  __attribute__ ((at(0x20017000)));
void mymemcpy(void *des,void *src,uint32_t n)  
{  
    uint8_t *xdes=des;
	  uint8_t *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}
static uint8_t NoPatch[] = ""
"\xc1\x6b\x89\x1c\xc1\x63\x89\x1c\xc1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x93\x13\x00\x20"
"";



static uint8_t patch_dispatch[] = ""
"\xC1\x6B\xDF\xF8\x18\x20\x91\x42\x03\xD0\xDF\xF8\x14\x20\x91\x42"//16
"\x01\xD0\xDF\xF8\x10\xF0\xDF\xF8\x0C\xF0\x00\x00\x2C\x22\x00\x08"
"\xDE\x22\x00\x08\x01\x90\x00\x20\x00\x00\x00\x00\x00\x00\x00\x00" //jumpback
"";

static uint8_t dwt_patch2018_16601[] = ""
"\x02\x6B\x41\x68\x89\x68\x0E\x39\x8A\x42\x02\xD8"//12
"\x01\x6B\x14\x29\x02\xD2\x4F\xF0\xFF\x31\x41\x62\x00\x00"
"\xdf\xf8\x04\xf0\x00\x00\xd5\x03\x00\x08" //jumpback
"";
static uint8_t patch2018_16601[] = ""
"\x02\x6B\x41\x68\x09\x68\x0E\x39\x8A\x42\x02\xD8\x01\x6B\x14\x29"//16
"\x06\xD2\x4F\xF0\xFF\x31\x41\x62\xC1\x6B\x89\x1C\xC1\x63\x04\xE0"
"\x00\x21\x41\x62\xC1\x6B\x89\x1C\xC1\x63\x00\x00"//patch   //10
"\xdf\xf8\x04\xf0\x00\x00\x8f\x13\x00\x20" //jumpback
"";
/*
static uint8_t patch2018_16603[] = ""
"\x02\x6b\x81\x68\x89\x68\x0e\x39\x8a\x42\x02\xd8\x01\x6b\x14\x29\x15\xd2\x00\x22\x42\x62\x42\x6c\x42\x60"
"\x82\x6c\xc2\x63\x10\x21\x05\xe0\x50\xf8\x21\x20\x00\xeb\x81\x03\x9a\x60\x49\x1e\x00\x29\xf7\xda\x00\xf1"
"\x08\x02\x82\xf3\x09\x88\x00\xbf\x04\xe0\x01\x21\x41\x62\xc1\x6b\x89\x1c\xc1\x63\x00\x00"//patch
"\xdf\xf8\x04\xf0\x00\x00\x7b\x13\x00\x20\x00\x00\x00\x00\x00\x00" //jumpback
"";*/
static uint8_t patch2018_16603[] = ""
"\x02\x6B\x81\x68\x09\x68\x0E\x39\x8A\x42\x02\xD8\x01\x6B\x14\x29"//16
"\x05\xD2\x00\x21\x41\x62\xC1\x6B\x09\x1D\xC1\x63\x06\xE0\xC1\x6A"
"\xD1\xF8\x19\x10\x41\x60\xC1\x6B\x89\x1C\xC1\x63\x00\x00"//patch
"\xdf\xf8\x04\xf0\x00\x00\x93\x13\x00\x20\x00" //jumpback
"";
static uint8_t patch16528[] = ""
"\x81\x68\x11\xb9\x01\x21\x42\x68\x11\x62\x41\x68\xc1\x60\xc1\x6b\x89\x1c\xc1\x63\x00\x00"//patch
"\xdf\xf8\x04\xf0\x00\x00\x93\x13\x00\x20\x00" //jumpback
"";
static uint8_t patch16526[] = ""
"\x81\x68\x09\x78\x01\xf0\xf0\x01\x49\x1d\x82\x68\x11\x70\xc1\x6b\x89\x1c\xc1\x63\x00\x00"//patch
"\xdf\xf8\x04\xf0\x00\x00\x7f\x13\x00\x20\x00\x00\x00\x00\x00\x00" //jumpback
"";
static uint8_t patch16599[] = ""
"\x81\x68\x32\x29\x17\xD2\x42\x6C\x42\x60\x82\x6C\x82\x60\xC2\x6C"//patch
"\xC2\x60\x02\x6D\xC2\x63\x10\x21\x05\xE0\x50\xF8\x21\x20\x00\xEB"
"\x81\x03\x1A\x61\x49\x1E\x00\x29\xF7\xDA\x00\xF1\x10\x02\x82\xF3"
"\x09\x88\x00\xBF\x04\xE0\xC1\x6A\xC1\x60\xC1\x6B\x89\x1C\xC1\x63\x00\x00"
"\xdf\xf8\x04\xf0\x00\x00\x7b\x13\x00\x20\x00\x00\x00\x00\x00\x00" //jumpback
"";

static uint8_t patch31532[] = ""
"\x02\x6B\x02\xF1\x19\x01\x8A\x42\x05\xD3\x00\x21\x41\x62\xC1\x6B"
"\x09\x1D\xC1\x63\x05\xE0\x01\x6B\x49\x1C\x01\x63\xC1\x6B\x89\x1C"
"\xC1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x7b\x13\x00\x20\x00\x00\x00\x00\x00\x00" //jumpback
"";

static uint8_t patchcve2021_31571[] = ""
"\xc1\x6a\x20\x31\xc2\x6a\x91\x42\x01\xD8\x00\xbf\xfe\xe7\x00\x21"
"\x41\x62\xc1\x6b\x89\x1c\xc1\x63"
"\xdf\xf8\x04\xf0\x00\x00\x7b\x13\x00\x20\x00\x00\x00\x00\x00\x00" //jumpback
"";

static uint8_t patchcve2018_16600[] = ""
"\x41\x68\x09\x7A\x20\x29\x06\xDA\x00\x21\x41\x62\x41\x64\xC1\x6B"
"\x89\x1D\xC1\x63\x04\xE0\x02\x21\x41\x62\xC1\x6B\x89\x1C\xC1\x63\x00\x00"
"\xdf\xf8\x04\xf0\x00\x00\x73\x13\x00\x20\x00\x00\x00\x00\x00\x00" //jumpback
"";
static uint8_t patchcve2018_16523[] = ""
"\xC2\x68\x41\x68\xD2\x68\x05\x23\xC3\xEB\x12\x12\x01\xEB\x82\x01"
"\x42\x6A\x52\x68\x43\x6A\x9B\x68\x1A\x44\x91\x42\x1B\xD9\x42\x6C"
"\x42\x60\x82\x6C\x82\x60\xC2\x6C\xC2\x60\x02\x6D\x02\x61\x42\x6D"
"\x42\x61\x82\x6D\xC2\x63\x10\x21\x05\xE0\x50\xF8\x21\x20\x00\xEB"
"\x81\x03\x9A\x61\x49\x1E\x00\x29\xF7\xDA\x00\xF1\x18\x02\x82\xF3"
"\x09\x88\x00\xBF\x04\xE0\x05\x21\x81\x62\xC1\x6B\x89\x1C\xC1\x63\x00\x00"
"\xdf\xf8\x04\xf0\x00\x00\x7b\x13\x00\x20\x00\x00\x00\x00\x00\x00" //jumpback
"";
static uint8_t patchcve2018_16524[] = ""
"\x81\x69\x02\x29\x03\xD2\xC1\x6B\x12\x31\xC1\x63\x02\xE0\xC1\x6B"
"\x0C\x31\xC1\x63\x00\x00"
"\xdf\xf8\x04\xf0\x00\x00\x93\x13\x00\x20\x00\x00\x00\x00\x00\x00" //jumpback
"";

void patchtask( void * pvParameters )
{	 break_point_instruction_addr = 0x200040a4;
	   patch_address = 0x20009000;
	patch_dispatch_address = 0x20007000;
	   uint32_t i;
	   for ( i = 0; i < sizeof(dwt_patch2018_16601); i=i+1){           
					  *(uint8_t *)(patch_address+i) = *(uint8_t *)(dwt_patch2018_16601+i);
        }
     for ( i = 0; i < sizeof(patch_dispatch); i=i+1){           
					  *(uint8_t *)(patch_dispatch_address+i) = *(uint8_t *)(patch_dispatch+i);
        }
	   dwt_trace_test();
		 vTaskDelete(NULL); 
}



