/*
 * cmd_proc.c
 *
 *  Created on: Jun 8, 2012
 *  Developer: Victor Verzilov, TRIUMF Diagnostics Group
 *  Rev. 7.4.3.1:  Introduced &(0xFFFF^0x3) style operation. Feb.27.2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "xparameters.h"
#include "xio.h"
#include <xilflash.h>
#include "eth_server.h"


int span_flash_read(XFlash *FlashInstance);
int span_flash_write(XFlash *FlashInstance);
int fpga_2_flash_buffer( );
int flash_buffer_2_fpga( );


extern u8 FlashDataBuffer[DATA_LEN];
extern XFlash FlashInstance;

int cmd_proc(char *cmdString_in , int *value_back, char **string_back, int **array_Ptr, int *array_Dim)
{

  char cmd[50],*sdp,*chp;
  int dataArray[4096];

  Xuint32 cmdlen, val_len;
  Xuint32 invalue, cntr;
  //Xuint32 fifo_wd;


	struct Flash_Data_Struct *Flash_Data_Ptr = &FlashDataBuffer;

	strcpy(cmd, cmdString_in);
	chp = strchr(cmd,'\r');
	*chp = '\0';
	cmdlen = strlen(cmd);

   // xil_printf("\n\r Your cmd is %s  and its length is %d \n\r", cmd,cmdlen);

	if ( cmdlen == 0 ){
    	xil_printf("\n\r Error: Your command doesn't contain any characters \n\r" );  return 0;}
	else {
		if ((sdp = strchr(cmd,' ')) != NULL){				  // There is a space in the command
			*sdp = '\0';
			sdp++;
			if ((chp = strchr(sdp,' ')) != NULL){
				chp = strrchr(sdp,' ');                       // Check if there were more than one spaces
				sdp = ++chp;
			}

			val_len = strlen(sdp);
			if (val_len > 0){
				if (val_len == strspn(sdp,"-1234567890")) {
					invalue = atoi(sdp);
				}
				else { xil_printf("\n\r Error: There is a non digit character in value \n\r" ); return 0;
				}
			}
//		    xil_printf("\n\r Your pv is %s  and the value is %d \n\r", cmd,invalue);
		}
//		else  xil_printf("\n\r Your pv is %s  and there is no value \n\r", cmd);


if ( strcmp(cmd,"CR")== 0 ){XIo_Out32(BRAM_BASE_ADDR+CR,invalue);return 0;}
else if ( strcmp(cmd,"CR?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + CR);return 1;}
else if ( strcmp(cmd,"SR?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + SR);return 1;}
else if ( strcmp(cmd,"FPGA_VER?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + FPGA_VER);return 1;}
else if ( strcmp(cmd,"ERR?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + SR) & 0xFF ;return 1;}
else if ( strcmp(cmd,"STS?")== 0 ){*value_back = (XIo_In32(BRAM_BASE_ADDR + SR)>>8) & 0xFF ;return 1;}
//added for AWAKE, SL, 20160309
else if ( strcmp(cmd,"BPM:DIA")== 0 ){XIo_Out32(BRAM_BASE_ADDR + BPM_DIA,invalue);return 0;}
else if ( strcmp(cmd,"BPM:DIA?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + BPM_DIA);return 1;}
else if ( strcmp(cmd,"TRIG:TH")== 0 ){XIo_Out32(BRAM_BASE_ADDR + TRIG_TH,invalue);return 0;}
else if ( strcmp(cmd,"TRIG:TH?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + TRIG_TH);return 1;}
else if ( strcmp(cmd,"TRIG:DT")== 0 ){XIo_Out32(BRAM_BASE_ADDR + TRIG_DT,invalue);return 0;}
else if ( strcmp(cmd,"TRIG:DT?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + TRIG_DT);return 1;}
else if ( strcmp(cmd,"TRIG:DL")== 0 ){XIo_Out32(BRAM_BASE_ADDR + TRIG_DL,invalue);return 0;}
else if ( strcmp(cmd,"TRIG:DL?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + TRIG_DL);return 1;}
else if ( strcmp(cmd,"EVT:LEN")== 0 ){XIo_Out32(BRAM_BASE_ADDR + EVT_LEN,invalue);return 0;}
else if ( strcmp(cmd,"EVT:LEN?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + EVT_LEN);return 1;}
else if ( strcmp(cmd,"EVT:TAIL")== 0 ){XIo_Out32(BRAM_BASE_ADDR + EVT_TAIL_LEN,invalue);return 0;}
else if ( strcmp(cmd,"EVT:TAIL?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + EVT_TAIL_LEN);return 1;}
else if ( strcmp(cmd,"BL:LEN")== 0 ){XIo_Out32(BRAM_BASE_ADDR + BL_LEN,invalue);return 0;}
else if ( strcmp(cmd,"BL:LEN?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + BL_LEN);return 1;}
else if ( strcmp(cmd,"ABFIFO:WD?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AB_FIFO_WD);return 1;}
else if ( strcmp(cmd,"CDFIFO:WD?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + CD_FIFO_WD);return 1;}

else if ( strcmp(cmd,"SFIFO:WD?")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + S_FIFO_WD_ADDR);return 1;}

else if ( strcmp(cmd,"POWER:A?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + POWER_A); return 1;}
else if ( strcmp(cmd,"POWER:B?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + POWER_B); return 1;}
else if ( strcmp(cmd,"POWER:C?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + POWER_C); return 1;}
else if ( strcmp(cmd,"POWER:D?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + POWER_D); return 1;}

else if ( strcmp(cmd,"CHA:3DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHA_3DB); return 1;}
else if ( strcmp(cmd,"CHA:6DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHA_6DB); return 1;}
else if ( strcmp(cmd,"CHA:9DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHA_9DB); return 1;}
else if ( strcmp(cmd,"CHA:12DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHA_12DB); return 1;}

else if ( strcmp(cmd,"CHB:3DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHB_3DB); return 1;}
else if ( strcmp(cmd,"CHB:6DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHB_6DB); return 1;}
else if ( strcmp(cmd,"CHB:9DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHB_9DB); return 1;}
else if ( strcmp(cmd,"CHB:12DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHB_12DB); return 1;}

else if ( strcmp(cmd,"CHC:3DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHC_3DB); return 1;}
else if ( strcmp(cmd,"CHC:6DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHC_6DB); return 1;}
else if ( strcmp(cmd,"CHC:9DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHC_9DB); return 1;}
else if ( strcmp(cmd,"CHC:12DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHC_12DB); return 1;}

else if ( strcmp(cmd,"CHD:3DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHD_3DB); return 1;}
else if ( strcmp(cmd,"CHD:6DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHD_6DB); return 1;}
else if ( strcmp(cmd,"CHD:9DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHD_9DB); return 1;}
else if ( strcmp(cmd,"CHD:12DB?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CHD_12DB); return 1;}

else if ( strcmp(cmd,"CHA:3DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHA_3DB,invalue);return 0;}
else if ( strcmp(cmd,"CHA:6DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHA_6DB,invalue);return 0;}
else if ( strcmp(cmd,"CHA:9DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHA_9DB,invalue);return 0;}
else if ( strcmp(cmd,"CHA:12DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHA_12DB,invalue);return 0;}

else if ( strcmp(cmd,"CHB:3DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHB_3DB,invalue);return 0;}
else if ( strcmp(cmd,"CHB:6DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHB_6DB,invalue);return 0;}
else if ( strcmp(cmd,"CHB:9DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHB_9DB,invalue);return 0;}
else if ( strcmp(cmd,"CHB:12DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHB_12DB,invalue);return 0;}

else if ( strcmp(cmd,"CHC:3DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHC_3DB,invalue);return 0;}
else if ( strcmp(cmd,"CHC:6DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHC_6DB,invalue);return 0;}
else if ( strcmp(cmd,"CHC:9DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHC_9DB,invalue);return 0;}
else if ( strcmp(cmd,"CHC:12DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHC_12DB,invalue);return 0;}

else if ( strcmp(cmd,"CHD:3DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHD_3DB,invalue);return 0;}
else if ( strcmp(cmd,"CHD:6DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHD_6DB,invalue);return 0;}
else if ( strcmp(cmd,"CHD:9DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHD_9DB,invalue);return 0;}
else if ( strcmp(cmd,"CHD:12DB")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CHD_12DB,invalue);return 0;}

/////////////////////

else if ( strcmp(cmd,"X:INST?")== 0 ){ *value_back = XIo_In32(BRAM_BASE_ADDR + X_INS);return 1;}
else if ( strcmp(cmd,"Y:INST?")== 0 ){ *value_back = XIo_In32(BRAM_BASE_ADDR + Y_INS);return 1;}
else if ( strcmp(cmd,"S:INST?")== 0 ){ *value_back = XIo_In32(BRAM_BASE_ADDR + S_INS);return 1;}
else if ( strcmp(cmd,"X:AVER?")== 0 ){ *value_back = XIo_In32(BRAM_BASE_ADDR + X_AVE);return 1;}
else if ( strcmp(cmd,"Y:AVER?")== 0 ){ *value_back = XIo_In32(BRAM_BASE_ADDR + Y_AVE);return 1;}
else if ( strcmp(cmd,"S:AVER?")== 0 ){ *value_back = XIo_In32(BRAM_BASE_ADDR + S_AVE);return 1;}
else if ( strcmp(cmd,"S:RAW?")== 0 ){ *value_back = XIo_In32(BRAM_BASE_ADDR + S_RAW);return 1;}

else if ( strcmp(cmd,"CH:GAIN:A")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CH_GAIN_A,invalue);return 0;}
else if ( strcmp(cmd,"CH:GAIN:A?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CH_GAIN_A); return 1;}
else if ( strcmp(cmd,"CH:GAIN:B")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CH_GAIN_B,invalue);return 0;}
else if ( strcmp(cmd,"CH:GAIN:B?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CH_GAIN_B); return 1;}
else if ( strcmp(cmd,"CH:GAIN:C")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CH_GAIN_C,invalue);return 0;}
else if ( strcmp(cmd,"CH:GAIN:C?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CH_GAIN_C); return 1;}
else if ( strcmp(cmd,"CH:GAIN:D")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CH_GAIN_D,invalue);return 0;}
else if ( strcmp(cmd,"CH:GAIN:D?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CH_GAIN_D); return 1;}


else if ( strcmp(cmd,"CAL:GAIN:A")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CAL_GAIN_A,invalue);return 0;}
else if ( strcmp(cmd,"CAL:GAIN:A?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CAL_GAIN_A); return 1;}
else if ( strcmp(cmd,"CAL:GAIN:B")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CAL_GAIN_B,invalue);return 0;}
else if ( strcmp(cmd,"CAL:GAIN:B?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CAL_GAIN_B); return 1;}
else if ( strcmp(cmd,"CAL:GAIN:C")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CAL_GAIN_C,invalue);return 0;}
else if ( strcmp(cmd,"CAL:GAIN:C?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CAL_GAIN_C); return 1;}
else if ( strcmp(cmd,"CAL:GAIN:D")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CAL_GAIN_D,invalue);return 0;}
else if ( strcmp(cmd,"CAL:GAIN:D?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CAL_GAIN_D); return 1;}

else if ( strcmp(cmd,"CAL:K")== 0 ){ XIo_Out32(BRAM_BASE_ADDR + CAL_K,invalue);return 0;}
else if ( strcmp(cmd,"CAL:K?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + CAL_K); return 1;}

else if ( strcmp(cmd,"AFE:CTRL")== 0 ){XIo_Out32(BRAM_BASE_ADDR + AFE_CTRL,invalue);return 0;}
else if ( strcmp(cmd,"AFE:CTRL?")== 0 ){ *value_back= XIo_In32(BRAM_BASE_ADDR + AFE_CTRL); return 1;}


////// added, for block fifo data transfer ,SL, 20160311,
else if ( strcmp(cmd,"FFIFO:AB?")== 0 ){

		for(cntr=0;cntr<16;cntr++)  dataArray[cntr]= XIo_In32(BRAM_BASE_ADDR + ABF_FIFO);
		*array_Ptr = &dataArray[0]; *array_Dim = 16;
		return 3;}

else if ( strcmp(cmd,"FFIFO:CD?")== 0 ){

		for(cntr=0;cntr<16;cntr++)  dataArray[cntr]= XIo_In32(BRAM_BASE_ADDR + CDF_FIFO);
		*array_Ptr = &dataArray[0]; *array_Dim = 16;
		return 3;}

else if ( strcmp(cmd,"SFIFO:DATA?")== 0 ){

		for(cntr=0;cntr<16;cntr++)  dataArray[cntr]= XIo_In32(BRAM_BASE_ADDR + S_FIFO_ADDR);
		*array_Ptr = &dataArray[0]; *array_Dim = 16;
		return 3;}

//////////////////////////////////////////////


else if ( strcmp(cmd,"DATA:ALL:INST?")== 0 ){	dataArray[0]= XIo_In32(BRAM_BASE_ADDR + X_INS);
		 dataArray[1] = XIo_In32(BRAM_BASE_ADDR + Y_INS); dataArray[2] = XIo_In32(BRAM_BASE_ADDR + S_INS);
		 dataArray[3] = XIo_In32(BRAM_BASE_ADDR + SR) & 0xFF; dataArray[4] = (XIo_In32(BRAM_BASE_ADDR + SR)>>8) & 0xFF;
		 *array_Ptr = &dataArray[0]; *array_Dim = 5;return 3;}
else if ( strcmp(cmd,"DATA:ALL:AVER?")== 0 ){	dataArray[0]= XIo_In32(BRAM_BASE_ADDR + X_AVE);
		 dataArray[1] = XIo_In32(BRAM_BASE_ADDR + Y_AVE); dataArray[2] = XIo_In32(BRAM_BASE_ADDR + S_AVE);
		 dataArray[3] = XIo_In32(BRAM_BASE_ADDR + SR) & 0xFF; dataArray[4] = (XIo_In32(BRAM_BASE_ADDR + SR)>>8) & 0xFF;
		 *array_Ptr = &dataArray[0]; *array_Dim = 5;return 3;}

else if ( strcmp(cmd,"BUF:WRIT:EN")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + CR);
	invalue = *value_back | 0x200; XIo_Out32(BRAM_BASE_ADDR+CR,invalue);
	for(cntr=0;cntr<20;cntr++){}
	*value_back = XIo_In32(BRAM_BASE_ADDR + CR);
	invalue = *value_back & (0xFFFF^0x200); XIo_Out32(BRAM_BASE_ADDR+CR,invalue);
	return 0;}
else if ( strcmp(cmd,"BUF:WRIT:DIS")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + CR);
	invalue = *value_back | 0x100; XIo_Out32(BRAM_BASE_ADDR+CR,invalue);
	for(cntr=0;cntr<20;cntr++){}
	*value_back = XIo_In32(BRAM_BASE_ADDR + CR);
	invalue = *value_back & (0xFFFF^0x100); XIo_Out32(BRAM_BASE_ADDR+CR,invalue);
	return 0;}

else if ( strcmp(cmd,"PAT:SYNC:ON")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + CR);
	invalue = *value_back | 0x1; XIo_Out32(BRAM_BASE_ADDR+CR,invalue);
	return 0;}
else if ( strcmp(cmd,"PAT:SYNC:OF")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + CR);
	invalue = *value_back & (0xFFFF^0x1); XIo_Out32(BRAM_BASE_ADDR+CR,invalue);
	return 0;}
else if ( strcmp(cmd,"PAT:DESK:ON")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + CR);
	invalue = *value_back | 0x3; XIo_Out32(BRAM_BASE_ADDR+CR,invalue);
	return 0;}
else if ( strcmp(cmd,"PAT:DESK:OF")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + CR);
	invalue = *value_back & (0xFFFF^0x3); XIo_Out32(BRAM_BASE_ADDR+CR,invalue);
	return 0;}

else if ( strcmp(cmd,"AFE:CAL:ALL:OF")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AFE_CTRL);
	invalue = *value_back | 0xF; XIo_Out32(BRAM_BASE_ADDR+AFE_CTRL,invalue);
	return 0;}
else if ( strcmp(cmd,"AFE:CAL:ALL:ON")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AFE_CTRL);
	invalue = *value_back & (0xFFFF^0xF); XIo_Out32(BRAM_BASE_ADDR+AFE_CTRL,invalue);
	return 0;}
else if ( strcmp(cmd,"AFE:PICK:ALL:OF")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AFE_CTRL);
	invalue = *value_back | 0xF0; XIo_Out32(BRAM_BASE_ADDR+AFE_CTRL,invalue);
	return 0;}
else if ( strcmp(cmd,"AFE:PICK:ALL:ON")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AFE_CTRL);
	invalue = *value_back & (0xFFFF^0xF0); XIo_Out32(BRAM_BASE_ADDR+AFE_CTRL,invalue);
	return 0;}
else if ( strcmp(cmd,"AFE:REC:ALL:OF")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AFE_CTRL);
	invalue = *value_back | 0xF00; XIo_Out32(BRAM_BASE_ADDR+AFE_CTRL,invalue);
	return 0;}
else if ( strcmp(cmd,"AFE:REC:ALL:ON")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AFE_CTRL);
	invalue = *value_back & (0xFFFF^0xF00); XIo_Out32(BRAM_BASE_ADDR+AFE_CTRL,invalue);
	return 0;}
else if ( strcmp(cmd,"AFE:GAIN:HIGH")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AFE_CTRL);
	invalue = *value_back | 0x8000; XIo_Out32(BRAM_BASE_ADDR+AFE_CTRL,invalue);
	return 0;}
else if ( strcmp(cmd,"AFE:GAIN:LOW")== 0 ){*value_back = XIo_In32(BRAM_BASE_ADDR + AFE_CTRL);
	invalue = *value_back & (0xFFFF^0x8000); XIo_Out32(BRAM_BASE_ADDR+AFE_CTRL,invalue);
	return 0;}

else if ( strcmp(cmd,"FL:BUF:PORT")== 0 ){Flash_Data_Ptr->IPport[0] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:PORT?")== 0 ){ *value_back = Flash_Data_Ptr->IPport[0]; return 1;}
else if ( strcmp(cmd,"FL:BUF:IP:0")== 0 ){Flash_Data_Ptr->boardIP[0] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:IP:0?")== 0 ){ *value_back = Flash_Data_Ptr->boardIP[0]; return 1;}
else if ( strcmp(cmd,"FL:BUF:IP:1")== 0 ){Flash_Data_Ptr->boardIP[1] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:IP:1?")== 0 ){ *value_back = Flash_Data_Ptr->boardIP[1]; return 1;}
else if ( strcmp(cmd,"FL:BUF:IP:2")== 0 ){Flash_Data_Ptr->boardIP[2] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:IP:2?")== 0 ){ *value_back = Flash_Data_Ptr->boardIP[2]; return 1;}
else if ( strcmp(cmd,"FL:BUF:IP:3")== 0 ){Flash_Data_Ptr->boardIP[3] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:IP:3?")== 0 ){ *value_back = Flash_Data_Ptr->boardIP[3]; return 1;}
else if ( strcmp(cmd,"FL:BUF:IP:MASK:0")== 0 ){Flash_Data_Ptr->boardIP_Mask[0] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:IP:MASK:0?")== 0 ){ *value_back = Flash_Data_Ptr->boardIP_Mask[0]; return 1;}
else if ( strcmp(cmd,"FL:BUF:IP:MASK:1")== 0 ){Flash_Data_Ptr->boardIP_Mask[1] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:IP:MASK:1?")== 0 ){ *value_back = Flash_Data_Ptr->boardIP_Mask[1]; return 1;}
else if ( strcmp(cmd,"FL:BUF:IP:MASK:2")== 0 ){Flash_Data_Ptr->boardIP_Mask[2] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:IP:MASK:2?")== 0 ){ *value_back = Flash_Data_Ptr->boardIP_Mask[2]; return 1;}
else if ( strcmp(cmd,"FL:BUF:IP:MASK:3")== 0 ){Flash_Data_Ptr->boardIP_Mask[3] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:IP:MASK:3?")== 0 ){ *value_back = Flash_Data_Ptr->boardIP_Mask[3]; return 1;}
else if ( strcmp(cmd,"FL:BUF:GATEW:0")== 0 ){Flash_Data_Ptr->boardGW[0] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:GATEW:0?")== 0 ){ *value_back = Flash_Data_Ptr->boardGW[0]; return 1;}
else if ( strcmp(cmd,"FL:BUF:GATEW:1")== 0 ){Flash_Data_Ptr->boardGW[1] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:GATEW:1?")== 0 ){ *value_back = Flash_Data_Ptr->boardGW[1]; return 1;}
else if ( strcmp(cmd,"FL:BUF:GATEW:2")== 0 ){Flash_Data_Ptr->boardGW[2] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:GATEW:2?")== 0 ){ *value_back = Flash_Data_Ptr->boardGW[2]; return 1;}
else if ( strcmp(cmd,"FL:BUF:GATEW:3")== 0 ){Flash_Data_Ptr->boardGW[3] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:GATEW:3?")== 0 ){ *value_back = Flash_Data_Ptr->boardGW[3]; return 1;}
else if ( strcmp(cmd,"FL:BUF:MAC:0")== 0 ){Flash_Data_Ptr->boardMac[0] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:MAC:0?")== 0 ){ *value_back = Flash_Data_Ptr->boardMac[0]; return 1;}
else if ( strcmp(cmd,"FL:BUF:MAC:1")== 0 ){Flash_Data_Ptr->boardMac[1] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:MAC:1?")== 0 ){ *value_back = Flash_Data_Ptr->boardMac[1]; return 1;}
else if ( strcmp(cmd,"FL:BUF:MAC:2")== 0 ){Flash_Data_Ptr->boardMac[2] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:MAC:2?")== 0 ){ *value_back = Flash_Data_Ptr->boardMac[2]; return 1;}
else if ( strcmp(cmd,"FL:BUF:MAC:3")== 0 ){Flash_Data_Ptr->boardMac[3] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:MAC:3?")== 0 ){ *value_back = Flash_Data_Ptr->boardMac[3]; return 1;}
else if ( strcmp(cmd,"FL:BUF:MAC:4")== 0 ){Flash_Data_Ptr->boardMac[4] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:MAC:4?")== 0 ){ *value_back = Flash_Data_Ptr->boardMac[4]; return 1;}
else if ( strcmp(cmd,"FL:BUF:MAC:5")== 0 ){Flash_Data_Ptr->boardMac[5] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:MAC:5?")== 0 ){ *value_back = Flash_Data_Ptr->boardMac[5]; return 1;}

else if ( strcmp(cmd,"FL:BUF:AFE:CTRL")== 0 ){Flash_Data_Ptr->boardAFE = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:AFE:CTRL?")== 0 ){ *value_back = Flash_Data_Ptr->boardAFE; return 1;}

/*
else if ( strcmp(cmd,"FL:BUF:NF:DOS")== 0 ){Flash_Data_Ptr->boardNFDOS = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:NF:DOS?")== 0 ){ *value_back = Flash_Data_Ptr->boardNFDOS; return 1;}
else if ( strcmp(cmd,"FL:BUF:NF:SUM")== 0 ){Flash_Data_Ptr->boardNFSUM = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:NF:SUM?")== 0 ){ *value_back = Flash_Data_Ptr->boardNFSUM; return 1;}
else if ( strcmp(cmd,"FL:BUF:THRSH")== 0 ){Flash_Data_Ptr->boardTHRESH = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:THRSH?")== 0 ){ *value_back = Flash_Data_Ptr->boardTHRESH; return 1;}
else if ( strcmp(cmd,"FL:BUF:OFFS:A")== 0 ){Flash_Data_Ptr->boardADC_OFFST[0] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:OFFS:A?")== 0 ){ *value_back = Flash_Data_Ptr->boardADC_OFFST[0]; return 1;}
else if ( strcmp(cmd,"FL:BUF:OFFS:B")== 0 ){Flash_Data_Ptr->boardADC_OFFST[1] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:OFFS:B?")== 0 ){ *value_back = Flash_Data_Ptr->boardADC_OFFST[1]; return 1;}
else if ( strcmp(cmd,"FL:BUF:OFFS:C")== 0 ){Flash_Data_Ptr->boardADC_OFFST[2] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:OFFS:C?")== 0 ){ *value_back = Flash_Data_Ptr->boardADC_OFFST[2]; return 1;}
else if ( strcmp(cmd,"FL:BUF:OFFS:D")== 0 ){Flash_Data_Ptr->boardADC_OFFST[3] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:OFFS:D?")== 0 ){ *value_back = Flash_Data_Ptr->boardADC_OFFST[3]; return 1;}
else if ( strcmp(cmd,"FL:BUF:GAIN:A")== 0 ){Flash_Data_Ptr->boardADC_GAIN[0] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:GAIN:A?")== 0 ){ *value_back = Flash_Data_Ptr->boardADC_GAIN[0]; return 1;}
else if ( strcmp(cmd,"FL:BUF:GAIN:B")== 0 ){Flash_Data_Ptr->boardADC_GAIN[1] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:GAIN:B?")== 0 ){ *value_back = Flash_Data_Ptr->boardADC_GAIN[1]; return 1;}
else if ( strcmp(cmd,"FL:BUF:GAIN:C")== 0 ){Flash_Data_Ptr->boardADC_GAIN[2] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:GAIN:C?")== 0 ){ *value_back = Flash_Data_Ptr->boardADC_GAIN[2]; return 1;}
else if ( strcmp(cmd,"FL:BUF:GAIN:D")== 0 ){Flash_Data_Ptr->boardADC_GAIN[3] = invalue; return 0;}
else if ( strcmp(cmd,"FL:BUF:GAIN:D?")== 0 ){ *value_back = Flash_Data_Ptr->boardADC_GAIN[3]; return 1;}
//else if ( strcmp(cmd,"FL:BUF:*IDN")== 0 ){Flash_Data_Ptr->boardIDN = invalue; return 0;}
 */

else if ( strcmp(cmd,"FL:BUF:*IDN?")== 0 ){	*string_back = Flash_Data_Ptr->boardIDN;return 2;}

else if ( strcmp(cmd,"FLASH:WRIT")== 0 )
		{ if (span_flash_write(&FlashInstance) == XST_SUCCESS)
			  xil_printf("Flash Write sucessful. \n\r");
		  else  xil_printf("Flash Write failed. \n\r");
		  return 0;
		}

else if ( strcmp(cmd,"FLASH:READ")== 0 )
		{ if (span_flash_read(&FlashInstance) == XST_SUCCESS)
			  xil_printf("Flash Read sucessful.\n\r");
		  else  xil_printf("Flash Read failed. \n\r");
		  return 0;
		}

else if ( strcmp(cmd,"FLASHBUF:TO:FPGA")== 0 ){flash_buffer_2_fpga( );return 0;}
else if ( strcmp(cmd,"FPGA:TO:FLASHBUF")== 0 ){fpga_2_flash_buffer( );return 0;}

else xil_printf("\n\r Error: Bad command %s \n\r", cmd);
}
	   return 0;

}
