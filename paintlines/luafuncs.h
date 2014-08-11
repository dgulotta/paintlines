#ifndef _LUAFUNCS_H
#define _LUAFUNCS_H

#include <string>
#include <vector>

template<typename T>
class symmetric_canvas;

void run_lua_rule(const char *filename, const char *funcname, symmetric_canvas<uint8_t> &canvas);

std::vector<std::string> get_lua_functions(const char *filename);

const std::vector<std::string> & get_lua_errors();

void clear_lua_errors();

#endif
