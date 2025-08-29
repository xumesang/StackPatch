#include "freertos_demo.h"
#include "usart.h"
#include "led.h"
#include "freertoscve.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "patch_delay_test.h"


#include "max30102.h" 
#include "myiic.h"
#include "algorithm.h"

#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   64
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );


#define TASK1_PRIO         3
#define TASK1_STACK_SIZE   256
TaskHandle_t    task1_handler;
void task1( void * pvParameters );


#define TASK2_PRIO         1
#define TASK2_STACK_SIZE   64
TaskHandle_t    task2_handler;
void patchtask( void * pvParameters );


#define TASK3_PRIO         2
#define TASK3_STACK_SIZE   64
TaskHandle_t    task3_handler;
void ledblinktask( void * pvParameters );
/******************************************************************************************************/
#define  DEM_CR_TRCENA                        (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

#define  DWT_CYCCNT                              *(volatile uint32_t *)0xE0001004
#define  DWT_CR                                        *(volatile uint32_t *)0xE0001000
#define  DEM_CR                                        *(volatile uint32_t *)0xE000EDFC
#define  DBGMCU_CR                               *(volatile uint32_t *)0xE0042004


// static DEBUG_CONTROL_BLOCK *const DEG_MON = (DEBUG_CONTROL_BLOCK *) 0xE000EDF0;

#define DCB_DEMCR 0xE000EDFC // page 464
#define DCB_DEMCR_MON_EN (uint32_t) (1 << 16)  
#define SCB_HFSR 0xE000E02C 
#define SCB_DFSR 0xE000ED30 // page 390
#define SCB_SHP8 0xE000ED20 // page 263, debug monitor priority

#define MAX_BP 6 // six inst breakpoint in cortex-m4
#define FPB_CB_REG 5
typedef struct {
	volatile uint32_t CTRL;
	volatile uint32_t REMAP;
	volatile uint32_t COMP[MAX_BP];
} FPB_UNIT;

// address must be 
typedef struct {
	uint32_t inst[10];
	bool active;
} FPB_REMAP;

static FPB_UNIT *const FPB = (FPB_UNIT *) 0xE0002000;
static FPB_REMAP REMAP_ADDR;

uint32_t DWT_TestTable[10];
// enable hardware breakpoint
static void fpb_enable(void) {
	FPB->CTRL |= 0x3;
}
static void fpb_disable(void) {
	FPB->CTRL = (FPB->CTRL & ~0x3) | 0x2;
}
static bool halting_debug_enabled(void) {
	volatile uint32_t *dhcsr = (uint32_t *)0xE000EDF0;
	return (((*dhcsr) & 0x1) != 0);
}
// init arm debug monitor
static void debug_monitor_init() {
	volatile uint32_t *scb_hfsr = (uint32_t *) SCB_HFSR;
	volatile uint32_t *scb_dfsr = (uint32_t *) SCB_DFSR;
	*scb_hfsr = (1 << 31);
//	*SCB_DFSR = 1;
//	*SCB_DFSR = (1 << 1);
	*scb_dfsr = (1 << 2);
	
	if (halting_debug_enabled()) {
		printf("Halting Debug Enabled - Can't Enable Monitor Mode Debug!");
		return;
	}
	
	volatile uint32_t *demcr = (uint32_t *) DCB_DEMCR;
	*demcr |= DCB_DEMCR_MON_EN; 
	//*demcr &= ~(1 << mon_en_bit); // DEG_MO = 0 && C_DEBUG = 1 -> HardFault
	fpb_enable();
	//printf("debug_monitor_init setup dhcsr: %u\r\n", *demcr);
	
	// Priority for DebugMonitor Exception is bits[7:0].
	// We will use the lowest priority so other ISRs can
	// fire while in the DebugMonitor Interrupt
	// Configurable priority values are in the range 0-255. This means that the Reset (), HardFault (-1), and NMI exceptions (-2), 
	// with fixed negative priority values, always have higher priority than any other exception.
	volatile uint32_t *shp8 = (uint32_t *)0xE000ED20;
	//DEBUG_LOG("SCB_SHP[8] priority: %d\n", (int) ((char)*shp8));
	*shp8 = 0xff;
	*shp8 = -1;
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
    taskENTER_CRITICAL();               
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
		xTaskCreate((TaskFunction_t         )   ledblinktask,
                (char *                 )   "ledblinktask",
                (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK3_PRIO,
                (TaskHandle_t *         )   &task3_handler );
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                
}

volatile uint32_t  patch_address = 0x08000000;
volatile uint32_t  patch_dispatch_address = 0x08000000;


char senddate[16]={0x5A,0xA5,0,0,0,0,0,0,0,0,0,0,0x53,0x54,0x4F,0x50};
uint32_t aun_ir_buffer[500]; //IR LED sensor data   ????,??????
int32_t n_ir_buffer_length=500;    //data length  
uint32_t aun_red_buffer[500];    //Red LED sensor data  ????,??????????????
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
#define MAX_BRIGHTNESS 255
int m  = 10;
void vul_func(void)
{
  int buffer[10] = {0}; // 40 bytes 
	 
	
	//	buffer[m] = 130000+m; //i = 10??
	// int d = buffer[m];
	int i;
	for(i = 0; i<100 ; i++ ){
	printf("%d\r\n",buffer[m]);
	m = m+1;
	}
	
 // printf("%d\r\n",buffer[m]);
}
void task1( void * pvParameters )
{ 

  uint32_t un_min, un_max, un_prev_data;  
	int i,j;
	int32_t n_brightness;
	float f_temp;
	uint8_t temp[6];
 	max30102_init();
	
	
	printf("\r\n MAX30102  init  \r\n");

	un_min=0x3FFFF;
	un_max=0;
	
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	//read the first 500 samples, and determine the signal range
    for(i=0;i<n_ir_buffer_length;i++)
    {
        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)==1);   //wait until the interrupt pin asserts
        
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
        if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];    //update signal max
    }
	un_prev_data=aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
  //  maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
		
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = pdMS_TO_TICKS(10);
	for(j=1;j<=5;j++) /////////  15 times
	{
		xLastWakeTime = xTaskGetTickCount();
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        }
		//take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
            while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		      	aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			      aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness-=(int)f_temp;
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness+=(int)f_temp;
              if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
			//send samples and calculation result to terminal program through UART
			
				 //   printf("red=");
          //  printf("%i", aun_red_buffer[i]);
          //  printf(", ir=");
           // printf("%i\r\n", aun_ir_buffer[i]);
         //   printf(", HR=%i, ", n_heart_rate); 
          //  printf("HRvalid=%i, ", ch_hr_valid);
          //  printf("SpO2=%i, ", n_sp02);
         //   printf("SPO2Valid=%i\n\r", ch_spo2_valid);
              printf("%d\r\n",aun_ir_buffer[i]);
              
		}
       // maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		 printf("NO.%d test ok\r\n",j);
	    vTaskDelay( xFrequency );
	}
	
	
	
	
	//printf("");
	while(1) /////////  
	{ 
		xLastWakeTime = xTaskGetTickCount();
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		   vul_func();
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        } 
		//take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {   
            un_prev_data=aun_red_buffer[i-1];
            while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness-=(int)f_temp;
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness+=(int)f_temp;
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
			//send samples and calculation result to terminal program through UART
			
				 //   printf("red=");
          //  printf("%i", aun_red_buffer[i]);
          //  printf(", ir=");
           // printf("%i\r\n", aun_ir_buffer[i]);
         //   printf(", HR=%i, ", n_heart_rate); 
          //  printf("HRvalid=%i, ", ch_hr_valid);
          //  printf("SpO2=%i, ", n_sp02);
         //   printf("SPO2Valid=%i\n\r", ch_spo2_valid);
              printf("%d\r\n",aun_ir_buffer[i]);

		}
       // maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		 printf("NO.%d test ok\r\n",j);
	    vTaskDelay( xFrequency );
	}
 //vTaskDelete(NULL); 
}

/* �������tansmit hello */
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
"\xC1\x6B\xDF\xF8\x30\x20\x91\x42\x0F\xD0\xDF\xF8\x2C\x20\x91\x42"//16
"\x0D\xD0\xDF\xF8\x28\x20\x91\x42\x09\xD0\xDF\xF8\x24\x20\x91\x42"
"\x05\xD0\xDF\xF8\x20\x20\x91\x42\xFF\xD0\xDF\xF8\x1C\xF0\xDF\xF8"
"\x1C\xF0\x00\x00\x01\x00\x00\x08\x02\x00\x00\x08\x03\x00\x00\x08"
"\x04\x00\x00\x08\x74\x32\x00\x08\x01\x90\x00\x20\x05\x90\x00\x20"
"";


static uint8_t fpb_patch2018_16601[] = ""
"\x02\x6B\x41\x68\x89\x68\x0E\x39"
"\x8A\x42\x02\xD8\x01\x6B\x14\x29\x06\xD2\x4F\xF0\xFF\x31\x41\x62"
"\xC1\x6B\x89\x1C\xC1\x63\x04\xE0\x00\x21\x41\x62\xC1\x6B\x89\x1C"
"\xC1\x63"
"\xdf\xf8\x04\xf0\x00\x00\xe9\x03\x00\x08" //jumpback
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
	
{	 	 break_point_instruction_addr = 0x08003da6;
	   vTaskDelay(pdMS_TO_TICKS(15000));
	   patch_address = 0x20009000;
	   patch_dispatch_address = 0x20007000;
	   uint32_t i;
	   for ( i = 0; i < sizeof(fpb_patch2018_16601); i=i+1){           
					  *(uint8_t *)(patch_address+i) = *(uint8_t *)(fpb_patch2018_16601+i);
        }
     for ( i = 0; i < sizeof(patch_dispatch); i=i+1){           
					  *(uint8_t *)(patch_dispatch_address+i) = *(uint8_t *)(patch_dispatch+i);
        }
		 
		 debug_monitor_init();
		 uint32_t replace = (break_point_instruction_addr & 0x2) == 0 ? 1 : 2;
	   uint32_t fp_comp = (break_point_instruction_addr & ~0x3) | 0x1 | (replace << 30);
				
		 FPB->COMP[5] = fp_comp;
	   //dwt_trace_test();
				printf("task2 finish");
		 vTaskDelete(NULL); 
}


void ledblinktask( void * pvParameters )
{  while(1){
     
	   vTaskDelay(pdMS_TO_TICKS(200));
	   Led_ON();
     vTaskDelay(pdMS_TO_TICKS(200));
     Led_OFF();

}
    


}
