#ifndef _EB_STATION_h
#define _EB_STATION_h

#define COM_STATE_GET_HEAD 0
#define COM_STATE_GET_LEN 1
#define COM_STATE_GET_PAYLOAD 2
#define COM_STATE_GET_CHECKSUM 3

#include "arduino.h"
#include "charbuf.h"

#include <ArduinoJson.h>
#include <EthernetClient.h>

typedef void(*jsonhandler)(JsonObject& root);

class EBrickUnit
{
private:
	int32_t report_time_ethernet = 0;
	int32_t report_time_client = 0;
protected:
	EthernetClient client;

	uint8_t status_ethernet = 0;
	uint8_t status_client = 0;

	uint8_t com_state = COM_STATE_GET_HEAD;

	char len_buf_arr[2];
	char checksum_buf_arr[2];
	char payload_buf_arr[256];

	CharBuffer len_buf;// = CharBuffer(len_buf_arr, 2);
	CharBuffer checksum_buf;// = CharBuffer(checksum_buf_arr, 2);
	CharBuffer payload_buf;// = CharBuffer(payload_buf_arr, 255);

	uint8_t head_read_len = 0;
	uint16_t payload_len = 0;
	uint16_t payload_read = 0;
	uint16_t payload_checksum = 0;

	StaticJsonBuffer<256> jsonBuffer;

	void checkeeprom();
	void beginEthernet();
	void beginClient(bool report = false);
	void checkEthernet();
	void checkClient();
	void readandprocess();
	bool read(Stream & in);
	void processCommand();

	jsonhandler jsonahandlerfunc;
public:
	EBrickUnit();
	/*!
	* 啟動工作站
	*/
	void begin();
	/*!
	* 處理工作站事件
	*/
	void handle();
	/*!
	* 取得 MAC Address
	* @param [out] buf 
	*/
	void getMACAddress(uint8_t* buf);
	/*!
	* 取得連線客戶端
	*/
	EthernetClient* connection();
	/*!
	* 設定事件處理
	* @param [in] handler
	*/
	void handleJson(jsonhandler handler);
};

extern EBrickUnit EBUnit;

#endif

