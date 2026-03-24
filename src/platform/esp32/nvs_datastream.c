#include <stdio.h>
#include <string.h>

#include "i_datastream.h"
#include "nvs_datastream.h"

static bool contains(i_datastream_t* interface, datastream_key_t key)
{
  nvs_datastream_t* instance = (nvs_datastream_t*)interface;
  return key < instance->config->count && instance->config->entries[key].size > 0;
}

static uint8_t size(i_datastream_t* interface, datastream_key_t key)
{
  if(contains(interface, key)) {
    nvs_datastream_t* instance = (nvs_datastream_t*)interface;
    return instance->config->entries[key].size;
  }
  return 0;
}

static void read(i_datastream_t* interface, datastream_key_t key, void* out)
{
  if(contains(interface, key)) {
    nvs_datastream_t* instance = (nvs_datastream_t*)interface;
    const persistent_datastream_entry_t* entry = &instance->config->entries[key];
    memcpy(out, (uint8_t*)instance->cache + entry->offset, entry->size);
  }
}

static void write(i_datastream_t* interface, datastream_key_t key, const void* data)
{
  if(!contains(interface, key))
    return;

  nvs_datastream_t* instance = (nvs_datastream_t*)interface;
  const persistent_datastream_entry_t* entry = &instance->config->entries[key];
  void* cached = (uint8_t*)instance->cache + entry->offset;

  if(memcmp(cached, data, entry->size) == 0)
    return;

  memcpy(cached, data, entry->size);

  char key_name[8];
  snprintf(key_name, sizeof(key_name), "k%u", (unsigned)key);
  nvs_set_blob(instance->handle, key_name, data, entry->size);
  nvs_commit(instance->handle);

  datastream_on_change_args_t args = { .key = key, .data = data };
  event_publish(&interface->all_on_change, &args);
}

static void unsubscribe(i_datastream_t* interface, event_subscription_t* subscription)
{
  event_unsubscribe(&interface->all_on_change, subscription);
}

void nvs_datastream_init(nvs_datastream_t* instance,
  const persistent_datastream_config_t* config,
  void* cache,
  const char* namespace)
{
  instance->config = config;
  instance->cache = cache;

  instance->interface = (i_datastream_t){
    .read = read,
    .write = write,
    .contains = contains,
    .size = size,
    .unsubscribe = unsubscribe,
  };

  event_init(&instance->interface.all_on_change);

  nvs_flash_init();
  nvs_open(namespace, NVS_READWRITE, &instance->handle);

  // Zero-init cache, then load any previously persisted values from NVS
  for(uint16_t i = 0; i < config->count; i++) {
    persistent_datastream_entry_t* entry = &config->entries[i];
    if(entry->size == 0)
      continue;

    uint8_t* slot = (uint8_t*)cache + entry->offset;
    memset(slot, 0, entry->size);

    char key_name[8];
    snprintf(key_name, sizeof(key_name), "k%u", (unsigned)i);
    size_t blob_len = entry->size;
    nvs_get_blob(instance->handle, key_name, slot, &blob_len);
    // If ESP_ERR_NVS_NOT_FOUND or any error, slot stays zero — that's fine.

  }
}
