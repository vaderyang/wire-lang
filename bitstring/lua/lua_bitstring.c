#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
#include <stdio.h>
#include <string.h>
#include <bitstring.h>

/*
	BITSTRING INTERFACE	
*/

int new_uint(lua_State *L){

	uint_t val = luaL_checknumber(L,1); //value
	unsigned int size = luaL_checknumber(L,2); //size
	char byte_order = luaL_checknumber(L,3); //byte order
	char bit_order = luaL_checknumber(L,4); //byte order
	
	printf("%d,%d,%d,%d\n", (unsigned int)val, size, byte_order, bit_order);
	
	pbitstring_t pbs = bitstring_new_uint(val, size, byte_order, bit_order);
	pbitstring_t* ppbs = lua_newuserdata(L, sizeof(pbitstring_t));
	*ppbs = pbs;
	
	luaL_getmetatable(L, "bitstring");
	lua_setmetatable(L, -2);
	return 1;
}

int new_sint(lua_State *L){

	sint_t val = luaL_checknumber(L,1); //value
	unsigned int size = luaL_checknumber(L,2); //size
	char byte_order = luaL_checknumber(L,3); //byte order
	char bit_order = luaL_checknumber(L,4); //byte order
	
	printf("%d,%d,%d,%d\n", (signed int)val, size, byte_order, bit_order);
	
	pbitstring_t pbs = bitstring_new_sint(val, size, byte_order, bit_order);
	pbitstring_t* ppbs = lua_newuserdata(L, sizeof(pbitstring_t));
	*ppbs = pbs;
	
	luaL_getmetatable(L, "bitstring");
	lua_setmetatable(L, -2);
	return 1;
}

int new_fp(lua_State *L){
	
	fp_t val = luaL_checknumber(L,1); //value
	unsigned int size = luaL_checknumber(L,2); //size
	char fp_rep = luaL_checknumber(L,3); //fp representation
	
	printf("%f,%d,%d\n", val, size, fp_rep);
	
	pbitstring_t pbs = bitstring_new_fp(val, size, fp_rep);
	pbitstring_t* ppbs = lua_newuserdata(L, sizeof(pbitstring_t));
	*ppbs = pbs;
	
	luaL_getmetatable(L, "bitstring");
	lua_setmetatable(L, -2);
	return 1;
}

/*
	METAMETHODS
*/

int delete(lua_State *L){
	printf("delete...\n");

	pbitstring_t *ppbs = luaL_checkudata(L, 1, "bitstring");
	pbitstring_t pbs = *ppbs;
	
	bitstring_del(pbs);
	return 0;
}

/*
Indexing starts with 1.
*/
int get_bit(lua_State *L){
	printf("get bit...\n");
	
	pbitstring_t *ppbs = luaL_checkudata(L, 1, "bitstring");
	pbitstring_t pbs = *ppbs;
	
	unsigned int lua_pos = luaL_checknumber(L, 2);
	unsigned int c_pos = lua_pos - 1;
	
	if(c_pos<0 || c_pos>=pbs->size){
		lua_pushfstring(L, "Indices out of bound: %d\n", lua_pos);
		lua_error(L);
	}
	char bit = bitstring_get_bit(pbs, c_pos);	
	lua_pushnumber(L, bit);
	return 1;
}

int get_byte(lua_State *L){
	printf("get_byte...\n");
	return 0;
}

int append_bit(lua_State *L){
	printf("append_bit...\n");
	
	pbitstring_t *ppbs = luaL_checkudata(L, 1, "bitstring");
	pbitstring_t pbs = *ppbs;
	
	char bit = luaL_checknumber(L, 2);
	printf("%d\n", bit);
	bitstring_append_bit(pbs, bit);
	
	return 0;
}

int append(lua_State *L){
	printf("append...\n");
	
	pbitstring_t *ppbs_dest = luaL_checkudata(L, 1, "bitstring");
	pbitstring_t pbs_dest = *ppbs_dest;
	
	pbitstring_t *ppbs_src = luaL_checkudata(L, 2, "bitstring");
	pbitstring_t pbs_src = *ppbs_src;
	
	bitstring_append(pbs_dest, pbs_src);
	return 0;
}

int dump(lua_State *L){
	printf("dump...\n");

	pbitstring_t *ppbs = luaL_checkudata(L, 1, "bitstring");
	pbitstring_t pbs = *ppbs;
	
	bitstring_print(stdout, pbs);
	return 0;
}

int size(lua_State *L){
	printf("size...\n");

	pbitstring_t *ppbs = luaL_checkudata(L, 1, "bitstring");
	pbitstring_t pbs = *ppbs;
	
	lua_pushnumber(L, pbs->size);
	return 1;
}

int set_raw(lua_State *L){
	printf("set raw...\n");

	pbitstring_t *ppbs = luaL_checkudata(L, 1, "bitstring");
	pbitstring_t pbs = *ppbs;
	
	unsigned int size_bytes;
	void* data = (void*)luaL_checklstring(L,2, &size_bytes);
	
	printf("%d\n", size_bytes*BYTE_SZ);
	bitstring_set(pbs, data, size_bytes*BYTE_SZ);
	pbs->size = size_bytes*BYTE_SZ;
	
	return 0;
}

int get_raw(lua_State *L){
	printf("get raw...\n");

	pbitstring_t *ppbs = luaL_checkudata(L, 1, "bitstring");
	pbitstring_t pbs = *ppbs;
	
	unsigned int size_bytes = 
		pbs->size%BYTE_SZ ? pbs->size/BYTE_SZ+1 : pbs->size/BYTE_SZ;
	
	printf("%d\n", size_bytes);
	lua_pushlstring(L, pbs->data, size_bytes);
	return 1;
}

/*
	LIB INIT
*/

int luaopen_bitstring(lua_State *L){
	luaL_reg map[] = {
		{"new_uint", new_uint},
		{"new_sint", new_sint},
		{"new_fp", new_fp},
		{NULL, NULL}
		};

	luaL_reg meta_map[] = {
		{"get_bit", get_bit},
		{"get_byte", get_byte},
		{"append_bit", append_bit},
		{"append", append},
		{"set_raw", set_raw},
		{"get_raw", get_raw},
		{"__concat", append},
		{"__gc", delete},
		{"__tostring", dump},
		{"__len", size},
		{NULL, NULL}
	};
	
	luaL_newmetatable(L, "bitstring");

	lua_pushvalue(L, -1);
	
	lua_setfield(L, -2, "__index");
	
	luaL_register(L, NULL, meta_map);
	
	bitstring_lib_init();

	luaL_register(L, "bitstring", map);
	
	return 1;
}

