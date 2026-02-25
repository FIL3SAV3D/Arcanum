//#include "Serializer.h"
//
//#include <fmt/core.h>
//
//template<typename T>
//bool Serializer::Serialize(T _Object)
//{
//    auto [data, out] = zpp::bits::data_out();
//
//    auto result = out(_Object);
//
//    if (failure(result)) {
//        // `result` is implicitly convertible to `std::errc`.
//        // handle the error or return/throw exception.
//        fmt::println("{}", result);
//        return false;
//    }
//
//    return true;
//}
//
//template<typename T>
//bool Serializer::Deserialize(T& _Object)
//{
//    auto [data, in] = zpp::bits::data_in();
//
//    auto result = in(_Object);
//
//    if (failure(result)) {
//        // `result` is implicitly convertible to `std::errc`.
//        // handle the error or return/throw exception.
//        fmt::println("{}", result);
//        return false;
//    }
//
//    return true;
//}