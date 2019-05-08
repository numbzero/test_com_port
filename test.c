#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main(void)
{
	HANDLE			h_comm;
	DCB				dcb_serial_param = {0};
	COMMTIMEOUTS	timeouts = {0};
	char			command = 0;
	DWORD			d_nbr_bytes_to_write;
	DWORD			d_nbr_bytes_writen = 0;
	
	/* COM port */
	h_comm = CreateFile("COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (h_comm == INVALID_HANDLE_VALUE)
	{
		printf("[-] Error in opening serial port.\n");
		return 1;
	}
	else
		printf("[+] Opening serial port.\n");
	dcb_serial_param.DCBlength = sizeof(dcb_serial_param);
	if (!GetCommState(h_comm, &dcb_serial_param))
	{
		printf("[-] Error in getting port state.\n");		
		return 1;
	}
	else
		printf("[+] Get port state.\n");
	dcb_serial_param.BaudRate = CBR_9600;
	dcb_serial_param.ByteSize = 8;
	dcb_serial_param.StopBits = TWOSTOPBITS;
	dcb_serial_param.Parity = NOPARITY;
	if (!SetCommState(h_comm, &dcb_serial_param))
	{
		printf("[-] Error in setting port state.\n");				
		return 1;
	}
	else
		printf("[+] Set port state.\n");
	timeouts.ReadIntervalTimeout 		 = 50;
	timeouts.ReadTotalTimeoutConstant 	 = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant 	 = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (!SetCommTimeouts(h_comm, &timeouts))
	{
		printf("[-] Error in setting port timeouts.\n");				
		return 1;
	}
	else
		printf("[+] Set port timeouts.\n");
	while (1)
	{
		command = getch();
		if (command == 'q' || command == 'Q')
			break;
		d_nbr_bytes_to_write = sizeof(char);
		if (!WriteFile(h_comm, &command, d_nbr_bytes_to_write, &d_nbr_bytes_writen, NULL))
			printf("--> Error writing data to port !!!\n");				
		else
			printf("--> Byte [%x] was sent to port.\n", command);
	}
	printf("[*] Exit.\n");
	CloseHandle(h_comm);
	return 0;
}