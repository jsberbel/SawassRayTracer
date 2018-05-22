#pragma once

// SFINAE Template Conditions
#define ENABLE_IF(...)              class = std::enable_if<__VA_ARGS__>
#define IS_REAL(NUM)                std::is_floating_point<NUM>::value
#define IS_INT(NUM)                 std::is_integral<NUM>::value
#define IS_CLASS(C)                 std::is_class<C>::value
#define IS_ENUM(E)                  std::is_enum<E>::value
#define IS_FUNCTION(F)              std::is_function<F>::value
#define IS_REFERENCE(R)             std::is_reference<R>::value
#define IS_POINTER(P)               std::is_pointer<P>::value
#define IS_ARRAY(A)                 std::is_array<A>::value
#define IS_BASE_OF(BASE, DERIVED)   std::is_base_of<BASE, DERIVED>::value
#define IS_SAME(A, B)               std::is_same<A, B>::value
#define ARE_SAME(T, Ts)             std::conjunction_v<std::is_same<T, Ts>...>
#define IS_ANY_OF(T, ...)           Framework::Utils::IsAnyOfTrait<T, __VA_ARGS__>::value
#define CAN_CAST(FROM, TO)          std::is_convertible<FROM, TO>::value
#define IF(CONDITION, THEN, ELSE)   std::conditional<CONDITION, THEN, ELSE>::type
#define ADD_ALL(Ts)                 Framework::Utils::AddAllTrait< sizeof(Ts)... >::value