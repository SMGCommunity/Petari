#pragma once

namespace nw4r {
    namespace ut {
            namespace detail {
                struct RuntimeTypeInfo {
                    const RuntimeTypeInfo* mParentTypeInfo;
                };
            };

        #define NW4R_UT_RUNTIME_TYPEINFO \
            virtual const nw4r::ut::detail::RuntimeTypeInfo*    GetRuntimeTypeInfo() const { return &typeInfo; } \
            static  const nw4r::ut::detail::RuntimeTypeInfo     typeInfo
    };
};