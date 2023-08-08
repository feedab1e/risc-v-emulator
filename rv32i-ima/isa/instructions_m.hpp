//
// Created by Artyom on 8/8/2023.
//

#pragma once

#include "instructions.hpp"
#include <iostream>

namespace rv32i::instructions{
struct MUL: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b000;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){
    machine.registers[r::rd] = ((int32_t)machine.registers[r::rs1] * (int32_t)machine.registers[r::rs2]);

#ifdef DEBUG
    std::cout<<"[MUL] "
        << reginfo(machine, rd) << " = "
        << reginfo(machine, rs1) << "* "
        << reginfo(machine, rs1) << std::endl;
#endif
  }
};

struct MULH: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b001;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){
    machine.registers[rd] = (((int64_t)(int32_t)machine.registers[r::rs1] * (int64_t)(int32_t)machine.registers[r::rs2]) >> 32);
#ifdef DEBUG
    std::cout<<"[MULH] "
              << reginfo(machine, rd) << " = "
              << reginfo(machine, rs1) << " * "
              << reginfo(machine, rs1);
#endif
  }
};

struct MULHSU: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b010;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){

    machine.registers[rd] = ((int64_t)(int32_t)machine.registers[r::rs1] * (uint64_t)machine.registers[r::rs2])>>32;
#ifdef DEBUG
    std::cout<<"[MULH] "
              << reginfo(machine, rd) << " = "
              << reginfo(machine, rs1) << " * "
              << reginfo(machine, rs1);
#endif
  }
};

struct MULHU: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b011;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){

    machine.registers[rd] = ((uint64_t)machine.registers[rs1] * (uint64_t )machine.registers[rs2])>>32;
#ifdef DEBUG
    std::cout<<"[MULH] "
              << reginfo(machine, rd) << " = "
              << reginfo(machine, rs1) << " * "
              << reginfo(machine, rs1);
#endif
  }
};

struct DIV: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b100;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){

    if(machine.registers[rs2] == 0){
      machine.registers[rd] = -1;
    } else{
     machine.registers[rd] = ((int32_t)machine.registers[rs1] == INT32_MIN && (int32_t)machine.registers[rs2] == -1) ? machine.registers[rs1] : ((int32_t)machine.registers[rs1] / (int32_t)machine.registers[rs2]);
    }
#ifdef DEBUG
    std::cout<<"[DIV] ";
#endif
  }
};

struct DIVU: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b101;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){
    if( machine.registers[rs2] == 0 ){
     machine.registers[rd]  = 0xffffffff;
    }  else{
     machine.registers[rd] = machine.registers[rs1] / machine.registers[rs2];
    }
#ifdef DEBUG
    std::cout<<"[DIVU] ";
#endif
  }
};
struct REM: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b110;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){
    if(machine.registers[rs2] == 0){
     machine.registers[rd] = machine.registers[rs1];
    } else{
     machine.registers[rd] = ((int32_t)machine.registers[rs1] == INT32_MIN && (int32_t)machine.registers[rs2] == -1) ? 0 : ((int32_t)machine.registers[rs1] % (int32_t)machine.registers[rs2]);
    }
#ifdef DEBUG
    std::cout<<"[REM] ";
#endif
  }
};
struct REMU: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b111;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){
    if( machine.registers[rs2] == 0 ){
     machine.registers[rd]  = machine.registers[rs1];
    }  else{
     machine.registers[rd] =  machine.registers[rs1] % machine.registers[rs2];
    }
#ifdef DEBUG
    std::cout<<"[REMU] ";
#endif
  }
};
}
