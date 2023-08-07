#pragma once

#include <cstdint>
#include <exception>
#include <type_traits>
#include <vector>
#include <array>
#include "types.hpp"

namespace rv32i{
namespace detail{

template<class formats, class isa>
struct dispatch;

constexpr auto fold_with(auto f, auto acc, auto val, auto... vals){
  return fold_with(f, f(acc, val), vals...);
};
constexpr auto fold_with(auto f, auto acc){
  return acc;
}

template<class T>
struct type_v{
  using type = T;
  T operator*()const noexcept;
};

template<class... formats, class... instrs>
struct dispatch<
  instruction_formats<formats...>,
  instruction_set<instrs...>
>{
  void operator()(uint32_t i, auto& machine){
    constexpr auto max_id = fold_with([](auto a, auto b){return a > b ? a : b; }, instrs::opcode...);
    constexpr auto types_with_max_opcode = fold_with(
      []<class... Ts, class U>(instruction_set<Ts...>, type_v<U>){
        if constexpr(U::opcode == max_id)
          return instruction_set<Ts..., U>{};
        else return instruction_set<Ts...>{};
      },
      instruction_set<>{},
      type_v<instrs>{}...);
    constexpr auto types_with_no_max_opcode = fold_with(
      []<class... Ts, class U>(instruction_set<Ts...>, type_v<U>){
        if constexpr(U::opcode != max_id)
          return instruction_set<Ts..., U>{};
        else return instruction_set<Ts...>{};
      },
      instruction_set<>{},
      type_v<instrs>{}...);
    constexpr auto is_format = []<class... instr>(instruction_set<instr...>, auto format){
      return (std::is_base_of_v<decltype(*format), instr> | ... | false);
    };
    constexpr auto max_id_format = fold_with([&](auto prev, auto fmt){
      if constexpr (is_format(types_with_max_opcode, fmt))
        return fmt;
      else return prev;
    }, nullptr, type_v<formats>{}...);

    if((i & 0b111'1111) == max_id){
      auto instr = std::bit_cast<decltype(*max_id_format)>(i);
      [&]<class... Is>(instruction_set<Is...>){
        instr.template dispatch<Is...>(machine);
      }(types_with_max_opcode);
    }else{
      dispatch<instruction_formats<formats...>, std::remove_cv_t<decltype(types_with_no_max_opcode)>>{}(i, machine);
    }
  }
};
template<class... formats>
struct dispatch<instruction_formats<formats...>, instruction_set<>>{
  auto operator()(uint32_t, auto& machine){
    machine.illegal_instruction();
  }
};
}
template<class formats, class isa>
struct machine;

template<class... formats, class... instrs>
struct machine<
  instruction_formats<formats...>,
  instruction_set<instrs...>
>{
  std::vector<uint32_t> program;
  std::array<int32_t, 32> registers;
  uint32_t pc = 0;
  void illegal_instruction(){
    throw std::runtime_error("пашол нахуй");
  }
  template<class T>
  T load(uint32_t addr)const{
    using memory_type = typename decltype(program)::value_type;
    constexpr auto ratio = sizeof(memory_type) / sizeof(T);
    return program[addr / sizeof(memory_type)] >> CHAR_BIT * sizeof(T) * (addr / sizeof(T) % ratio);
  }
  template<class T>
  void store(uint32_t addr, T value){
    using memory_type = typename decltype(program)::value_type;
    constexpr auto ratio = sizeof(memory_type) / sizeof(T);
    auto oldval = program[addr/sizeof(memory_type)];
    memory_type towrite = value;
    oldval |= towrite << CHAR_BIT * sizeof(T) * (addr/sizeof(T) % ratio);
    oldval &= ~(~towrite << CHAR_BIT * sizeof(T) * (addr/sizeof(T) % ratio));
    program[addr / sizeof(memory_type)] = oldval;
  }
  void step(){
    detail::dispatch<
      instruction_formats<formats...>,
      instruction_set<instrs...>
    >{}(program.at(pc), *this);
  }
};

}
