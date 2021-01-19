#include "def.h"


/***********************************************************************************/
#define	HSE_FREQ	12	// MHz
#define	PLL_MUL		5	// HCLK = 72 MHz
//#define	HSE_FREQ	8	// MHz
//#define	PLL_MUL		9	// HCLK = 80 MHz
#define	_PLL_
void StartupClockConfigure(void)
{
	// смещение таблицы векторов прерываний
	// SCB - System Control Block
	// VTOR - Vector Table Offset Register
	SCB->VTOR = 0x08000000;	// таблица располагается в начале Flash-памяти
	
	// После RESET тактирование осуществляется от HSI = 8 МГц
	
	// Запускаем генератор HSE
	MDR_RST_CLK->HS_CONTROL = 0x0001;	// режим осциллятора
	
	while((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY) != RST_CLK_CLOCK_STATUS_HSE_RDY);
	
	// Переключаем источник тактирования CPU_C1 с HSI на HSE
	// CPU_C1 = HSE_FREQ : CPU_C2 = CPU_C1 : CPU_C3 = CPU_C2 : HCLK = CPU_C3 = HSE_FREQ
	MDR_RST_CLK->CPU_CLOCK = 0x0102;
	
	// выключаем генераторы HSI и LSI
	MDR_BKP->REG_0F = 0x0000;
	
#ifdef	_PLL_
	// коэффициент умножения входного тактового сигнала
	MDR_RST_CLK->PLL_CONTROL = (PLL_MUL << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos);
	// запускаем PLL
	MDR_RST_CLK->PLL_CONTROL |= RST_CLK_PLL_CONTROL_PLL_CPU_ON;
	while((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY) != RST_CLK_CLOCK_STATUS_PLL_CPU_RDY);
	// подключаем PLL к тактированию: CPU_C2 = PLL_CPU
	MDR_RST_CLK->CPU_CLOCK |= RST_CLK_CPU_CLOCK_CPU_C2_SEL;
#endif	// _PLL_
}


/***********************************************************************************/
/********************			 M A I N			 *******************************/
/***********************************************************************************/
void main(void)
{
	StartupClockConfigure();
	
	while(1);
}