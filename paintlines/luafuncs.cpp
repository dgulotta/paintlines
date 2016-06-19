#include <lua.hpp>
#include <unordered_set>
#include "luafuncs.h"
#include "paintlines.h"
#include "../randgen.h"

paintlines_layer_generator * get_lg(lua_State *L)
{
	return (paintlines_layer_generator *)lua_touserdata(L,lua_upvalueindex(1));
}

int l_random_int(lua_State *L)
{
	lua_pushinteger(L,random_int(lua_tointeger(L,1)));
	return 1;
}

int l_random_range_inclusive(lua_State *L)
{
	lua_pushinteger(L,random_range_inclusive(lua_tointeger(L,1),lua_tointeger(L,2)));
	return 1;
}

int l_random_uniform(lua_State *L)
{
	lua_pushnumber(L,random_uniform());
	return 1;
}

int l_random_angle(lua_State *L)
{
	lua_pushnumber(L,random_angle());
	return 1;
}

int l_random_exponential(lua_State *L)
{
	lua_pushnumber(L,random_exponential(lua_tonumber(L,1)));
	return 1;
}

int l_random_normal(lua_State *L)
{
	lua_pushnumber(L,random_normal(lua_tonumber(L,1)));
	return 1;
}

int l_random_bernoulli(lua_State *L)
{
	lua_pushboolean(L,random_bernoulli(lua_tonumber(L,1)));
	return 1;
}

int l_random_poisson(lua_State *L)
{
	lua_pushinteger(L,random_poisson(lua_tonumber(L,1)));
	return 1;
}

int l_drawdotsymmetric(lua_State *L)
{
	get_lg(L)->drawdotsymmetric(lua_tointeger(L,1),lua_tointeger(L,2),lua_tointeger(L,3),lua_tonumber(L,4));
	return 0;
}

int l_num_symmetries(lua_State *L)
{
	lua_pushinteger(L,num_symmetries(get_lg(L)->group()));
	return 1;
}

int l_div(lua_State *L)
{
	lua_pushinteger(L,lua_tointeger(L,1)/lua_tointeger(L,2));
	return 1;
}

int l_mod(lua_State *L)
{
	lua_pushinteger(L,lua_tointeger(L,1)%lua_tointeger(L,2));
	return 1;
}

std::vector<std::string> lua_errors;

class lua_stateptr {
public:
	lua_stateptr() : L(lua_open()) {}
	~lua_stateptr() { lua_close(L); }
	operator lua_State * () { return L; }
private:
	lua_State *L;
};

void prepare_lua_environment(lua_State *L, paintlines_layer_generator &gen)
{
	lua_pushlightuserdata(L,&gen);
	lua_pushcclosure(L,l_drawdotsymmetric,1);
	lua_setglobal(L,"drawdotsymmetric");
	lua_pushinteger(L,gen.size());
	lua_setglobal(L,"size");
	lua_pushinteger(L,static_cast<int>(gen.group()));
	lua_setglobal(L,"group");
	lua_pushinteger(L,num_symmetries(gen.group()));
	lua_setglobal(L,"num_symmetries");
	lua_pushboolean(L,gen.is_hexagonal());
	lua_setglobal(L,"hexagonal");
	lua_register(L,"random_int",l_random_int);
	lua_register(L,"random_range_inclusive",l_random_range_inclusive);
	lua_register(L,"random_uniform",l_random_uniform);
	lua_register(L,"random_angle",l_random_angle);
	lua_register(L,"random_exponential",l_random_exponential);
	lua_register(L,"random_normal",l_random_normal);
	lua_register(L,"random_bernoulli",l_random_bernoulli);
	lua_register(L,"random_poisson",l_random_poisson);
	lua_register(L,"div",l_div);
	lua_register(L,"mod",l_mod);
}

void run_lua_rule(const char *filename, const char *funcname, symmetric_canvas<uint8_t> &canvas) {
	lua_stateptr L;
	paintlines_layer_generator gen(canvas);
	prepare_lua_environment(L,gen);
	if(luaL_dofile(L,filename))
	{
		lua_errors.emplace_back(lua_tostring(L,-1));
		return;
	}
	lua_getglobal(L,funcname);
	if(lua_isnil(L,-1)) {
		lua_errors.emplace_back("invalid function name");
		return;
	}
	if(lua_pcall(L,0,0,0)) {
		lua_errors.emplace_back(lua_tostring(L,-1));
		return;
	}
}

std::vector<std::string> function_names(lua_State *L)
{
	std::vector<std::string> names;
	lua_pushvalue(L,LUA_GLOBALSINDEX);
	lua_pushnil(L);
	while(lua_next(L,-2)!=0) {
		if(lua_isfunction(L,-1))
			names.push_back(lua_tostring(L,-2));
		lua_pop(L,1);
	}
	return names;
}

std::vector<std::string> get_lua_functions(const char *filename)
{
	lua_stateptr L;
	symmetric_canvas<uint8_t> c(2,symgroup::P1);
	paintlines_layer_generator gen(c);
	prepare_lua_environment(L,gen);
	auto hide = function_names(L);
	std::unordered_set<std::string> hideset(hide.begin(),hide.end());
	if(luaL_dofile(L,filename)) {
		lua_errors.push_back(lua_tostring(L,-1));
		return {};
	}
	auto allfuncs = function_names(L);
	std::vector<std::string> funcs;
	for(auto s : allfuncs)
		if(hideset.count(s)==0) funcs.push_back(s);
	return funcs;
}

const std::vector<std::string> &get_lua_errors() { return lua_errors; }
void clear_lua_errors() { lua_errors.clear(); }
