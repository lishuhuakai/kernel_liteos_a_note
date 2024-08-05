#ifndef FEATURES_H
#define FEATURES_H

#include "../../include/features.h"

#define weak __attribute__((weak))
#define hidden
#ifndef weak_alias
#define weak_alias(old, new) \
    extern __typeof(old) new __attribute__((weak, alias(#old)))
#endif
#ifndef strong_alias
#define strong_alias(old, new) \
    extern __typeof(old) new __attribute__((alias(#old)))
#endif
#endif
