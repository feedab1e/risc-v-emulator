#pragma once

#include <iostream>
#include "formats.hpp"

namespace rv32i::instructions{

struct lui: format::u {
  static constexpr uint32_t opcode = 0b0110111;
  void invoke(auto &machine){
    //machine
    machine.registers[u::rd] = u::get_immediate();

#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[u::rd]
        << std::dec << std::endl;
#endif
  }
};
struct auipc: format::u {
  static constexpr uint32_t opcode = 0b0010111;
  void invoke(auto &machine){

    machine.registers[u::rd] = (machine.pc + u::get_immediate());
#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[u::rd]
              << std::dec << std::endl;
#endif
  }
};

struct add : format::r {
  static constexpr uint32_t opcode = 46;
  static constexpr uint32_t func3 = 0;
  void invoke(auto&){
    std::cout << "add\n";
  }
};


struct csrrw: format::i{
  static constexpr uint32_t opcode = 58;
  static constexpr uint32_t func3 = 2;
  void invoke(auto&){
    std::cout << "csrrw\n";
  }
};
struct csrrs: format::i {
  static constexpr uint32_t opcode = 0;
  static constexpr uint32_t func3 = 0;
  void invoke(auto&){
    std::cout << "csrrs\n" << this->imm << "\n";
  }
};


}
