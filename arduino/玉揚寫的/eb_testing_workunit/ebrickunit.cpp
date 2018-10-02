#include "ebrickunit.h"

#include <EEPROM.h>
#include <Ethernet.h>

#include "bytes_util.h"
#include "clock.h"
#include "eeprom_map.h"
#include "eeprom_util.h"
#include "eeprom_default.h"
#include "debug_config.h"

EBrickUnit EBUnit;

void EBrickUnit::checkeeprom()
{
	uint8_t wipe = EEPROM.read(EEPADDR_WIPE_ALL);
	if (wipe != 0) {
		debug_println_1("start eeprom wipe");
		uint8_t mac[] = EEPROM_DEFAULT_MAC;
		eeprom_write_arr(EEPADDR_MAC, mac, EEPADDR_MAC_SIZE);
		uint8_t server_ip[] = EEPROM_DEFAULT_SERVER_IP;
		eeprom_write_arr(EEPADDR_SERVER_ADDRESS, server_ip, EEPADDR_SERVER_ADDRESS_SIZE);
		uint16_t server_port = EEPROM_DEFAULT_SERVER_PORT;
		eeprom_write_uint16_t(EEPADDR_SERVER_PORT, server_port);
		debug_println_1("end eeprom wipe");
	}
}

void EBrickUnit::beginEthernet()
{
	debug_println_1("Begin Ethernet");

	uint8_t mac[6];
	this->getMACAddress(mac);

	int res = Ethernet.begin(mac);

	if (res) {
		debug_println_1("Ethernet init success");
		debug_println_2("IP = ", Ethernet.localIP());

		status_ethernet = 1;
	}
	else {
		debug_println_1("Ethernet init failed");
		status_ethernet = 0;
		// todo Ethernet begin failed
	}
}

void EBrickUnit::beginClient(bool report)
{
	/*
	if (!client) {
		if (report) {
			debug_println_1("EthernetClient not ready");
		}
		return;
	}
	*/
	if (client.connected()) {
		return;
	}

	uint8_t ip[4];
	uint16_t port = 0;
	eeprom_read_arr(EEPADDR_SERVER_ADDRESS, ip, EEPADDR_SERVER_ADDRESS_SIZE);
	eeprom_read_uint16_t(EEPADDR_SERVER_PORT, port);
	IPAddress server_ip(ip);
	
	if (report) {
		debug_println_4("Begin connection to server ", server_ip, ":", port);
	}

	status_client = client.connect(server_ip, port);

	if (status_client) {
		debug_println_1("Connected to server ");
	}
	else {
		if (report) {
			debug_println_1("Connect to server failed");
		}
		// todo connect begin failed
	}
}

void EBrickUnit::checkEthernet()
{
	if (!status_ethernet) {
		int res = Ethernet.maintain();

		bool report = Clock.pass(report_time_ethernet, 1000000L);

		switch (res)
		{
		case 0: // nothing happened
			break;
		case 2: // renew success
			if (report) {
				debug_println_1("Ethernet renew success");
			}
			status_ethernet = 1;
			break;
		case 4: // rebind success
			if (report) {
				debug_println_1("Ethernet rebind success");
			}
			status_ethernet = 1;
			break;
		case 1: // renew failed
			if (report) {
				debug_println_1("Ethernet renew failed");
			}
			status_ethernet = 0;
			break;
		case 3: // rebind fail
			if (report) {
				debug_println_1("Ethernet rebind failed");
			}
			status_ethernet = 0;
			break;
		default:
			break;
		}
	}
}

void EBrickUnit::checkClient()
{
	bool report = Clock.pass(report_time_client, 1000000L);
	if (!client) {
		if (report) {
			debug_println_1("Client not ready");
		}
	}
	status_client = client.connected();
	if (!status_client) {
		if (report) {
			debug_println_1("Client not connected");
		}
		this->beginClient(report);
	}
}

void EBrickUnit::readandprocess()
{
	bool complete = read(client);
}

bool EBrickUnit::read(Stream & in)
{
	int buffered = in.available();
	if (buffered > 0) {
		for (int i = 0; i < buffered; i++) {
			char c = in.read();

			switch (com_state)
			{
			case COM_STATE_GET_HEAD:
				if (c == (int8_t)0xAA) {
					head_read_len++;
					if (head_read_len == 2) {
						com_state = COM_STATE_GET_LEN;
						head_read_len = 0;
						debug_println_1("0xAA 0xAA get");
					}
				}
				else {
					head_read_len = 0;
				}
				break;
			case COM_STATE_GET_LEN:
				len_buf.write(c);
				if (len_buf.pos == 2) {
					len_buf.pack();
					payload_len = len_buf.getUint16();
					len_buf.reset();
					com_state = COM_STATE_GET_PAYLOAD;

					debug_println_2("payload_len ", payload_len);
				}
				break;
			case COM_STATE_GET_PAYLOAD:
				if (payload_read < payload_len) {
					payload_buf.write(c);
					payload_read++;
				}
				if (payload_read == payload_len) {
					debug_println_1("payload read");
					payload_buf.write((char)0);
					com_state = COM_STATE_GET_CHECKSUM;
					payload_read = 0;

					uint16_t calc_checksum = checksum(payload_buf.buf, payload_buf.pos);
					debug_println_2("calc_checksum", calc_checksum);
				}
				break;
			case COM_STATE_GET_CHECKSUM:
				checksum_buf.write(c);
				if (checksum_buf.pos == 2) {
					checksum_buf.pack();
					payload_checksum = checksum_buf.getUint16();
					checksum_buf.reset();
					uint16_t calc_checksum = checksum(payload_buf.buf, payload_buf.pos);
					debug_println_2("read_checksum", payload_checksum);
					bool check = (calc_checksum == payload_checksum);

					if (check) {
						this->processCommand();

						payload_buf.reset();
					}
					com_state = COM_STATE_GET_HEAD;
				}
				break;
			}
		}
	}
}

void debug_print_json(JsonObject& root) {
	char jsonstr[256];
	root.printTo(jsonstr,256);
	Serial.print(";");
	Serial.println(jsonstr);
	debug_println();
}

void EBrickUnit::processCommand()
{
	JsonObject& root = jsonBuffer.parseObject(payload_buf.buf);

#if MDEBUG
	debug_print_json(root);
#endif

	jsonahandlerfunc(root);
}

EBrickUnit::EBrickUnit() : len_buf(len_buf_arr, 2), checksum_buf(checksum_buf_arr, 2), payload_buf(payload_buf_arr, 256)
{
}

void EBrickUnit::begin()
{
	this->checkeeprom();
	this->beginEthernet();
	this->beginClient(true);
}

void EBrickUnit::handle()
{
	this->checkEthernet();
	this->checkClient();

	if (status_client) {
		this->read(client);
	}
}

void EBrickUnit::getMACAddress(uint8_t* buf)
{
	eeprom_read_arr(EEPADDR_MAC, buf, EEPADDR_MAC_SIZE);
}

EthernetClient * EBrickUnit::connection()
{
	return &client;
}

void EBrickUnit::handleJson(jsonhandler h)
{
	this->jsonahandlerfunc = h;
}
