#include "debug.h"

#include "instrument.h"
#include "stalker.h"

static GumStalker *stalker = NULL;

void stalker_init() {

  stalker = gum_stalker_new();
  if (stalker == NULL) { FATAL("Failed to initialize stalker"); }

  gum_stalker_set_trust_threshold(stalker, 0);

}

GumStalker *stalker_get() {

  if (stalker == NULL) { FATAL("Stalker uninitialized"); }
  return stalker;

}

__attribute__((noinline)) static void stalker_activation() {

  asm volatile("");

}

void stalker_start() {

  GumStalkerTransformer *transformer = instrument_get_transformer();
  gum_stalker_follow_me(stalker, transformer, NULL);

}

void stalker_pause() {

  gum_stalker_deactivate(stalker);

}

void stalker_resume() {

  gum_stalker_activate(stalker, stalker_activation);
  stalker_activation();

}

