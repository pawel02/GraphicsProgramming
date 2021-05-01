#include "Event.h"
std::unordered_map<std::type_index, EventBus::EventVector*> EventBus::_subscribers;