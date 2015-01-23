#ifndef LIBLYX_FOUNDATION_LYXMETAPROGRAMMING_H_
#define LIBLYX_FOUNDATION_LYXMETAPROGRAMMING_H_

namespace lyx {

// Use this struct to determine if a template type is a reference
template <typename T>
    struct IsReference {
        enum {
            VALUE = 0
        };
    };

template <typename T>
    struct IsReference<T&> {
        enum {
            VALUE = 1
        };
    };

template <typename T>
    struct IsReference<const T&> {
        enum {
            VALUE = 1
        };
    };

// Use this struct to determine if a template type is a const type
template <typename T>
    struct IsConst {
        enum {
            VALUE = 0
        };
    };


template <typename T>
    struct IsConst<const T&> {
        enum {
            VALUE = 1
        };
    };


template <typename T>
    struct IsConst<const T> {
        enum {
            VALUE = 1
        };
    };

// Specialization for const char arrays
template <typename T, int i>
    struct IsConst<const T[i]> {
        enum {
            VALUE = 1
        };
    };

// Use the type wrapper if you want to decouple constness and references from template types
template <typename T>
    struct TypeWrapper {
        typedef T TYPE;
        typedef const T CONSTTYPE;
        typedef T& REFTYPE;
        typedef const T& CONSTREFTYPE;
    };

template <typename T>
    struct TypeWrapper<const T> {
        typedef T TYPE;
        typedef const T CONSTTYPE;
        typedef T& REFTYPE;
        typedef const T& CONSTREFTYPE;
    };

template <typename T>
    struct TypeWrapper<const T&> {
        typedef T TYPE;
        typedef const T CONSTTYPE;
        typedef T& REFTYPE;
        typedef const T& CONSTREFTYPE;
    };

template <typename T>
    struct TypeWrapper<T&> {
        typedef T TYPE;
        typedef const T CONSTTYPE;
        typedef T& REFTYPE;
        typedef const T& CONSTREFTYPE;
    };

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXMETAPROGRAMMING_H_
