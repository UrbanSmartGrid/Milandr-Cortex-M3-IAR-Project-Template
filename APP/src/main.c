#include "def.h"


/***********************************************************************************/
#define	HSE_FREQ	12	// MHz
#define	PLL_MUL		5	// HCLK = 72 MHz
//#define	HSE_FREQ	8	// MHz
//#define	PLL_MUL		9	// HCLK = 80 MHz
#define	_PLL_
void StartupClockConfigure(void)
{
	// �������� ������� �������� ����������
	// SCB - System Control Block
	// VTOR - Vector Table Offset Register
	SCB->VTOR = 0x08000000;	// ������� ������������� � ������ Flash-������
	
	// ����� RESET ������������ �������������� �� HSI = 8 ���
	
	// ��������� ��������� HSE
	MDR_RST_CLK->HS_CONTROL = 0x0001;	// ����� �����������
	
	while((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY) != RST_CLK_CLOCK_STATUS_HSE_RDY);
	
	// ����������� �������� ������������ CPU_C1 � HSI �� HSE
	// CPU_C1 = HSE_FREQ : CPU_C2 = CPU_C1 : CPU_C3 = CPU_C2 : HCLK = CPU_C3 = HSE_FREQ
	MDR_RST_CLK->CPU_CLOCK = 0x0102;
	
	// ��������� ���������� HSI � LSI
	MDR_BKP->REG_0F = 0x0000;
	
#ifdef	_PLL_
	// ����������� ��������� �������� ��������� �������
	MDR_RST_CLK->PLL_CONTROL = (PLL_MUL << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos);
	// ��������� PLL
	MDR_RST_CLK->PLL_CONTROL |= RST_CLK_PLL_CONTROL_PLL_CPU_ON;
	while((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY) != RST_CLK_CLOCK_STATUS_PLL_CPU_RDY);
	// ���������� PLL � ������������: CPU_C2 = PLL_CPU
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