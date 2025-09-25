#pragma once

#include <stdint.h>

struct Semester {
    int groupId;
    int64_t start;
    int64_t end;
    int cfMaxPoint = 0;
    int atcRatio = 0;
};
