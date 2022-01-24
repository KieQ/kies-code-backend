#ifndef DTO_MACRO_HPP
#define DTO_MACRO_HPP

#define FROM_JSON_STR_OPT(j, u, name) u.name = j.contains(#name) && j[#name].is_string()? std::optional<std::string>(j.at(#name).get<std::string>()):std::nullopt

#define FROM_JSON_INT_OPT(j, u, name) u.name = j.contains(#name) && j[#name].is_number_integer()? std::optional<int>(j.at(#name).get<int>()):std::nullopt

#define FROM_JSON_BOOL_OPT(j, u, name) u.name = j.contains(#name) && j[#name].is_boolean()? std::optional<bool>(j.at(#name).get<bool>()):std::nullopt

#define FROM_JSON_FLOAT_OPT(j, u, name) u.name = j.contains(#name) && j[#name].is_number_float()? std::optional<double>(j.at(#name).get<double>()):std::nullopt

#define FROM_JSON_NUM_OPT(j, u, name) u.name = j.contains(#name) && j[#name].is_number()? std::optional<long long>(j.at(#name).get<long long>()):std::nullopt

#define FROM_JSON_STR(j, u, name, dv) u.name = j.contains(#name) && j[#name].is_string()? j.at(#name).get<std::string>():dv

#define FROM_JSON_INT(j, u, name, dv) u.name = j.contains(#name) && j[#name].is_number_integer()? j.at(#name).get<int>():dv

#define FROM_JSON_BOOL(j, u, name, dv) u.name = j.contains(#name) && j[#name].is_boolean()? j.at(#name).get<bool>():dv

#define FROM_JSON_FLOAT(j, u, name, dv) u.name = j.contains(#name) && j[#name].is_number_float()? j.at(#name).get<double>():dv

#define FROM_JSON_NUM(j, u, name, dv) u.name = j.contains(#name) && j[#name].is_number()? j.at(#name).get<long long>():dv

#define FROM_JSON_VECTOR_INT(j, u, name) u.name = j.contains(#name) && j[#name].is_array()? j.at(#name).get<std::vector<std::int64_t>>():std::vector<std::int64_t>{};

#define FROM_JSON_VECTOR_STR(j, u, name) u.name = j.contains(#name) && j[#name].is_array()? j.at(#name).get<std::vector<std::string>>():std::vector<std::string>{};

#define TO_JSON(j, u, name) j[#name] = u.name

#define TO_JSON_OPT(j, u, name) if(u.name.has_value()) j[#name] = u.name.value()

#define TO_JSON_VECTOR(j, u, name) if(!u.name.empty()) j[#name] = u.name;

#define TO_JSON_OPT_WITH_DEFAULT(j, u, name, dv) if(u.name.has_value()) j[#name] = u.name.value(); else j[#name] = dv;

#define ADD_TO_MAP_OPT(m, u, name) if(u.name.has_value()) m[#name] = u.name;

#endif
