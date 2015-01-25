#ifndef LIBLYX_FOUNDATION_LYXANY_H_
#define LIBLYX_FOUNDATION_LYXANY_H_

#include "lyxMetaProgramming.h"
#include "lyxException.h"
#include <cstring>
#include <iostream>

namespace lyx {

class Any;

namespace Dynamic {

class Var;
class VarHolder;
template <class> class VarHolderImpl;

} // namespace Dynamic

template <typename PlaceholderT, unsigned int SizeV = 32>
union Placeholder {
    public:
        struct Size {
            static const unsigned int value = SizeV;
        };

        Placeholder() {
            erase();
        }

        void erase() {
            std::memset(holder, 0, sizeof(Placeholder));
        }

        bool isLocal() const {
            return holder[SizeV] != 0;
        }

        void setLocal(bool local) const {
            holder[SizeV] = local ? 1: 0;
        }

        PlaceholderT* content() const {
            if (isLocal())
                return reinterpret_cast<PlaceholderT*>(holder);
            else
                return pHolder;
        }

    private:
        typedef typename std::aligned_storage<SizeV + 1>::type AlignerType;

        PlaceholderT* pHolder;
        mutable char  holder[SizeV + 1];
        AlignerType   aligner;

        friend class Any;
        friend class Dynamic::Var;
        friend class Dynamic::VarHolder;
        template <class> friend class Dynamic::VarHolderImpl;
};

class Any {
    public:
        Any() {
        }

        template<typename ValueType>
            Any(const ValueType& value) {
                construct(value);
            }

        Any(const Any& other) {
            if ((this != &other) && !other.empty())
                construct(other);
        }

        ~Any() {
            if (!empty()) {
                if (_valueHolder.isLocal())
                    destruct();
                else
                    delete content();
            }
        }

        Any& swap(Any& other) {
            if (this == &other) return *this;

            if (!_valueHolder.isLocal() && !other._valueHolder.isLocal()) {
                std::swap(_valueHolder.pHolder, other._valueHolder.pHolder);
            }
            else {
                Any tmp(*this);
                try {
                    if (_valueHolder.isLocal()) destruct();
                    construct(other);
                    other = tmp;
                }
                catch (...) {
                    construct(tmp);
                    throw;
                }
            }

            return *this;
        }

        template<typename ValueType>
            Any& operator = (const ValueType& rhs) {
                construct(rhs);
                return *this;
            }

        Any& operator = (const Any& rhs) {
            if ((this != &rhs) && !rhs.empty())
                construct(rhs);
            else if ((this != &rhs) && rhs.empty())
                _valueHolder.erase();

            return *this;
        }

        bool empty() const {
            char buf[32] = {0};
            return 0 == std::memcmp(_valueHolder.holder, buf, 32);
        }

        const std::type_info & type() const {
            return empty() ? typeid(void) : content()->type();
        }

    private:

        class ValueHolder {
            public:
                virtual ~ValueHolder() {
                }

                virtual const std::type_info & type() const = 0;
                virtual void clone(Placeholder<ValueHolder>*) const = 0;
        };

        template<typename ValueType>
            class Holder : public ValueHolder {
                public:
                    Holder(const ValueType & value) : _held(value) {
                    }

                    virtual const std::type_info & type() const {
                        return typeid(ValueType);
                    }

                    virtual void clone(Placeholder<ValueHolder>* pPlaceholder) const {
                        if ((sizeof(Holder<ValueType>) <= 32)) {
                            new ((ValueHolder*) pPlaceholder->holder) Holder(_held);
                            pPlaceholder->setLocal(true);
                        }
                        else {
                            pPlaceholder->pHolder = new Holder(_held);
                            pPlaceholder->setLocal(false);
                        }
                    }

                    ValueType _held;

                private:
                    Holder & operator = (const Holder &);
            };

        ValueHolder* content() const {
            return _valueHolder.content();
        }

        template<typename ValueType>
            void construct(const ValueType& value) {
                if (sizeof(Holder<ValueType>) <= Placeholder<ValueType>::Size::value) {
                    new (reinterpret_cast<ValueHolder*>(_valueHolder.holder))Holder<ValueType>(value);
                    _valueHolder.setLocal(true);
                }
                else {
                    _valueHolder.pHolder = new Holder<ValueType>(value);
                    _valueHolder.setLocal(false);
                }
            }

        void construct(const Any& other) {
            if (!other.empty())
                other.content()->clone(&_valueHolder);
            else
                _valueHolder.erase();
        }

        void destruct() {
            content()->~ValueHolder();
        }

        Placeholder<ValueHolder> _valueHolder;

        template <typename ValueType>
            friend ValueType* AnyCast(Any*);
        template <typename ValueType>
            friend ValueType* UnsafeAnyCast(Any*);
};

template <typename ValueType>
ValueType* AnyCast(Any* operand) {
    return operand && operand->type() == typeid(ValueType)
        ? &static_cast<Any::Holder<ValueType>*>(operand->content())->_held
        : 0;
}

template <typename ValueType>
const ValueType* AnyCast(const Any* operand) {
    return AnyCast<ValueType>(const_cast<Any*>(operand));
}

template <typename ValueType>
ValueType AnyCast(Any& operand) {
    typedef typename TypeWrapper<ValueType>::TYPE NonRef;

    NonRef* result = AnyCast<NonRef>(&operand);
    if (!result) throw BadCastException("Failed to convert between Any types");
    return *result;
}

template <typename ValueType>
ValueType AnyCast(const Any& operand) {
    typedef typename TypeWrapper<ValueType>::TYPE NonRef;

    return AnyCast<NonRef&>(const_cast<Any&>(operand));
}

template <typename ValueType>
const ValueType& RefAnyCast(const Any & operand) {
    ValueType* result = AnyCast<ValueType>(const_cast<Any*>(&operand));
    if (!result) throw BadCastException("RefAnyCast: Failed to convert between const Any types");
    return *result;
}

template <typename ValueType>
ValueType& RefAnyCast(Any& operand) {
    ValueType* result = AnyCast<ValueType>(const_cast<Any*>(&operand));
    if (!result) throw BadCastException("RefAnyCast: Failed to convert between Any types");
    return *result;
}

template <typename ValueType>
ValueType* UnsafeAnyCast(Any* operand) {
    return &static_cast<Any::Holder<ValueType>*>(operand->content())->_held;
}

template <typename ValueType>
const ValueType* UnsafeAnyCast(const Any* operand) {
    return AnyCast<ValueType>(const_cast<Any*>(operand));
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXANY_H_
