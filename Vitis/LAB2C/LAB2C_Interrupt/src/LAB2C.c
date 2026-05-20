
#include "xparameters.h"
#include "xgpio.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"


#define BOARD_ZYBO           


#define BTN_MASK    0x0F    // 4 nút bấm
#define LED_MASK    0x0F    // 4 LEDs


#define BTNS_GPIO_BASEADDR   XPAR_AXI_GPIO_0_BASEADDR
#define LEDS_GPIO_BASEADDR   XPAR_AXI_GPIO_1_BASEADDR
#define INTC_BASEADDR        XPAR_XSCUGIC_0_BASEADDR

#define BTNS_INTR_ID    XPAR_FABRIC_AXI_GPIO_0_INTR


#define BTN_CHANNEL     1             
#define LED_CHANNEL     1
#define BTN_INT_MASK    XGPIO_IR_CH1_MASK


XGpio   BTNInst;       
XGpio   LEDInst;       
XScuGic INTCInst;       

static int led_counter = 0;     


void BTN_Intr_Handler(void *InstancePtr)
{
    (void)InstancePtr;  
    XGpio_InterruptDisable(&BTNInst, BTN_INT_MASK);

    if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT_MASK) != BTN_INT_MASK) {
        return;
    }

    int btn_value = XGpio_DiscreteRead(&BTNInst, BTN_CHANNEL) & BTN_MASK;


    led_counter = (led_counter + btn_value) & LED_MASK;

    XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, led_counter);

    (void)XGpio_InterruptClear(&BTNInst, BTN_INT_MASK);

    XGpio_InterruptEnable(&BTNInst, BTN_INT_MASK);
}


int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
    
    XGpio_InterruptEnable(&BTNInst, BTN_INT_MASK);
    XGpio_InterruptGlobalEnable(&BTNInst);


    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
        (Xil_ExceptionHandler)XScuGic_InterruptHandler,
        XScuGicInstancePtr);

    Xil_ExceptionEnable();

    return XST_SUCCESS;
}


int IntcInitFunction(UINTPTR BaseAddress, XGpio *GpioInstancePtr)
{
    XScuGic_Config *IntcConfig;
    int status;

    IntcConfig = XScuGic_LookupConfig(BaseAddress);
    if (IntcConfig == NULL) {
        return XST_FAILURE;
    }

    status = XScuGic_CfgInitialize(&INTCInst, IntcConfig,
                                    IntcConfig->CpuBaseAddress);
    if (status != XST_SUCCESS) {
        return status;
    }

    XScuGic_SetPriorityTriggerType(&INTCInst, BTNS_INTR_ID, 0xA0, 0x3);

    status = XScuGic_Connect(&INTCInst, BTNS_INTR_ID,
                             (Xil_InterruptHandler)BTN_Intr_Handler,
                             (void *)GpioInstancePtr);
    if (status != XST_SUCCESS) {
        return status;
    }

    XScuGic_Enable(&INTCInst, BTNS_INTR_ID);

    return InterruptSystemSetup(&INTCInst);
}

int main(void)
{
    int status;

    xil_printf("\r\n==========================================\r\n");
    xil_printf("  Lab 2C - Interrupt Counter Demo\r\n");
    xil_printf("==========================================\r\n");

    status = XGpio_Initialize(&LEDInst, LEDS_GPIO_BASEADDR);
    if (status != XST_SUCCESS) {
        xil_printf("[ERROR] LED GPIO khoi tao that bai\r\n");
        return XST_FAILURE;
    }

    status = XGpio_Initialize(&BTNInst, BTNS_GPIO_BASEADDR);
    if (status != XST_SUCCESS) {
        xil_printf("[ERROR] Button GPIO khoi tao that bai\r\n");
        return XST_FAILURE;
    }

    XGpio_SetDataDirection(&LEDInst, LED_CHANNEL, 0x00);        // output
    XGpio_SetDataDirection(&BTNInst, BTN_CHANNEL, BTN_MASK);    // input

    XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, 0x00);

    status = IntcInitFunction(INTC_BASEADDR, &BTNInst);
    if (status != XST_SUCCESS) {
        xil_printf("[ERROR] Interrupt setup that bai\r\n");
        return XST_FAILURE;
    }

    xil_printf("[OK] San sang! Bam cac nut de tang counter\r\n");
#ifdef BOARD_ZEDBOARD
    xil_printf("     BTNC=+1, BTNU=+2, BTNL=+4, BTNR=+8, BTND=+16\r\n\r\n");
#else
    xil_printf("     BTN0=+1, BTN1=+2, BTN2=+4, BTN3=+8\r\n\r\n");
#endif

    while (1) {
    }

    return 0;
}