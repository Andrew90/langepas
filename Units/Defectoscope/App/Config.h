﻿#pragma once
#pragma message("ПОДКЛЮЧЕНИЕ ПЛАТ РАСШИРЕНИЯ!!!")
//undef DEBUG_ITEMS - ПОДключение платы: 1730
//define DEBUG_ITEMS - ОТключение платы: 1730
#define DEBUG_ITEMS

#ifndef DEBUG_ITEMS
#pragma message("подключены платы 1730")
#endif

#define THREAD_PRIORITY

#define CLIENT

#define EMUL

