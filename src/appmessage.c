#include <pebble.h>
#include "appmessage.h"
#include "common.h"
//include "windows/storylist.h"

static void in_dropped_handler(AppMessageResult reason, void *context);
static void out_sent_handler(DictionaryIterator *sent, void *context);
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);
	
static void in_dropped_handler(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Incoming AppMessage from Pebble dropped, %d", reason);
}

static void out_sent_handler(DictionaryIterator *sent, void *context) {
	// outgoing message was delivered
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Failed to send AppMessage to Pebble");
}

void appmessage_init(void) {
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);
	app_message_open(512 /* inbound_size */, 32 /* outbound_size */);
}


void request_data(uint16_t endpoint) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "request_data: %u", endpoint);
	
	Tuplet endpoint_tuple = TupletInteger(KEY_ENDPOINT, endpoint);

	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	if (iter == NULL) {
		return;
	}

	dict_write_tuplet(iter, &endpoint_tuple);
	dict_write_end(iter);

	app_message_outbox_send();
}