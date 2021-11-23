#ifndef DTO_MACRO_HPP
#define DTO_MACRO_HPP

#define FROM_JSON_STR_OPT(j, u, name) u.name = utils::get_string(j, #name)

#define FROM_JSON_INT_OPT(j, u, name) u.name = utils::get_integer(j, #name)

#define FROM_JSON_BOOL_OPT(j, u, name) u.name = utils::get_boolean(j, #name)

#define FROM_JSON_FLOAT_OPT(j, u, name) u.name = utils::get_float(j, #name)

#define FROM_JSON_NUM_OPT(j, u, name) u.name = utils::get_number(j, #name)

#define FROM_JSON_STR(j, u, name, dv) u.name = utils::get_string(j, #name, dv)

#define FROM_JSON_INT(j, u, name, dv) u.name = utils::get_integer(j, #name, dv)

#define FROM_JSON_BOOL(j, u, name, dv) u.name = utils::get_boolean(j, #name, dv)

#define FROM_JSON_FLOAT(j, u, name, dv) u.name = utils::get_float(j, #name, dv)

#define FROM_JSON_NUM(j, u, name, dv) u.name = utils::get_number(j, #name, dv)

#define TO_JSON(j, u, name) j[#name] = u.name

#define TO_JSON_OPT(j, u, name) if(u.name.has_value()) j[#name] = u.name.value()

#define TO_JSON_OPT_WITH_DEFAULT(j, u, name, dv) if(u.name.has_value()) j[#name] = u.name.value(); else j[#name] = dv;

#define ADD_TO_MAP_OPT(m, u, name) if(u.name.has_value()) m[#name] = u.name;

#endif