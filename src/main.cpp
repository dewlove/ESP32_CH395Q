 //本机ip:192.168.1.110
 //本机端口号:7000
 //esp32 ip:192.168.1.100
 //esp32 端口号:8000
	
#include <Arduino.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// #include <Ethernet.h> 
// #include <EthernetUdp.h>
#include <SPI.h>


#include "CH395SPI.H" 
#include "CH395INC.H" //通用文件
#include "CH395CMD.H" //通用文件，调用SPI接口文件进行通讯
/*提示:(只是提示!这节使用的Socket0通信,并没有人为分配缓存区)
芯片共有48块缓存区,每个缓存区512字节
芯片共有8个Socket,默认把48块缓存区分给了Socket0,Socket1,Socket2,Socket3
这四个Socket,每个 Socket 使用8块缓存区作为接收,4块缓存区作为发送,
即Socket0,Socket1,Socket2,Socket3的接收区各为512*8 = 4KB
即Socket0,Socket1,Socket2,Socket3的发送区各为512*4 = 2KB
如果要使用Socket4,Socket5,Socket6,Socket7需要重新分配缓存区
*/

/*存储网络接收的数据*/
#define recv_buff_len 1500
unsigned char recv_buff[recv_buff_len];

/*UDP 自动发送间隔次数*/
int UDP_Auto_send = 0;


char ch395_version = 0;							//获取版本号
uint8_t ch395_check = 0;							//测试命令
uint8_t ch395_init = 0;							//初始化
uint8_t ch395_mac[] = {0,0,0,0,0,0};				//mac地址
uint16_t ch395_CMDstatus = 0;							
uint8_t ch395_IP[] = {192,168,1,100};				//CH395的IP地址
uint8_t ch395_MASK[] = {255,255,255,0};				//CH395的子网掩码
uint8_t ch395_GWIP[] = {192,168,1,1};				//CH395的网关IP地址

unsigned char buf[20];
int ch395_status=0;							//获取中断事件

/* socket 相关定义*/
uint8_t  SocketIndex = 0; 						/* Socket 索引(0,1,2,3,4,5,6,7) */
// uint8_t  SocketDesIP[4] = {192,168,1,110}; 		/* Socket 目的IP地址 */
uint16_t SocketDesPort   = 7000;           		/* Socket 目的端口 */
uint8_t  SocketDesIP[4] = {255,255,255,255}; 	/* Socket 目的IP地址:UDP广播 */
uint16_t SocketSourPort  = 8000;           		/* Socket 本地端口 */

UINT8  SocketMultiIP[4] = {224,1,1,1}; 			 /* 多播的地址 */
UINT8  SocketMultiMac[6]  = {1,0,0x5e,3,4,5};/* 对应多播Mac地址 */

// #define AXT_SPI1_VSPI_MISO_PIN 19
// #define AXT_SPI1_VSPI_MOSI_PIN 23
// #define AXT_SPI1_VSPI_CS_PIN   5
// #define AXT_SPI1_VSPI_SCLK_PIN 18
// #define AXT_SPI1_VSPI_TX_PIN   4
// #define AXT_SPI1_VSPI_RST_PIN   25
// #define AXT_SPI1_VSPI_INT_PIN	26

//@1-本地网络1硬件MAC地址
byte mac1[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
  // 0x00, 0xE0, 0x4C, 0xA0, 0x7E, 0x61
};

/**
* @brief   初始化socket
* @param   sockindex  Socket索引(0,1,2,3,4,5,6,7)
* @param   ipaddr  目的地址
* @param   desprot 目的端口号
* @param   surprot 本地端口号
* @retval  0:初始化成功; others:初始化失败
* @warning None
* @example 
**/
char ch395_socket_udp_init(uint8_t sockindex,uint8_t *ipaddr,uint16_t desprot,uint16_t surprot)
{
    CH395SetSocketDesIP(sockindex,ipaddr);         		/* 目的地址 */         
    CH395SetSocketProtType(sockindex,PROTO_TYPE_UDP); /* 协议类型 */
    CH395SetSocketDesPort(sockindex,desprot);     		/* 目的端口号 */
    CH395SetSocketSourPort(sockindex,surprot);  			/* 本地端口号 */
	if(CH395OpenSocket(sockindex) !=0)                /* 打开Socket */
	{
		return 1;
	}
	return 0;
}


/**
* @brief   socket处理函数(把此函数放到全局socket中断里面)
* @param   sockindex  Socket索引(0,1,2,3,4,5,6,7)
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void ch395_socket_udp_interrupt(UINT8 sockindex)
{
	UINT8  sock_int_socket;
	UINT16 len;
	UINT16 tmp,port;
	
	/* 获取socket 的中断状态 */
	sock_int_socket = CH395GetSocketInt(sockindex);
	
	/* 发送缓冲区空闲，可以继续写入要发送的数据 */
	if(sock_int_socket & SINT_STAT_SENBUF_FREE)
	{
	}
	
	/* 发送完成中断 */
	if(sock_int_socket & SINT_STAT_SEND_OK)                           
	{
	}
	
	/* 接收数据中断 */
	if(sock_int_socket & SINT_STAT_RECV)                              
	{
		len = CH395GetRecvLength(sockindex);/* 获取当前缓冲区内数据长度 */
		printf("\r\nreceive len = %d\r\n",len);
		if(len == 0)return;
		if(len > recv_buff_len)len = recv_buff_len;
		CH395GetRecvData(sockindex,len,recv_buff);/* 读取数据 */

		/*打印Socket信息*/
		//0,1存储接收的数据个数
		tmp = ((UINT16)recv_buff[1] << 8) + recv_buff[0];
		//2,3存储Socket端口号
		port = ((UINT16)recv_buff[3] << 8) + recv_buff[2];
		printf("Data Len = %d\r\n",tmp);
		printf("Port = %d\r\n",port);
		//4,5,6,7存储Socket IP地址
		printf("IP = %2d.%2d.%2d.%2d\r\n",(UINT16)recv_buff[4],(UINT16)recv_buff[5],(UINT16)recv_buff[6],(UINT16)recv_buff[7]);      

		//从8开始存储真实数据
		CH395UDPSendTo(&recv_buff[8], (len -8),&recv_buff[4], port, sockindex);//返回数据
				

// 		/*把接收的数据返回给UDP,使用串口打印接收的数据*/
// 		/*----------------------------------------*/
// 		/*把接收的数据返回给UDP*/
// 		CH395SendData(sockindex,recv_buff,len);
		
// 		//也可以使用下面的函数发送数据
// //		CH395UDPSendTo(recv_buff, len,SocketDesIP, SocketDesPort, sockindex);
// 		/*----------------------------------------*/

		/*使用串口打印接收的数据*/
		for(int i = 8; i < len; i++)
		{
			Serial.print((char)recv_buff[i]);
		}
		Serial.print("\r\n \r\n");

	}
}

void setup()
{
	uint8_t MacCrc32;
	uint32_t Hash0 = 0, Hash1 = 0;

	Serial.begin(115200);	 //串口初始化为115200

	//初始化CH395使用的GPIO
	CH395_PORT_INIT();
	//复位 CH395
	CH395_RST();
    //初始化SPI
	SPI.begin();
	// //@-网络接口1初始化-DSP网络
	// Ethernet.init(AXT_SPI1_VSPI_CS_PIN);
	// //@-绑定自身IP和MAC
	// Ethernet.begin(mac1, local_ip1);  
	// Net_Check_LinkStatus1();
	// //@-配置AXT网络
	// Net_Axt_Setup1();



	// /*获取芯片版本*/
	// while((ch395_version = CH395CMDGetVer()) < 0x40)
	// {
	// 	printf("CH395CMDGetVer ERR\r\n");
    //     delay(100);
	// }
	// Serial.println(ch395_version);
	// Serial.println("__________________________");

	// /*测试命令，按位取反返回说明测试通过*/
	// while((ch395_check = CH395CMDCheckExist(0x55)) != 0xaa)
	// {
	// 	printf("\r\nCH395CMDCheck ERR\r\n");
    //     delay(100);
	// }
	// Serial.println(ch395_check);
	// Serial.println("__________________________");
	
	// /*获取模块的MAC地址:返回六个数据*/
	// CH395CMDGetMACAddr(ch395_mac);
	// // Serial.print(sizeof(ch395_mac));
	// for(int i = 0; i < (sizeof(ch395_mac)); i++)
	// {
	// 	Serial.println(ch395_mac[i]);
	// }
	// // Serial.println(ch395_mac);
	// Serial.println("__________________________");

	/* 设置模块的MAC地址 */
	// 一般不需要，CH395 在出厂时，已经烧录了 IEEE 分配的 MAC 地址

	/* 设置 CH395 的 IP 地址 */
	// 如果需要启动 DHCP 或者 PPPOE，则不需要这个步骤
	CH395CMDSetIPAddr(ch395_IP);

	/* 设置 CH395 的网关 IP 地址 */
	// 如果需要启动 DHCP 或者 PPPOE，则不需要这个步骤
	CH395CMDSetGWIPAddr(ch395_GWIP);

	/* 设置 CH395 的子网掩码 */
	// 如果需要启动 DHCP 或者 PPPOE，则不需要这个步骤
	// 默认为 255.255.255.0，可以不设置
	CH395CMDSetMASKAddr(ch395_MASK);

	/*初始化模块:成功返回 0 */
	while((ch395_init = CH395CMDInitCH395()) != 0)
	{
		printf("\r\nCH395CMDInitCH395 ERR\r\n");
		delay(100);
	}
	Serial.println(ch395_init);
	Serial.println("__________________________");

	/* 延时 2MS 以上发送命令 CMD_GET_CMD_STATUS 获取 CMD_INIT_CH395 执行状态，如果返回
	CH395_ERR_BUSY 表示 CH395 内部正在执行命令，需要再次执行⑥；如果返回 CH395_ERR_SUCCESS
	表示命令执行成功。CMD_INIT_CH395 一般需要 350mS 才能执行完毕。*/
	delay(5);
	// ch395_status = CH395GetCmdStatus();
	// Serial.println(ch395_status);
	while((ch395_CMDstatus = CH395GetCmdStatus()) != 0x00)
	{
		printf("\r\nCH395CMDInitCH395: CH395GetCmdStatus ERR\r\n");
		delay(100);
	}

	/*初始化UDP*/
	while( ch395_socket_udp_init(SocketIndex,SocketDesIP,SocketDesPort,SocketSourPort) != 0)
	{
		printf("\r\nch395_socket_udp_init ERR\r\n");
		delay(100);
	}

	delay(5);
	// ch395_status = CH395GetCmdStatus();
	// Serial.println(ch395_status);
	while((ch395_CMDstatus = CH395GetCmdStatus()) != 0x00)
	{
		printf("\r\nch395_socket_udp_init: CH395GetCmdStatus ERR\r\n");
		delay(100);
	}

	/*设置MAC过滤*/
	MacCrc32 =  CH395CRCRet6Bit(SocketMultiMac);
	printf("MacCrc32 : %2x\n",(UINT16)MacCrc32);
	if(MacCrc32 > 31)
	{
		MacCrc32 -=31;
		Hash1 |= ((UINT32)1<<MacCrc32);
	}
	else
	{
		Hash0 |= ((UINT32)1<<MacCrc32);
	}
	//0x1a:00011010 (发送使能,接收使能,接收所有数据)
	CH395CMDSetMACFilt(0x1a,Hash0,Hash1);
	delay(100);

	printf("\r\nstart\r\n");
}


void loop() 
{	

	/* 查询CH395是否连接 */
	if(CH395CMDGetPHYStatus() == PHY_DISCONN)
	{
		printf("CH395 DisConnect Ethernet\n");
	}
	else /* CH395芯片连接到以太网，此时会产生中断 */
	{   
		printf("CH395 Connect Ethernet\n");
	}
	delay(300);

	// 读取引脚电平
  	// int pinState = digitalRead(AXT_SPI1_VSPI_INT_PIN);

	// if(pinState == TRUE)
	// {
	// 	Serial.println("true");
	// }
	// else if(pinState == FALSE)
	// {
	// 	Serial.println("false");
	// }

	// // 打印引脚状态
	// Serial.print("Pin state: ");
	// Serial.println(pinState);

	// if(Query395Interrupt())
	// {
	// 	/*获取中断事件(不同的硬件版本获取函数不一样)*/
	// 	if(ch395_version>=0x44) 
	// 	{
	// 		ch395_status = CH395CMDGetGlobIntStatus_ALL();
	// 	}
	// 	else
	// 	{
	// 		ch395_status = CH395CMDGetGlobIntStatus();
	// 	}
		
	// 	/* 处理PHY改变中断*/
	// 	if(ch395_status & GINT_STAT_PHY_CHANGE)                           
	// 	{
	// 		if(CH395CMDGetPHYStatus() == PHY_DISCONN)//网线断开
	// 		{
	// 			printf("\r\nPHY_DISCONN\r\n");
	// 		}
	// 		else//网线连接
	// 		{
	// 			printf("\r\nPHY_CONNECTED\r\n");
	// 		}
	// 	}
	// }

	// IWDG_Feed();//喂狗

	/*每隔一段时间发送数据*/
	UDP_Auto_send ++;
	if(UDP_Auto_send>2)
	{
		UDP_Auto_send=0;
		// CH395SendData(SocketIndex,"11223344",8);
		CH395UDPCharSendTo("11223344",8,SocketDesIP, SocketDesPort, SocketIndex);
	}			
	
	//INT引脚产生低电平中断以后进去判断
	if(Query395Interrupt())
	{
		/*获取中断事件*/
		if(ch395_version>=0x44)
		{
			ch395_status = CH395CMDGetGlobIntStatus_ALL();
		}
		else
		{
			ch395_status = CH395CMDGetGlobIntStatus();
		}
		
		/* 处理PHY改变中断*/
		// if(ch395_status & GINT_STAT_PHY_CHANGE)                           
		// {
		// 	if(CH395CMDGetPHYStatus() == PHY_DISCONN)//网线断开
		// 	{
		// 		printf("\r\nPHY_DISCONN\r\n");
		// 	}
		// 	else//网线连接
		// 	{
		// 		printf("\r\nPHY_CONNECTED\r\n");
		// 		CH395DHCPEnable(1);//启动DHCP
		// 	}
		// }

		/* 处理DHCP/PPPOE中断 */
		// if(ch395_status & GINT_STAT_DHCP)
		// {
		// 	if(CH395GetDHCPStatus() == 0)//DHCP OK
		// 	{
		// 		CH395GetIPInf(buf);//获取IP,网关和子网掩码
		// 		printf("IP:%d.%d.%d.%d\r\n",buf[0],buf[1],buf[2],buf[3]);
		// 		printf("GWIP:%d.%d.%d.%d\r\n",buf[4],buf[5],buf[6],buf[7]);
		// 		printf("Mask:%d.%d.%d.%d\r\n",buf[8],buf[9],buf[10],buf[11]);
		// 		printf("DNS1:%d.%d.%d.%d\r\n",buf[12],buf[13],buf[14],buf[15]);
		// 		printf("DNS2:%d.%d.%d.%d\r\n",buf[16],buf[17],buf[18],buf[19]);
		// 	}
		// }

		// CH395GetIPInf(buf);//获取IP,网关和子网掩码
		// printf("IP:%d.%d.%d.%d\r\n",buf[0],buf[1],buf[2],buf[3]);
		// printf("GWIP:%d.%d.%d.%d\r\n",buf[4],buf[5],buf[6],buf[7]);
		// printf("Mask:%d.%d.%d.%d\r\n",buf[8],buf[9],buf[10],buf[11]);
		// printf("DNS1:%d.%d.%d.%d\r\n",buf[12],buf[13],buf[14],buf[15]);
		// printf("DNS2:%d.%d.%d.%d\r\n",buf[16],buf[17],buf[18],buf[19]);

		/* 处理不可达中断，读取不可达信息 */
		if(ch395_status & GINT_STAT_UNREACH){
			CH395CMDGetUnreachIPPT(buf);
		}
		
		/* 处理IP冲突中断，建议重新修改CH395的 IP，并初始化CH395*/
		if(ch395_status & GINT_STAT_IP_CONFLI){
			printf("\r\nGINT_STAT_IP_CONFLI\r\n");
		}
		/* 处理 SOCK0 中断 */
		if(ch395_status & GINT_STAT_SOCK0){
			ch395_socket_udp_interrupt(SocketIndex);
		}
		/* 处理 SOCK1 中断 */
		if(ch395_status & GINT_STAT_SOCK1){

		}
		/* 处理 SOCK2 中断 */
		if(ch395_status & GINT_STAT_SOCK2){

		}
		/* 处理 SOCK3 中断 */
		if(ch395_status & GINT_STAT_SOCK3){

		}
		
		
		if(ch395_version>=0x44)
		{
			/* 处理 SOCK4 中断 */
			if(ch395_status & GINT_STAT_SOCK4){

			}
			/* 处理 SOCK5 中断 */
			if(ch395_status & GINT_STAT_SOCK5){

			}
			/* 处理 SOCK6 中断 */
			if(ch395_status & GINT_STAT_SOCK6){

			}
			/* 处理 SOCK7 中断 */
			if(ch395_status & GINT_STAT_SOCK7){

			}
		}

	}
	delay(2000);
}



