#pragma once

#include "i_datastream.h"
#include "nvs_flash.h"
#include "persistent_datastream_config.h"

typedef struct {
  i_datastream_t                        interface;
  const persistent_datastream_config_t* config;
  void*                                 cache;
  nvs_handle_t                          handle;
} nvs_datastream_t;

void nvs_datastream_init(nvs_datastream_t* instance,
                         const persistent_datastream_config_t* config,
                         void* cache,
                         const char* namespace);
