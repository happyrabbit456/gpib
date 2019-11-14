// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;


/*****************************************************************************
 * Refer to the language interface documentation for details on
 * which header and .obj files to include in your project.
 *****************************************************************************/
#include "ni4882.h"
#include <stdio.h>

void GpibError(const char * msg); /* Error function declaration              */

void sampleTest();
bool DMM_Write(string strWrite);
bool DMM_Read(string strWrite, string &strRead);
bool DMM_ReadCurrent(double &value);

int Device = 0;                   /* Device unit descriptor                  */
int BoardIndex = 0;               /* Interface Index (GPIB0=0,GPIB1=1,etc.)  */


//caijx
int   PrimaryAddress = 22;// 2;      /* Primary address of the device           */
int   SecondaryAddress = 0;    /* Secondary address of the device         */
char  Buffer[101];             /* Read buffer                             */

/**
 * double转换为string
 */
string convertToString(double d) {
	ostringstream os;
	if (os << d)
		return os.str();
	return "invalid conversion";
}
/**
 * double转string
 */
double convertFromString(string str) {
	istringstream iss(str);
	double x;
	if (iss >> x)
		return x;
	return 0.0;
}

int main() {

	//官方例子
	sampleTest();

	double value = 0.0;
	DMM_ReadCurrent(value);

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

void sampleTest()
{
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
}


bool DMM_Write(string strWrite)
{
	Device = ibdev(                /* Create a unit descriptor handle         */
		BoardIndex,              /* Board Index (GPIB0 = 0, GPIB1 = 1, ...) */
		PrimaryAddress,          /* Device primary address                  */
		SecondaryAddress,        /* Device secondary address                */
		T10s,                    /* Timeout setting (T10s = 10 seconds)     */
		1,                       /* Assert EOI line at end of write         */
		0);                      /* EOS termination mode                    */
	if (Ibsta() & ERR) {           /* Check for GPIB Error                    */
		GpibError("ibdev Error");

		return false;
	}

	ibclr(Device);                 /* Clear the device                        */
	if (Ibsta() & ERR) {
		GpibError("ibclr Error");

		return false;
	}

	ibwrt(Device, strWrite.c_str(), strWrite.length());     /* Send the identification query command   */
	if (Ibsta() & ERR) {
		GpibError("ibwrt Error");

		return false;
	}

	ibonl(Device, 0);
	if (Ibsta() & ERR) {
		GpibError("ibonl Error");

		return false;
	}

	return true;

	/*
	//Open and intialize an GPIB instrument

	int dev = GPIB.ibdev(0, addr, 0, (int)GPIB.gpib_timeout.T1s, 1, 0);
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in initializing the GPIB instrument.");
		return false;
	}

	//clear the specific GPIB instrument
	GPIB.ibclr(dev);
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in clearing the GPIB device.");
		return false;
	}

	//Write a string command to a GPIB instrument using the ibwrt() command
	GPIB.ibwrt(dev, strWrite, strWrite.Length);
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in writing the string command to the GPIB instrument.");
		return false;
	}

	//Offline the GPIB interface card
	GPIB.ibonl(dev, 0);
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in offline the GPIB interface card.");
		return false;
	}
	return true;
	*/
}

bool DMM_Read(string strWrite, string &strRead)
{
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

		return false;
	}

	ibclr(Device);                 /* Clear the device                        */
	if (Ibsta() & ERR) {
		GpibError("ibclr Error");

		return false;
	}

	/*****************************************************************************
	 * Main Application Body - Write the majority of your GPIB code here.
	 *****************************************************************************/

	ibwrt(Device, strWrite.c_str(), strWrite.length());     /* Send the identification query command   */
	if (Ibsta() & ERR) {
		GpibError("ibwrt Error");

		return false;
	}

	ibrd(Device, Buffer, 100);     /* Read up to 100 bytes from the device    */
	if (Ibsta() & ERR) {
		GpibError("ibrd Error");
	}

	Buffer[Ibcnt()] = '\0';        /* Null terminate the ASCII string         */

	//printf("%s\n", Buffer);        /* Print the device identification         */

	strRead = Buffer;

	ibonl(Device, 0);
	if (Ibsta() & ERR) {
		GpibError("ibonl Error");

		return false;
	}

	return true;

	/*
	StringBuilder str = new StringBuilder(100);
	//Open and intialize an GPIB instrument
	int dev = GPIB.ibdev(0, addr, 0, (int)GPIB.gpib_timeout.T1s, 1, 0);
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in initializing the GPIB instrument.");
		return false;
	}

	//clear the specific GPIB instrument
	GPIB.ibclr(dev);
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in clearing the GPIB device.");
		return false;
	}

	//Write a string command to a GPIB instrument using the ibwrt() command
	GPIB.ibwrt(dev, strWrite, strWrite.Length);
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in writing the string command to the GPIB instrument.");
		return false;
	}

	//Read the response string from the GPIB instrument using the ibrd() command
	GPIB.ibrd(dev, str, 100);
	strRead = str.ToString();
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in reading the response string from the GPIB instrument.");
		return false;
	}

	//Offline the GPIB interface card
	GPIB.ibonl(dev, 0);
	GPIB.gpib_get_globals(out ibsta, out iberr, out ibcnt, out ibcntl);
	if ((ibsta & (int)GPIB.ibsta_bits.ERR) != 0)
	{
		//MessageBox.Show("Error in offline the GPIB interface card.");
		return false;
	}
	return true;
	*/
}

/// <summary>
/// 数字万用表：读取电流
/// </summary>
bool DMM_ReadCurrent(double &value)
{
	string strRead;
	string strWriteCmd = "CONF:CURR:DC DEF";
	if (!DMM_Write(strWriteCmd)) return false;
	strWriteCmd = "READ?";
	if (!DMM_Read(strWriteCmd, strRead)) return false;

	printf("%s\n", strRead.c_str());
	value = convertFromString(strRead);

	return true;
	/*
	try
	{
		string strRead = "";
		if (write(DMMaddr, "CONF:CURR:DC DEF") == false) return false;
		if (read(DMMaddr, "READ?", strRead) == false) return false;
		value = Convert.ToDouble(strRead);
		return true;
	}
	catch
	{
		//MessageBox.Show("Voltage Read Fail!");
		return false;
	}
	*/
}




