#include "any.hpp"
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <functional>
#include <iomanip>

namespace utils
{

    template <class T, class F>
    inline std::pair<const std::type_index, std::function<std::string(std::any const &)>>
    to_any_visitor(F const &f)
    {
        return {
            std::type_index(typeid(T)),
            [g = f](const std::any &a)
            {
                if constexpr (std::is_void_v<T>)
                    return g();
                else
                    return g(std::any_cast<const T &>(a));
            }};
    }

    static std::unordered_map<
        std::type_index, std::function<std::string(std::any const &)>>
        any_visitor{
            to_any_visitor<void>([]
                                 { return "{}"; }),
            to_any_visitor<int>([](int x)
                                { return std::to_string(x); }),
            to_any_visitor<unsigned int>([](unsigned int x)
                                         { return std::to_string(x); }),
            to_any_visitor<std::int64_t>([](std::int64_t x)
                                         { return std::to_string(x); }),
            to_any_visitor<std::int32_t>([](std::int32_t x)
                                         { return std::to_string(x); }),
            to_any_visitor<std::int16_t>([](std::int16_t x)
                                         { return std::to_string(x); }),
            to_any_visitor<std::int8_t>([](std::int8_t x)
                                        { return std::to_string(x); }),
            to_any_visitor<std::uint64_t>([](std::uint64_t x)
                                          { return std::to_string(x); }),
            to_any_visitor<std::uint32_t>([](std::uint32_t x)
                                          { return std::to_string(x); }),
            to_any_visitor<std::uint8_t>([](std::uint8_t x)
                                         { return std::to_string(x); }),
            to_any_visitor<bool>([](bool x)
                                 { return x ? "true" : "false"; }),
            to_any_visitor<std::string>([](std::string x)
                                        {
                                            return x;
                                        }),
            to_any_visitor<char const *>([](char const *x)
                                         {
                                            return std::string(x);
                                         }),
            to_any_visitor<std::string_view>([](std::string_view x)
                                             {
                                                 return std::string(x);
                                             }),
        };

    std::string any_to_string(const std::any &any)
    {
        using namespace std;

        if (const auto it = any_visitor.find(std::type_index(any.type()));
            it != any_visitor.cend())
        {
            return it->second(any);
        }
        else
        {
            return "unsupported type, type = "s + any.type().name();
        }
    }

} // namespace utils
