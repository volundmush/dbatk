//
// Created by volund on 11/26/21.
//

#ifndef DBATK_SYSDEP_H
#define DBATK_SYSDEP_H

// C++ STD libraries
#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <set>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cstdarg>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#error "no filesystem support ='("
#endif

#if __has_include(<optional>)
#include <optional>
#define opt_type std::optional
#elif __has_include(<experimental/optional>)
#include <experimental/optional>
#define opt_type std::experimental::optional
#else
#error "no optional support ='("
#endif

// imported libraries
#include "fmt/core.h"
#include "nlohmann/json.hpp"
#include "entt/entt.hpp"

typedef uint64_t vnum;
typedef uint64_t flag;

#include <bitset>
#include <iostream>



#endif //DBATK_SYSDEP_H
