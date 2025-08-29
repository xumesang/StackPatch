
#include "stm32f4xx_hal.h"
#include "fpb.h"
#include "stm32f4xx_it.h"
#define FPB_CTRL_REGISTER   0xE0002000UL
#define FPB_REMAP_REGISTER  0xE0002004UL
#define FPB_COMP0           0xE0002008UL
#define FPB_COMP1           0xE000200CUL
#define FPB_COMP2           0xE0002010UL
#define FPB_COMP3           0xE0002014UL
#define FPB_COMP4           0xE0002018UL
#define FPB_COMP5           0xE000201CUL
#define FPB_COMP6           0xE0002020UL
#define FPB_COMP7           0xE0002024UL
#define REMAP_TABLE_ADDR (0x20007000UL)  //SRAM
#define HWREG(x) (*((volatile uint32_t *)(x)))
#define FLASH_REMAP_TABLE 0x0800c000  //3
#define FLASH_COMP_TABLE 0x08008000 //2
#define FLASH_NO_OF_COUNTERS 0x08004000 //1
#define LED_ON() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
#define LED_OFF() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
#define LITTLE_ENDIAN(x) ((x & 0xFFFF0000) >> 16) | ((x & 0x0000FFFF) << 16)
volatile uint32_t old_instruction_address;
volatile uint32_t new_instruction_address;
volatile uint8_t link;
uint8_t Tx_str1[] = "please send old_instruction_address\r\n" ;
uint8_t Tx_str2[] = "please send new_instruction_address\r\n";
uint8_t Tx_str3[] = "please send link\r\n";
uint8_t Tx_str4[] = "please send patch length\r\n";
uint8_t Tx_str5[] = "please send patch\r\n";
uint8_t Rx_dat = 0;
__asm void FPB_Handler(void)
{
	NOP
	NOP
	NOP
	NOP
	//IMPORT leave_fpb_bpkt
//	B leave_fpb_bpkt
	PUSH {r0-r3, r12, lr}
	TST lr, #4
	ITE EQ
	MRSEQ r0, MSP
	MRSNE r0, PSP
	MOVS r1, #3
//	BL leave_fpb_bpkt
	// todo: return in other address
//	MOV r5, r0
	POP {r0-r3, r12, lr}
//	MOV r0, r5
	BX LR
}

void flash_erase(uint32_t t)
{
    
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    uint32_t sect = 0;                     
    if(t == 0x08004000){
			sect = 1;
		}else if(t == 0x08008000){
			sect = 2;
		}else if(t == 0x0800c000){
			sect = 3;
		}else if(t == 0x8010000){
			sect = 4;
		}else{
			HAL_FLASH_Lock();
			return;
		} 
    FLASH_Erase_Sector(sect,FLASH_VOLTAGE_RANGE_3);
    HAL_FLASH_Lock();
}

void flash_write(uint32_t TypeProgram, uint32_t Address, uint64_t Data){
   HAL_FLASH_Unlock();
   HAL_FLASH_Program(TypeProgram,Address,Data);
	 HAL_FLASH_Lock();	 
}



void write_fpb_comp_register(
    uint8_t comparator_index,
    uint32_t old_instr_addr
){

    uint32_t comparator_value = old_instr_addr | 0x01; 
//   Updating FPB_COMPs in SRAM 
    if(comparator_index == 0)      HWREG(FPB_COMP0) = comparator_value;
    else if(comparator_index == 1) HWREG(FPB_COMP1) = comparator_value;
    else if(comparator_index == 2) HWREG(FPB_COMP2) = comparator_value;
    else if(comparator_index == 3) HWREG(FPB_COMP3) = comparator_value;
    else if(comparator_index == 4) HWREG(FPB_COMP4) = comparator_value;
    else if(comparator_index == 5) HWREG(FPB_COMP5) = comparator_value;



}
uint32_t calculate_branch_instruction(
    uint32_t curr_instr_addr,
    uint32_t target_instr_addr)
{

    uint32_t offset = target_instr_addr - curr_instr_addr - 4;
    
    uint32_t offset_msb10 = (offset >> 12) & 0x3FF;
    uint32_t offset_lsb11 = (offset >> 1) & 0x07FF;

    uint32_t s = (offset & (1 << 24)) >> 24;
    uint32_t i1 = (offset & (1 << 23)) >> 23;
    uint32_t i2 = (offset & (1 << 22)) >> 22;

    uint32_t j1 = 0x01 & ((~i1) ^ s);
    uint32_t j2 = 0x01 & ((~i2) ^ s);


    uint32_t b_instr_msb16 = ((0x1E << 11) | (s << 10) | offset_msb10) & 0xFFFF; 
    uint32_t b_instr_lsb16 = ((0x02 << 14) | (j1 << 13) | (0x01 << 12) | (j2 << 11) | offset_lsb11) & 0xFFFF;

    uint32_t b_instr = ((uint32_t)b_instr_msb16 << 16) | b_instr_lsb16;

    return b_instr;
}
uint32_t calculate_branch_link_instruction(
    uint32_t curr_instr_addr,
    uint32_t target_instr_addr
)
{
    uint32_t b_instr = calculate_branch_instruction(curr_instr_addr, target_instr_addr);
    //14th bit is set as per documentation
    uint32_t bl_instr = b_instr | 0x00004000;
    return bl_instr;
}
void hera_remap_instr(
    uint32_t old_instr_addr,
    uint32_t new_instr_addr,
    uint32_t comp_index,
    bool link
)
{
    
    uint32_t remap_instr;

    if (link == TRUE){ 
        remap_instr = calculate_branch_link_instruction(old_instr_addr, new_instr_addr);
    }
    else {
       remap_instr = calculate_branch_instruction(old_instr_addr, new_instr_addr);
    }

    remap_instr = LITTLE_ENDIAN(remap_instr);

    // If aligned no issues
    if(old_instr_addr % 4 == 0){
        
        write_fpb_comp_register(comp_index, old_instr_addr);
        
        //Updating remap table in SRAM and FLASH
        *((uint32_t *)(REMAP_TABLE_ADDR + 4*comp_index)) = remap_instr;
        flash_write(FLASH_TYPEPROGRAM_WORD,FLASH_REMAP_TABLE+4*comp_index,remap_instr);
        uint32_t counter = HWREG(FLASH_NO_OF_COUNTERS);
			  counter = counter + 1;
			  flash_erase(FLASH_NO_OF_COUNTERS);
			  flash_write(FLASH_TYPEPROGRAM_WORD,FLASH_NO_OF_COUNTERS,counter);
     
    }

    // Half-aligned : pain in the arse
    // Multiple comparators must be used
    else{
        uint32_t old_instr[2];
        old_instr[0] = *((uint32_t *)(old_instr_addr & 0xFFFFFFFC));
        old_instr[1] = *((uint32_t *)(old_instr_addr & 0xFFFFFFFC) + 4);

        write_fpb_comp_register(comp_index, old_instr_addr & 0xFFFFFFFC);
        write_fpb_comp_register(comp_index+1, (old_instr_addr & 0xFFFFFFFC) + 4);

        uint32_t instr1 = (((remap_instr & 0x0000FFFF) << 16) | (old_instr[0] & 0x0000FFFF));
        uint32_t instr2 = ((old_instr[1] & 0xFFFF0000) | ((remap_instr & 0xFFFF0000) >> 16));

        //Updating remap table in SRAM and FLASH
        *((uint32_t *)(REMAP_TABLE_ADDR + 4*comp_index)) = instr1; 
        *((uint32_t *)(REMAP_TABLE_ADDR + 4*(comp_index+1))) = instr2;
			   flash_write(FLASH_TYPEPROGRAM_WORD,FLASH_REMAP_TABLE+4*comp_index,instr1);
			   flash_write(FLASH_TYPEPROGRAM_WORD,FLASH_REMAP_TABLE+4*comp_index,instr2);
   
         
        uint32_t counter = HWREG(FLASH_NO_OF_COUNTERS);
        counter = counter + 2;
		    flash_erase(FLASH_NO_OF_COUNTERS);
			  flash_write(FLASH_TYPEPROGRAM_WORD,FLASH_NO_OF_COUNTERS,counter);
    }
    return;
}

void FPB_INIT(){
	  
    
    HWREG(FPB_CTRL_REGISTER) |= 0x03;
    HWREG(FPB_REMAP_REGISTER) = REMAP_TABLE_ADDR;
   
    HWREG(FPB_COMP0) = *((uint32_t *)(FLASH_COMP_TABLE + 0));
    //HWREG(FPB_COMP1) = *((uint32_t *)(FLASH_COMP_TABLE + 4)); 
    //HWREG(FPB_COMP2) = *((uint32_t *)(FLASH_COMP_TABLE + 8));
    //HWREG(FPB_COMP3) = *((uint32_t *)(FLASH_COMP_TABLE + 12)); 
    //HWREG(FPB_COMP4) = *((uint32_t *)(FLASH_COMP_TABLE + 16));
    //HWREG(FPB_COMP5) = *((uint32_t *)(FLASH_COMP_TABLE + 20)); 
		hera_remap_instr(old_instruction_address,new_instruction_address,1,link);
}