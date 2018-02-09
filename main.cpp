#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <windows.h>		// for windows specific keywords in ftd2xx.h
#include "ftd2xx.h"			// Header file for ftd2xx.lib

#define version 1.0

using namespace std;

int main(int argc, char *argv[]) {
	
	FT_HANDLE ft_handle;
	FT_STATUS ft_status;
	UCHAR Mode = 0x01;              // Select Chip mode as Asynchronous Bit Bang
	UCHAR Mask = 0xff;              // 8 bit port all output
	UCHAR data = 0x00; 
	DWORD lpBytesWritten = 0;
	
	if (argc == 2 && 
	         (strcmp(argv[1], "-help") == 0 ||
			  strcmp(argv[1], "-h") == 0 ||
			  strcmp(argv[1], "--help") == 0 ||
			  strcmp(argv[1], "/h") == 0)) {
		printf("\nFTDI USB Relay Board control application\n");
		printf("           By: Luis Giron\n");
		printf("            version %0.1f\n\n", version);
		printf("usage: usbrelay <bit_value>\n\n");
		printf("bit_value  :value used to turn on and off relays\n");
		printf("            this value can range from 0-15. 0=all off and 15=all on\n");
		return 0;
	}
	
	ft_status = FT_Open(0,&ft_handle);

	if(ft_status == FT_OK) {
		printf("Open was successful\n");
	}
	else { 
		printf("Open failed\n");
		return 1;
	}
	
    ft_status = FT_SetBitMode(ft_handle,Mask,Mode); //sets Asynchronous BBM
	if(ft_status != FT_OK) {
		printf("Failed to set bitmode\n");
		return 1;
	}
	
	ft_status = FT_SetBaudRate(ft_handle, 9600);
	if(ft_status != FT_OK) {
		printf("Failed to set baudrate\n");
		return 1;
	}
	
	ft_status = FT_SetDataCharacteristics(ft_handle,       // Handle of the chip(FT232)
                          FT_BITS_8,       // No of Data bits = 8
                          FT_STOP_BITS_1,  // No of Stop Bits = 1
                          FT_PARITY_NONE   // Parity = NONE
                          );
	if(ft_status != FT_OK) {
		printf("Failed to Set Data Characteristics\n");
		return 1;
	}

	ft_status = FT_SetFlowControl( ft_handle,    // Handle of the chip(FT232)
	                   FT_FLOW_NONE, // No Flow control
	                   0x00,         // Null since no Flow control
	                   0x00          // Null since no Flow control
	                  );
	if(ft_status != FT_OK) {
		printf("Failed to Set Flow Control\n");
		return 1;
	}
	
	if (argc == 2 && argv[1][0] != '-' && argv[1][0] != '/') { // User passed in a value
		data = atoi(argv[1]);
		printf("Write bit_value value\n");
		ft_status = FT_Write( ft_handle,         // Handle to the chip
							  &data,           // byte to be Txed
							  sizeof(data),    // sizeof(TxByte) = char
							  &lpBytesWritten    // No of Bytes written to the port
							 );
	}
	else { // turn off all relays
		data = 0x00;
		printf("Turn off all relays\n");
		ft_status = FT_Write( ft_handle,         // Handle to the chip
							  &data,           // byte to be Txed
							  sizeof(data),    // sizeof(TxByte) = char
							  &lpBytesWritten    // No of Bytes written to the port
							 );
	}
	if(ft_status != FT_OK) {
		printf("Failed to write data\n");
	}
	else if (lpBytesWritten > 0) {
		printf("Data was written successfully\n");
	}

	ft_status = FT_Close(ft_handle);
	printf("Done\n");
	
	return 0;
}