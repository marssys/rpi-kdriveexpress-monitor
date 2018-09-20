#include <stdio.h>
#include <kdrive_express.h>

/*******************************
** Private Functions
********************************/

/*!
	Telegram Callback Handler
*/
static void on_telegram(const uint8_t* telegram, uint32_t telegram_len, void* user_data);

/*!
	Called when an error occurs
*/
static void error_callback(error_t e, void* user_data);

/*!
	Called when an event occurs
*/
static void event_callback(int32_t ap, uint32_t e, void* user_data);

/*******************************
** Main
********************************/

int main(int argc, char* argv[])
{
//	uint16_t address = 0x901;
//	uint32_t index = 0;
	uint32_t key = 0;
//	uint8_t value = 127;
	int32_t ap = 0;

	/*
		Configure the logging level and console logger
	*/
	kdrive_logger_set_level(KDRIVE_LOGGER_INFORMATION);
	kdrive_logger_console();

    /*
		We register an error callback as a convenience logger function to
		print out the error message when an error occurs.
	*/
	kdrive_register_error_callback(&error_callback, NULL);

    /*
		We create a Access Port descriptor. This descriptor is then used for
		all calls to that specific access port.
	*/
	ap = kdrive_ap_create();

    /*
		We check that we were able to allocate a new descriptor
		This should always happen, unless a bad_alloc exception is internally thrown
		which means the memory couldn't be allocated.
	*/
	if (ap == KDRIVE_INVALID_DESCRIPTOR)
	{
		printf("Unable to create access port. This is a terminal failure\n");
		while (1)
		{
			;
		}
	}

    /*
		We register an event callback to notify of the Access Port Events
		For example: KDRIVE_EVENT_TERMINATED
	*/
	kdrive_set_event_callback(ap, &event_callback, NULL);

    /*
		Open a connection to a KNX FT1.2 serial interface
	*/
	if (kdrive_ap_open_serial_ft12(ap, "/dev/ttyAMA0") == KDRIVE_ERROR_NONE)
	{
		/*
			Connect the Packet Trace logging mechanism
			to see the Rx and Tx packets
		*/
		kdrive_ap_packet_trace_connect(ap);

		/* now we simply go into bus monitor mode, and display received telegrams */
		kdrive_ap_register_telegram_callback(ap, &on_telegram, NULL, &key);

		kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Entering BusMonitor Mode");
		kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Press [Enter] to exit the application ...");
		getchar();

		/* close the access port */
		kdrive_ap_close(ap);
	}

	/* releases the access port */
	kdrive_ap_release(ap);

	return 0;
}

/*******************************
** Private Functions
********************************/

/*!
	When a telegram is received we check to see if it is a L_Data.ind group value write
	telegram. If it is, we get the destination address and the datapoint value
*/
void on_telegram(const uint8_t* telegram, uint32_t telegram_len, void* user_data)
{
	static uint8_t data[KDRIVE_MAX_GROUP_VALUE_LEN];
	uint32_t data_len = KDRIVE_MAX_GROUP_VALUE_LEN;
	uint16_t address = 0;
	uint8_t message_code = 0;

	kdrive_ap_get_message_code(telegram, telegram_len, &message_code);

	if ((message_code == KDRIVE_CEMI_L_DATA_IND) &&
	    kdrive_ap_is_group_write(telegram, telegram_len) &&
	    (kdrive_ap_get_dest(telegram, telegram_len, &address) == KDRIVE_ERROR_NONE) &&
	    (kdrive_ap_get_group_data(telegram, telegram_len, data, &data_len) == KDRIVE_ERROR_NONE))
	{
		kdrive_logger_ex(KDRIVE_LOGGER_INFORMATION, "A_GroupValue_Write: 0x%04x ", address);
		kdrive_logger_dump(KDRIVE_LOGGER_INFORMATION, "A_GroupValue_Write Data :", data, data_len);
	}
}

/*!
	Called when a kdrive error exception is raised.
	The handling in the error callback is typically
	application specific. And here we simply show
	the error message.
*/
void error_callback(error_t e, void* user_data)
{
	if (e != KDRIVE_TIMEOUT_ERROR)
	{
		static char error_message[ERROR_MESSAGE_LEN];
		kdrive_get_error_message(e, error_message, ERROR_MESSAGE_LEN);
		kdrive_logger_ex(KDRIVE_LOGGER_ERROR, "kdrive error: %s", error_message);
	}
}

/*!
	The event callback is called when an Access Port event is raised
*/
void event_callback(int32_t ap, uint32_t e, void* user_data)
{
	switch (e)
	{
		case KDRIVE_EVENT_ERROR:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Access Port Error");
			break;

		case KDRIVE_EVENT_OPENING:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Access Port Opening");
			break;

		case KDRIVE_EVENT_OPENED:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Access Port Opened");
			break;

		case KDRIVE_EVENT_CLOSED:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Access Port Closed");
			break;

		case KDRIVE_EVENT_CLOSING:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Access Port Closing");
			break;

		case KDRIVE_EVENT_TERMINATED:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Access Port Terminated");
			break;

		case KDRIVE_EVENT_KNX_BUS_CONNECTED:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "KNX Bus Connected");
			break;

		case KDRIVE_EVENT_KNX_BUS_DISCONNECTED:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "KNX Bus Disconnected");
			break;

		case KDRIVE_EVENT_LOCAL_DEVICE_RESET:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Local Device Reset");
			break;

		case KDRIVE_EVENT_TELEGRAM_INDICATION:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Telegram Indication");
			break;

		case KDRIVE_EVENT_TELEGRAM_CONFIRM:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Telegram Confirm");
			break;

		case KDRIVE_EVENT_TELEGRAM_CONFIRM_TIMEOUT:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Telegram Confirm Timeout");
			break;

		case KDRIVE_EVENT_INTERNAL_01:
			break;

		default:
			kdrive_logger(KDRIVE_LOGGER_INFORMATION, "Unknown kdrive event");
			break;
	}
}