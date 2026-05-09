#ifndef __WSTL_PARAMETERTYPE_HPP__
#define __WSTL_PARAMETERTYPE_HPP__

#include "Platform.hpp"
#include "../TypeTraits.hpp"


namespace wstl {
    namespace __private {
        template<typename T>
        struct __ParameterType {
        private:
            typedef typename Decay<T>::Type U;
        
        public:
            typedef typename Conditional<IsFundamental<U>::Value || IsPointer<U>::Value || 
                (sizeof(U) <= sizeof(void*)), U, const U&>::Type Type;
        };
    }
}

#endif