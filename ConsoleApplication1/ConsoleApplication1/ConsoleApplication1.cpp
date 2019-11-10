// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

/*
int main()
{
    std::cout << "Hello World!\n"; 
}
*/

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件



/*****************************************************************************
 * Refer to the language interface documentation for details on
 * which header and .obj files to include in your project.
 *****************************************************************************/
#include "ni4882.h"
#include <stdio.h>

void GpibError(const char * msg); /* Error function declaration              */

int Device = 0;                   /* Device unit descriptor                  */
int BoardIndex = 0;               /* Interface Index (GPIB0=0,GPIB1=1,etc.)  */

int main() {
	//caijx
	int   PrimaryAddress = 22;// 2;      /* Primary address of the device           */
	int   SecondaryAddress = 0;    /* Secondary address of the device         */
	char  Buffer[101];             /* Read buffer                             */

 /*****************************************************************************
  * Initialization - Done only once at the beginning of your application.
  *****************************************************************************/

	Device = ibdev(                /* Create a unit descriptor handle         */
		BoardIndex,              /* Board Index (GPIB0 = 0, GPIB1 = 1, ...) */
		PrimaryAddress,          /* Device primary address                  */
		SecondaryAddress,        /* Device secondary address                */
		T10s,                    /* Timeout setting (T10s = 10 seconds)     */
		1,                       /* Assert EOI line at end of write         */
		0);                      /* EOS termination mode                    */
	if (Ibsta() & ERR) {           /* Check for GPIB Error                    */
		GpibError("ibdev Error");
	}

	ibclr(Device);                 /* Clear the device                        */
	if (Ibsta() & ERR) {
		GpibError("ibclr Error");
	}

	/*****************************************************************************
	 * Main Application Body - Write the majority of your GPIB code here.
	 *****************************************************************************/

	ibwrt(Device, "*IDN?", 5);     /* Send the identification query command   */
	if (Ibsta() & ERR) {
		GpibError("ibwrt Error");
	}

	ibrd(Device, Buffer, 100);     /* Read up to 100 bytes from the device    */
	if (Ibsta() & ERR) {
		GpibError("ibrd Error");
	}

	Buffer[Ibcnt()] = '\0';        /* Null terminate the ASCII string         */

	printf("%s\n", Buffer);        /* Print the device identification         */


 /*****************************************************************************
  * Uninitialization - Done only once at the end of your application.
  *****************************************************************************/

	ibonl(Device, 0);              /* Take the device offline                 */
	if (Ibsta() & ERR) {
		GpibError("ibonl Error");
	}

	return 0;

}


/*****************************************************************************
 *                      Function GPIBERROR
 * This function will notify you that a NI-488 function failed by
 * printing an error message.  The status variable IBSTA will also be
 * printed in hexadecimal along with the mnemonic meaning of the bit
 * position. The status variable IBERR will be printed in decimal
 * along with the mnemonic meaning of the decimal value.  The status
 * variable IBCNT will be printed in decimal.
 *
 * The NI-488 function IBONL is called to disable the hardware and
 * software.
 *
 * The EXIT function will terminate this program.
 *****************************************************************************/
void GpibError(const char *msg) {

	printf("%s\n", msg);

	printf("Ibsta() = 0x%x  <", Ibsta());
	if (Ibsta() & ERR)  printf(" ERR");
	if (Ibsta() & TIMO)  printf(" TIMO");
	if (Ibsta() & END)  printf(" END");
	if (Ibsta() & SRQI)  printf(" SRQI");
	if (Ibsta() & RQS)  printf(" RQS");
	if (Ibsta() & CMPL)  printf(" CMPL");
	if (Ibsta() & LOK)  printf(" LOK");
	if (Ibsta() & REM)  printf(" REM");
	if (Ibsta() & CIC)  printf(" CIC");
	if (Ibsta() & ATN)  printf(" ATN");
	if (Ibsta() & TACS)  printf(" TACS");
	if (Ibsta() & LACS)  printf(" LACS");
	if (Ibsta() & DTAS)  printf(" DTAS");
	if (Ibsta() & DCAS)  printf(" DCAS");
	printf(" >\n");

	printf("Iberr() = %d", Iberr());
	if (Iberr() == EDVR) printf(" EDVR <Driver error>\n");
	if (Iberr() == ECIC) printf(" ECIC <Not Controller-In-Charge>\n");
	if (Iberr() == ENOL) printf(" ENOL <No Listener>\n");
	if (Iberr() == EADR) printf(" EADR <Address error>\n");
	if (Iberr() == EARG) printf(" EARG <Invalid argument>\n");
	if (Iberr() == ESAC) printf(" ESAC <Not System Controller>\n");
	if (Iberr() == EABO) printf(" EABO <Operation aborted>\n");
	if (Iberr() == ENEB) printf(" ENEB <No GPIB board>\n");
	if (Iberr() == EOIP) printf(" EOIP <Async I/O in progress>\n");
	if (Iberr() == ECAP) printf(" ECAP <No capability>\n");
	if (Iberr() == EFSO) printf(" EFSO <File system error>\n");
	if (Iberr() == EBUS) printf(" EBUS <Command error>\n");
	if (Iberr() == ESTB) printf(" ESTB <Status byte lost>\n");
	if (Iberr() == ESRQ) printf(" ESRQ <SRQ stuck on>\n");
	if (Iberr() == ETAB) printf(" ETAB <Table Overflow>\n");
	if (Iberr() == ELCK) printf(" ELCK <Lock error>\n");
	if (Iberr() == EARM) printf(" EARM <Ibnotify rearm error>\n");
	if (Iberr() == EHDL) printf(" EHDL <Invalid Handle>\n");
	if (Iberr() == EWIP) printf(" EWIP <Wait already in progress>\n");
	if (Iberr() == ERST) printf(" ERST <Notification cancelled due to reset>\n");
	if (Iberr() == EPWR) printf(" EPWR <Power error>\n");

	printf("Ibcnt() = %u\n", Ibcnt());
	printf("\n");

	/* Call ibonl to take the device and interface offline */
	ibonl(Device, 0);

	exit(1);	
}

