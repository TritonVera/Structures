#ifndef __FUNCTION_WRAPPER_HPP
#define __FUNCTION_WRAPPER_HPP

struct ClassMethodCallback {
    void* method;
    void* owner;

    ClassMethodCallback(void* inMethod = nullptr, void* inOwner = nullptr) : method(inMethod), owner(inOwner) {}

    operator bool() {
        return (method != nullptr);
    }

    template<typename ReturnType = void, typename... InputType> ReturnType call(InputType... args) {
        return owner ? ((ReturnType(*)(void*, InputType...))method)(owner, args...) : ((ReturnType(*)(InputType...))method)(args...);
    }

    // template<typename ReturnType> ReturnType operator()() {
    //     return owner ? ((ReturnType(*)(void*))method)(owner) : ((ReturnType(*)())method)();
    // }
};

#endif  //__FUNCTION_WRAPPER_HPP