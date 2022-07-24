#pragma once

#define GE_BIND_CALLBACK_FN(fn) [this](auto && PH1) { fn(std::forward<decltype(PH1)>(PH1)); }