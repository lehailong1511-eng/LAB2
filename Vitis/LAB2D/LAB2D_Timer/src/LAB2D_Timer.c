#include "xparameters.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"

#define SW_BASEADDR       XPAR_XGPIO_0_BASEADDR
#define LED_BASEADDR      XPAR_XGPIO_1_BASEADDR
#define TMR_BASEADDR      XPAR_XTMRCTR_0_BASEADDR
#define INTC_BASEADDR     XPAR_XSCUGIC_0_BASEADDR 


#define TMR_INTR_ID       XPAR_FABRIC_AXI_TIMER_0_INTR
#define SW_INTR_ID        XPAR_FABRIC_AXI_GPIO_0_INTR

XGpio Gpio_Sw;
XGpio Gpio_Led;
XTmrCtr TimerCounter;
XScuGic Intc;

volatile u8 led_data = 0;
volatile u8 is_running = 1; 

void Timer_InterruptHandler(void *CallBackRef, u8 TmrCtrNumber) {
    XTmrCtr *InstancePtr = (XTmrCtr *)CallBackRef;
    
    if (XTmrCtr_IsExpired(InstancePtr, TmrCtrNumber)) {
        if (is_running) {
            led_data++; // Tăng giá trị đếm nhị phân
            XGpio_DiscreteWrite(&Gpio_Led, 1, led_data);
        }
    }
}

void Button_InterruptHandler(void *InstancePtr) {
    XGpio *GpioPtr = (XGpio *)InstancePtr;
    
    XGpio_InterruptClear(GpioPtr, 1);
    
    is_running = !is_running;
    
    if (is_running) {
        xil_printf("=> Timer RESUMED\r\n");
    } else {
        xil_printf("=> Timer PAUSED\r\n");
    }
}

int SetupInterruptSystem(XScuGic *IntcInstancePtr, XGpio *GpioInstancePtr, XTmrCtr *TmrInstancePtr) {
    int Status;
    XScuGic_Config *IntcConfig;

    IntcConfig = XScuGic_LookupConfig(INTC_BASEADDR);
    Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
(Xil_ExceptionHandler)XScuGic_InterruptHandler, 
                                 IntcInstancePtr);
    Xil_ExceptionEnable();

    Status = XScuGic_Connect(IntcInstancePtr, SW_INTR_ID, 
                             (Xil_ExceptionHandler)Button_InterruptHandler, 
                             (void *)GpioInstancePtr);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    Status = XScuGic_Connect(IntcInstancePtr, TMR_INTR_ID, 
                             (Xil_ExceptionHandler)XTmrCtr_InterruptHandler, 
                             (void *)TmrInstancePtr);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    XScuGic_Enable(IntcInstancePtr, SW_INTR_ID);
    XScuGic_Enable(IntcInstancePtr, TMR_INTR_ID);

    XGpio_InterruptEnable(GpioInstancePtr, 1);
    XGpio_InterruptGlobalEnable(GpioInstancePtr);

    return XST_SUCCESS;
}

int main() {
    int Status;

    xil_printf("\r\n--- KHOI DONG HE THONG TIMER & NGAT ---\r\n");

    XGpio_Initialize(&Gpio_Sw, SW_BASEADDR);
    XGpio_Initialize(&Gpio_Led, LED_BASEADDR);
    XGpio_SetDataDirection(&Gpio_Sw, 1, 0xFFFFFFFF);
    XGpio_SetDataDirection(&Gpio_Led, 1, 0x00000000);

    Status = XTmrCtr_Initialize(&TimerCounter, TMR_BASEADDR);
    if (Status != XST_SUCCESS) {
        xil_printf("Loi: Khong the khoi tao Timer!\r\n");
        return XST_FAILURE;
    }

    XTmrCtr_SetHandler(&TimerCounter, Timer_InterruptHandler, &TimerCounter);
    
    XTmrCtr_SetOptions(&TimerCounter, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
    
    XTmrCtr_SetResetValue(&TimerCounter, 0, 0xFFFFFFFF - 50000000);

    Status = SetupInterruptSystem(&Intc, &Gpio_Sw, &TimerCounter);
    if (Status != XST_SUCCESS) {
        xil_printf("Loi: Khong the cai dat he thong ngat!\r\n");
        return XST_FAILURE;
    }

    xil_printf("Cau hinh hoan tat. Bat dau dem nhi phan!\r\n");
    xil_printf("Nhan nut (giao tiep qua gpio_sw) de Pause/Resume.\r\n");

    XTmrCtr_Start(&TimerCounter, 0);
    while (1) {
    }

    return 0;
}