#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <type_traits>
#include <vector>
#include <bitset>

#include "detail/meta.hpp"

namespace rv32i{
namespace detail{

constexpr auto fold_with(auto f, auto acc, auto val, auto... vals){
  return fold_with(f, f(acc, val), vals...);
}
constexpr auto fold_with(auto, auto acc){
  return acc;
}

template<auto formats, auto instrs>
struct dispatch{
  void operator()(uint32_t i, auto& machine){
    if constexpr (instrs.size() == 0){
      machine.illegal_instruction();
    }else{
      constexpr auto max_id = instrs([](auto... instr){
        return fold_with([](auto a, auto b){
          return a > b ? a : b;
        }, decltype(*instr)::opcode...);
      });
      constexpr auto types_with_max_opcode = instrs.select(
        [&](auto instr){
          return detail::meta::lift_value<decltype(*instr)::opcode == max_id>{};
        });
      constexpr auto types_with_no_max_opcode = instrs.select(
        [&](auto instr){
          return detail::meta::lift_value<decltype(*instr)::opcode != max_id>{};
        });
      constexpr auto is_format = [](auto instr, auto format){
        return instr([&](auto...instr){
          return (std::is_base_of_v<decltype(*format), decltype(*instr)> && ... && true);
        });
      };
      constexpr auto max_id_format = formats(
        [&](auto... fmts){
          return fold_with([&](auto prev, auto fmt){
            if constexpr (is_format(types_with_max_opcode, fmt))
              return fmt;
            else return prev;
          },
          nullptr, fmts...);
        });

      if((i & 0b111'1111) == max_id){
        types_with_max_opcode(meta::fix|[&](auto self, auto insn, auto... insns){
          auto ins = std::bit_cast<decltype(*insn)>(i);
          if constexpr(sizeof...(insns) == 0){
            if(ins.dispatch()) ins.invoke(machine);
            else machine.illegal_instruction();
          } else {
            if(ins.dispatch()) ins.invoke(machine);
            else self(insns...);
          }
        });
      }else{
        dispatch<formats, types_with_no_max_opcode>{}(i, machine);
        //machine.illegal_instruction();
      }
    }
  }
};
}

void writeBinaryFile(std::string fileName, unsigned int arr[], size_t len)
{
  std::ofstream f (fileName.c_str(), std::ios::binary | std::ios::out);
  f.write(reinterpret_cast<const char *>(arr), len);
  //for (size_t i = 0; i < len; i++)
  //  f << arr[i];
}

template<
  auto formats,
  auto instrs
>
struct machine{
  std::vector<uint32_t> program;
  std::array<uint32_t, 32> registers = {};
  std::array<uint32_t, 4096> csr = {0};
  uint32_t pc = 0;
  struct trap{
    uint32_t: 31 code;
    uint32_t: 1 interupt;
  };


  void illegal_instruction(){
    printf("error at instruction [pc:%d 0x%X]: 0x%X 0b", this->pc, this->pc, this->program[pc]);
    std::cout<<std::bitset<32>(this->program[pc])<<std::endl;
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
    detail::dispatch<formats, instrs>{}(program.at(pc/4), *this);
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
