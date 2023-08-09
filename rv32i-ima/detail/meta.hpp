#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>
#include <memory>

#pragma push_macro("FWD")
#define FWD(x) ((decltype(x))x)

namespace rv32i::detail::meta{

template<auto x>
struct lift_value{
  constexpr operator decltype(x)()const noexcept{
    return x;
  }
#pragma push_macro("DEFINE_OP")
#define DEFINE_OP(op)template<auto y> constexpr auto operator op(lift_value<y>)->lift_value<(x op y)>{return {};}
DEFINE_OP(+)
DEFINE_OP(-)
DEFINE_OP(*)
DEFINE_OP(/)
DEFINE_OP(==)
DEFINE_OP(!=)
DEFINE_OP(<=)
DEFINE_OP(>=)
DEFINE_OP(<)
DEFINE_OP(>)
DEFINE_OP(||)
DEFINE_OP(&&)
#pragma pop_macro("DEFINE_OP")
};
template<char... cs>
constexpr auto operator""_v(){
  constexpr char str[] = {cs...};
  constexpr auto parse = [](auto in){
    std::size_t i = 0;
    if(sizeof str >= 2 && in[0] == '0' && in[1] == 'x'){
      for(auto c = in + 2; c != in + sizeof str; ++c)
        switch(*c){
          case '\'': break;
          case '0'...'9': i = i * 16 + *c - '0'; break;
          case 'a'...'f': i = i * 16 + *c - 'a' + 10; break;
          case 'A'...'F': i = i * 16 + *c - 'A' + 10; break;
        }
      return i;
    }
    if(sizeof str >= 2 && in[0] == '0' && in[1] == 'b'){
      for(auto c = in + 2; c != in + sizeof str; ++c)
        switch(*c){
          case '\'': break;
          case '0'...'1': i = i * 2 + *c - '0'; break;
        }
      return i;
    }
    if(in[0] == '0'){
      for(auto c = in + 1; c != in + sizeof str; ++c)
        switch(*c){
          case '\'': break;
          case '0'...'7': i = i * 8 + *c - '0'; break;
        }
      return i;
    }
      for(auto c = in; c != in + sizeof str; ++c)
        switch(*c){
          case '\'': break;
          case '0'...'9': i = i * 10 + *c - '0'; break;
        }
      return i;
  };
  return lift_value<parse(str)>{};
}

template<class T>
struct lift_type{
  T operator*()const noexcept{
    static_assert("this function should never be called");
  }
  T* operator->()const noexcept{
    static_assert("this function should never be called");
  }
  lift_value<false> operator==(auto){return{};};
  lift_value<true> operator==(lift_type){return{};};
  lift_value<true> operator!=(auto){return{};};
  lift_value<false> operator!=(lift_type){return{};};
};

template<class T, std::size_t I>
struct tuple_element{
  T v;
  constexpr T& operator[](lift_value<I>)&{return v;}
  constexpr const T& operator[](lift_value<I>)const&{return v;}
  constexpr T&& operator[](lift_value<I>)&&{return (T&&)v;}
};

template<class iseq, class... T>
class tuple;

constexpr auto make_tuple(auto... args){
  return tuple{args...};
}

template<std::size_t... i, class... T>
class tuple<std::index_sequence<i...>, T...> : public tuple_element<T, i>...{
  constexpr auto filter_indices(auto&& f, auto idx) const{
    if constexpr(idx == sizeof...(i))
      return std::index_sequence<>{};
    else if constexpr(decltype(f(std::declval<tuple>()[idx])){})
      return [&]<std::size_t... Is>(std::index_sequence<Is...>){
        return std::index_sequence<idx, Is...>{};
      }(filter_indices(f, idx+1_v));
    else
      return filter_indices(f, idx+1_v);
  }
public:
  using tuple_element<T, i>::operator[]...;
  constexpr tuple(tuple&&) = default;
  constexpr tuple(const tuple&) = default;
  constexpr tuple(auto&&... vs) requires(sizeof...(vs) == sizeof...(i))
    :tuple_element<T, i>{FWD(vs)}...{}
  constexpr decltype(auto) operator()(auto&& fn)&&{ return ((decltype(fn))fn)(std::move(tuple_element<T,i>::v)...); }
  constexpr decltype(auto) operator()(auto&& fn)&{ return ((decltype(fn))fn)(tuple_element<T,i>::v...); }
  constexpr decltype(auto) operator()(auto&& fn)const&{ return ((decltype(fn))fn)(tuple_element<T,i>::v...); }

  constexpr auto size() const{
    return sizeof...(i);
  }

  constexpr auto select(auto&& fn)const&{
    return [&]<std::size_t... Is>(std::index_sequence<Is...>){
      return make_tuple((*this)[lift_value<Is>{}]...);
    }(filter_indices(fn, 0_v));
  }
  constexpr auto select(auto&& fn)&&{
    return [&]<std::size_t... Is>(std::index_sequence<Is...>){
      return make_tuple(std::move(*this)[lift_value<Is>{}]...);
    }(filter_indices(fn, 0_v));
  }

  constexpr auto contains(auto v){
    return ((tuple_element<T,i>::v == v) || ... || lift_value<false>{});
  }

  constexpr auto operator +(auto&& t)&&{
    return std::move(*this)([&](auto&&... ys){
      return FWD(t)([&](auto&&... xs){return make_tuple(FWD(ys)..., FWD(xs)...);});
    });
  }
  constexpr auto operator +(auto&& t)const&{
    return (*this)([&](auto&&... ys){
      return FWD(t)([&](auto&&... xs){return make_tuple(FWD(ys)..., FWD(xs)...);});
    });
  }
  constexpr auto operator|(auto&& other)&&{
    return std::move(*this) + FWD(other).select([&](auto x){return !contains(x);});
  }
  constexpr auto operator|(auto&& other)const&{
    return *this + FWD(other).select([&](auto x){return !contains(x);});
  }
  constexpr auto operator&(auto&& other)&&{
    return std::move(*this).select([&](auto x){return other.contains(x);});
  }
  constexpr auto operator&(auto&& other)const&{
    return this->select([&](auto x){return !other.contains(x);});
  }
};

template<class... Ts>
tuple(Ts...)->tuple<std::make_index_sequence<sizeof...(Ts)>, Ts...>;
template<class... Ts>
tuple(tuple<Ts...>)->tuple<Ts...>;

constexpr auto y = 1_v + 2_v;

static_assert(y == 3_v);

auto x = tuple(lift_type<int>{}, lift_type<float>{}) & tuple(lift_type<char>{});

struct {
  template<class F>
  struct fix_t{
    F f;
    auto operator()(auto&&... xs)&      -> decltype(f(*this, FWD(xs)...)){ return f(*this, FWD(xs)...); }
    auto operator()(auto&&... xs)const& -> decltype(f(*this, FWD(xs)...)){ return f(*this, FWD(xs)...); }
    auto operator()(auto&&... xs)&&     -> decltype(f(*this, FWD(xs)...)){ return f(std::move(*this), FWD(xs)...); }
  };
  template<class F>
  auto operator|(F&& f){
    return fix_t<std::remove_cvref_t<F>>{f};
  }
} fix;

}

#pragma pop_macro("FWD")
