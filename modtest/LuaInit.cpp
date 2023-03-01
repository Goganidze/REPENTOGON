#include <cstdio>
#include <chrono>

#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include <iostream>
#include <sstream>
#include <string>

static std::map<std::string, std::vector<std::pair<std::string, void*>>> _functions;

static int LuaDumpRegistry(lua_State* L) {
	int top = lua_gettop(L);
	lua_newtable(L);
	lua_pushnil(L);
	while (lua_next(L, LUA_REGISTRYINDEX) != 0) {
		lua_pushvalue(L, -2);
		lua_pushvalue(L, -2);
		lua_rawset(L, -5);
		lua_pop(L, 1);
	}

	int newtop = lua_gettop(L);
	if (newtop != top + 1) {
		FILE* f = fopen("repentogon.log", "a");
		fprintf(f, "top = %d, newtop = %d\n", top, newtop);
		fclose(f);
		exit(-1);
	}
	return 1;
}

static int LuaExtractFunctions(lua_State* L) {
	/* int n = lua_gettop(L);
	std::string path;
	if (n == 0) {
		path = "lua_functions.log";
	} else {
		if (lua_type(L, 1) == LUA_TSTRING) {
			path = lua_tostring(L, 1);
		} else {
			path = "lua_functions.log";
		}
	} */

	lua_newtable(L); // t
	int i = 0;
	for (auto iter = _functions.begin(); iter != _functions.end(); ++iter, ++i) {
		lua_newtable(L); // t t

		lua_pushstring(L, "type"); // t t type 
		lua_pushstring(L, iter->first.c_str()); // t t type name
		lua_rawset(L, -3); // t t

		lua_pushstring(L, "functions"); // t t functions
		lua_newtable(L); // t t functions t

		for (auto fn_iter = iter->second.begin(); fn_iter != iter->second.end(); ++fn_iter) {
			lua_pushstring(L, fn_iter->first.c_str()); // t t functions t fnname
			lua_pushlightuserdata(L, fn_iter->second); // t t functions t fname addr
			lua_rawset(L, -3); // t t functions t
		}

		lua_rawset(L, -3); // t t

		lua_rawseti(L, -2, i); // t
	}

	return 1;
}

static void ExtractGameFunctions(lua_State* L, std::vector<std::pair<std::string, void*>>& functions, FILE* f) {
	lua_pushnil(L);
	while (lua_next(L, -3)) {
		if (lua_type(L, -2) == LUA_TSTRING && lua_tostring(L, -2)[0] != '_') {
			const char* name = lua_tostring(L, -2);
			if (lua_type(L, -1) == LUA_TFUNCTION) {
				const void* addr = lua_topointer(L, -1);
				unsigned char nupvalues = *(unsigned char*)((char*)addr + 0x6);

				if (nupvalues == 1) {
					lua_TValue* upvalue = (lua_TValue*)((char*)addr + 0x10);
					if ((upvalue->tt_ & 0xF) == LUA_TUSERDATA) {
						Udata* closure_udata = (Udata*)upvalue->value_.p;
						void* fn_addr = *(void**)((char*)closure_udata + 0x18);
						// fprintf(f, "Found addr of %s at %p\n", name, fn_addr);
						functions.push_back(std::make_pair(name, fn_addr));
					}
				}
			}
		}
		lua_pop(L, 1);
	}
}

static void RegisterMetatables(lua_State* L) {
	FILE* f = fopen("repentogon.log", "a");
	fprintf(f, "Dumping Lua registry\n");

	lua_pushnil(L);
	std::map<std::string, void*> metatables;

	while (lua_next(L, LUA_REGISTRYINDEX)) {
		if (lua_type(L, -2) == LUA_TLIGHTUSERDATA) {
			if (lua_type(L, -1) == LUA_TTABLE) {
				lua_pushstring(L, "__type");
				int __type = lua_rawget(L, -2);

				if (__type == LUA_TSTRING) {
					std::string type(lua_tostring(L, -1));
					void* addr = lua_touserdata(L, -3);

					if (type == "Room" || type == "const Room") {
						lua_pushstring(L, "GetBossID");
						int bossID = lua_rawget(L, -3);

						if (bossID == LUA_TNIL) {
							if (type.find("const") != std::string::npos) {
								type = "const RoomConfig_Room";
							}
							else {
								type = "RoomConfig_Room";
							}
						}

						lua_pop(L, 1);
					}
					else if (type == "RoomDescriptor" || type == "const RoomDescriptor") {
						lua_pushstring(L, "Get");
						int get = lua_rawget(L, -3);

						if (get != LUA_TNIL) {
							if (type.find("const") != std::string::npos) {
								type = "const ArrayProxy_RoomDescriptor";
							}
							else {
								type = "ArrayProxy_RoomDescriptor";
							}
						}

						lua_pop(L, 1);
					}

					lua::RegisterMetatable(lua::GetMetatableIdxFromName(type), addr);
					ExtractGameFunctions(L, _functions[type.c_str()], f);
				}

				lua_pop(L, 1);
			}
		}

		lua_pop(L, 1);
	}

	fprintf(f, "Done dumping Lua registry\n");
	fclose(f);
}

HOOK_METHOD(LuaEngine, Init, (bool Debug) -> void) {
	super(Debug);
	luaL_requiref(g_LuaEngine->_state, "debug", luaopen_debug, 1);
	lua_pop(g_LuaEngine->_state, 1);
	luaL_requiref(g_LuaEngine->_state, "os", luaopen_os, 1);
	lua_pop(g_LuaEngine->_state, 1);
	lua_State* state = g_LuaEngine->runCallbackRegistry->state;
	this->RunBundledScript("resources/scripts/enums_ex.lua");
	this->RunBundledScript("resources/scripts/main_ex.lua");

	luaL_unref(state, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
	lua_getglobal(state, "_RunCallback");
	g_LuaEngine->runCallbackRegistry->key = luaL_ref(state, LUA_REGISTRYINDEX);
}

static int LuaBenchmark(lua_State* L) {
	if (int n = lua_gettop(L); n != 1) {
		return luaL_error(L, "Expected one parameter, got %d", n);
	}

	const char* s = lua_tostring(L, 1);
	if (!s) {
		return luaL_error(L, "Invalid parameter");
	}

	if (luaL_dostring(L, s)) {
		const char* err = lua_tostring(L, 1);
		std::string copy(err);
		lua_pop(L, 1);
		return luaL_error(L, "Invalid error: %s", copy.c_str());
	}

	bool running = lua_gc(L, LUA_GCISRUNNING, 0);
	if (running) {
		lua_gc(L, LUA_GCSTOP, 0);
	}

	auto begin = std::chrono::steady_clock::now();
	luaL_dostring(L, s);
	auto end = std::chrono::steady_clock::now();

	auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	lua_pushinteger(L, diff);

	if (running) {
		lua_gc(L, LUA_GCRESTART, 0);
	}

	return 1;
}

HOOK_METHOD_PRIORITY(LuaEngine, RegisterClasses, 100, () -> void) {
	super();
	printf("[REPENTOGON] Registering Lua functions and metatables\n");
	lua_State *state = g_LuaEngine->_state;
	// luaL_openlibs(state);
	lua_register(state, "DumpRegistry", LuaDumpRegistry);
	lua::UnloadMetatables();
	RegisterMetatables(state);
	lua_register(state, "ExtractFunctions", LuaExtractFunctions);
	lua_register(state, "Benchmark", LuaBenchmark);
}