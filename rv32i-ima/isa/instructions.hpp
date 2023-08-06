#pragma once

#include <iostream>
#include "formats.hpp"

namespace rv32i::instructions{

struct LUI: format::u {
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
struct AUIPC: format::u {
  static constexpr uint32_t opcode = 0b0010111;
  void invoke(auto &machine){

    machine.registers[u::rd] = (machine.pc + u::get_immediate());
#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[u::rd]
              << std::dec << std::endl;
#endif
  }
};

struct ADD : format::r {
  static constexpr uint32_t opcode = 0b0010'011;
  static constexpr uint32_t func3  = 0b000;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] + machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " + " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct SLT : format::r {
  static constexpr uint32_t opcode = 0b0010'011;
  static constexpr uint32_t func3  = 0b010;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = ((int32_t)machine.registers[r::rs1] < (int32_t)machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " < " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct SLTU : format::r {
  static constexpr uint32_t opcode = 0b0010'011;
  static constexpr uint32_t func3  = 0b011;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] < machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " < " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct AND: format::r {
  static constexpr uint32_t opcode = 0b0010'011;
  static constexpr uint32_t func3  = 0b111;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] & machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " & " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct OR : format::r {
  static constexpr uint32_t opcode = 0b0010'011;
  static constexpr uint32_t func3  = 0b110;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] < machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " | " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct XOR : format::r {
  static constexpr uint32_t opcode = 0b0010'011;
  static constexpr uint32_t func3  = 0b100;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] ^ machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< u::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " ^ " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
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
