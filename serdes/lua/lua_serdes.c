#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <serdes.h>
#include <bitstring.h>

/*
	SERDES INTERFACE
*/

int new(lua_State *L){
    //printf("new...\n");
	pserdes_t psd = serdes_new();
	pserdes_t* ppsd = lua_newuserdata(L, sizeof(pserdes_t));
	*ppsd = psd;

	luaL_getmetatable(L, "serdes");
	lua_setmetatable(L, -2);
	return 1;
}



/*
	METAMETHODS
*/

int delete(lua_State *L){
	//printf("delete...\n");

	pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    //serdes_del(psd); //BUG: Lua gc crashes REASON: serdes_del frees serdes.pbs but gc does it also.
    free(psd);
	return 0;
}

/*
@note Lua indexing starts with 1 so conversions are made to follow this principle.
*/
int get(lua_State *L){
	//printf("get...\n");

	pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

	pbitstring_t* ppbs = lua_newuserdata(L, sizeof(pbitstring_t));
	*ppbs = bitstring_copy(psd->pbs);

	luaL_getmetatable(L, "bitstring");
	lua_setmetatable(L, -2);

	lua_pushnumber(L, psd->pos+1);
	return 2;
}

int set(lua_State *L){
	printf("set...\n");

	pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    pbitstring_t *ppbs = luaL_checkudata(L, 2, "bitstring");
	pbitstring_t pbs = *ppbs;

	unsigned int pos = luaL_checknumber(L, 3);

	serdes_set(psd, pbs, pos-1);
	return 0;
}

int tostring(lua_State *L){
	////printf("tostring...\n");

	lua_pushstring(L, "serdes: to be implemented");

	return 1;
}

int push_bitstring(lua_State *L){
    pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    pbitstring_t *ppbs = luaL_checkudata(L, 2, "bitstring");
	pbitstring_t pbs = *ppbs;

    serdes_push_bitstring(psd, pbs);

    return 0;
}

int push_uint(lua_State *L){
    pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    uint_t val = (uint_t)luaL_checkint(L,2);
    unsigned int size = (unsigned int)luaL_checkint(L,3);
    char byte_order = (char)luaL_checkint(L,4);
    char bit_order = (char)luaL_checkint(L,5);

    serdes_push_uint(psd, val, size, byte_order, bit_order);
    return 0;
}

int push_sint(lua_State *L){
    pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

	sint_t val = (sint_t)luaL_checkint(L,2);
    unsigned int size = (unsigned int)luaL_checkint(L,3);
    char byte_order = (char)luaL_checkint(L,4);
    char bit_order = (char)luaL_checkint(L,5);

    serdes_push_sint(psd, val, size, byte_order, bit_order);
    return 0;
}

int push_fp(lua_State *L){
    pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    fp_t val = (fp_t)luaL_checknumber(L,2);
    unsigned int size = (unsigned int)luaL_checkint(L,3);
    char fp_rep = (char)luaL_checkint(L,4);
    char byte_order = (char)luaL_checkint(L,5);
    char bit_order = (char)luaL_checkint(L,6);

    serdes_push_fp(psd, val, size, fp_rep, byte_order, bit_order);
    return 0;
}

int pull_bitstring(lua_State *L){
    pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    unsigned int size = (unsigned int)luaL_checkint(L, 2);

    pbitstring_t pbs = serdes_pull_bitstring(psd, size);

    pbitstring_t* ppbs = lua_newuserdata(L, sizeof(pbitstring_t));
	*ppbs = pbs;

	luaL_getmetatable(L, "bitstring");
	lua_setmetatable(L, -2);

    return 1;
}

int pull_uint(lua_State *L){
    pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    unsigned int size = (unsigned int)luaL_checkint(L,2);
    char byte_order = (char)luaL_checkint(L,3);
    char bit_order = (char)luaL_checkint(L,4);
    uint_t val = (uint_t)serdes_pull_uint(psd, size, byte_order, bit_order);

    lua_pushinteger(L, val);
    return 1;
}

int pull_sint(lua_State *L){
    pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    unsigned int size = (unsigned int)luaL_checkint(L,2);
    char byte_order = (char)luaL_checkint(L,3);
    char bit_order = (char)luaL_checkint(L,4);
    sint_t val = (sint_t)serdes_pull_sint(psd, size, byte_order, bit_order);

    lua_pushinteger(L, val);
    return 1;
}

int pull_fp(lua_State *L){
    pserdes_t *ppsd = luaL_checkudata(L, 1, "serdes");
	pserdes_t psd = *ppsd;

    unsigned int size = (unsigned int)luaL_checkint(L,2);
    char fp_rep = (char)luaL_checknumber(L, 3);
    char byte_order = (char)luaL_checkint(L,4);
    char bit_order = (char)luaL_checkint(L,5);
    fp_t val = (fp_t)serdes_pull_fp(psd, size, fp_rep, byte_order, bit_order);

    lua_pushnumber(L, val);
    return 1;
}


/*
	LIB INIT
*/
int luaopen_serdes(lua_State *L){
    //require bitstring module
    lua_getglobal(L, "require");
    lua_pushstring(L, "bitstring");
    lua_pcall(L, 1, 1, 0);

	luaL_reg map[] = {
		{"new", new},
		{NULL, NULL}
		};

	luaL_reg meta_map[] = {
	    {"get", get},
	    {"set", set},
		{"push_bitstring", push_bitstring},
		{"push_uint", push_uint},
		{"push_sint", push_sint},
		{"push_fp", push_fp},
		{"pull_bitstring", pull_bitstring},
		{"pull_uint", pull_uint},
		{"pull_sint", pull_sint},
		{"pull_fp", pull_fp},
		{"__gc", delete},
		{"__tostring", tostring},
		{NULL, NULL}
	};

	luaL_newmetatable(L, "serdes");

	lua_pushvalue(L, -1);

	lua_setfield(L, -2, "__index");

	luaL_register(L, NULL, meta_map);

	luaL_register(L, "serdes", map);

	return 1;
}

