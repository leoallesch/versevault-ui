#pragma once

#include <stdint.h>

#include "i_datastream.h"

enum {
  KEY_EVENT_NONE = 0,
  KEY_EVENT_PRESS,
  KEY_EVENT_RELEASE,
  KEY_EVENT_LONGPRESS,
  KEY_EVENT_PRESSANDRELEASE,
  KEY_EVENT_LONGPRESSANDRELEASE
};
typedef uint8_t key_event_t;

typedef struct {
  datastream_key_t key;
  key_event_t event;
  uint8_t seq;
} key_event_data_t;
