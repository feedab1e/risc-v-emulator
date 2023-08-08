#pragma once

#include "types.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <type_traits>
#include <vector>

namespace rv32i{
namespace detail{

template<class formats, class isa>
struct dispatch;

constexpr auto fold_with(auto f, auto acc, auto val, auto... vals){
  return fold_with(f, f(acc, val), vals...);
}
constexpr auto fold_with(auto, auto acc){
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

void writeBinaryFile(std::string fileName, unsigned int arr[], size_t len)
{
  std::ofstream f (fileName.c_str(), std::ios::binary | std::ios::out);
  f.write(reinterpret_cast<const char *>(arr), len);
  //for (size_t i = 0; i < len; i++)
  //  f << arr[i];
}

template<class... formats, class... instrs>
struct machine<
  instruction_formats<formats...>,
  instruction_set<instrs...>
>{
  std::vector<uint32_t> program;
  std::array<uint32_t, 32> registers = {};
  std::array<uint32_t, 4096> csr = {0};
  uint32_t pc = 0;


  void illegal_instruction(){
    printf("error at instruction [pc:%d 0x%X]: 0x%X\n", this->pc, this->pc, this->program[pc]);
    writeBinaryFile(std::filesystem::current_path()/"../test/memory.dump", this->program.data(), this->program.size()*4);
    throw std::runtime_error("пашол нахуй");
  }
  template<class T>
  T load(uint32_t addr)const{
    //using memory_type = typename decltype(program)::value_type;
    //constexpr auto ratio = sizeof(memory_type) / sizeof(T);
    //return program[addr / sizeof(memory_type)] >> CHAR_BIT * sizeof(T) * (addr / sizeof(T) % ratio);
    T tmp;
    memcpy(&tmp, ((uint8_t*)program.data()) + addr, sizeof(T));
    return tmp;
  }
  template<class T>
  void store(uint32_t addr, T value){
    memcpy(((uint8_t*)program.data()) + addr, &value, sizeof(T));
  }
  void step(){
#ifdef DEBUG
    std::cout<< *this;
#endif
    this->registers[0] = 0;
    detail::dispatch<
      instruction_formats<formats...>,
      instruction_set<instrs...>
    >{}(program.at(pc/4), *this);
    pc+=4;
  }

  friend std::ostream& operator<<(std::ostream& os, const auto& machine) {
    std::cout.fill('0');
    return os << "PC[" << std::setw(5)   << machine.pc/4
               << " 0x" <<std::setw(8) << std::hex << machine.pc
              << "]: 0x" << std::setw(8) << machine.program[machine.pc/4]
              <<std::dec
              << "\t";
  }
};

}
