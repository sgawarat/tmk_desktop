/**
 * @file macro.hpp
 * @brief マクロ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <span>
#include <vector>
#include <cstdint>

extern "C" {
#include <tmk_core/common/action_macro.h>
}

// 短い名前のマクロを取り除く
#undef D
#undef U
#undef T
#undef W
#undef I
#undef SM
#undef RM
#undef CM
#undef ST
#undef MD
#undef MU

namespace tmk_desktop {
}  // namespace tmk_desktop
