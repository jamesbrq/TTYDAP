#pragma once

#include "gc/pad.h"
#include "ttyd/party.h"

#include <cstdint>

uint32_t autoMashText(gc::pad::PadId controllerId);
void *fixPouchInitMemoryLeak(int32_t heap, uint32_t size);
void *fixMapProblems();
