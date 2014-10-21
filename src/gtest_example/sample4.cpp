#include "sample4.h"

#include <stdio.h>

int Counter::Increment() {
    return counter_++;
}

void Counter::Print() const {
    printf("%d", counter_);
}
