#include "garbage_collector.h"

Collector& GetCollector() {
    static Collector collector;
    return collector;
}
