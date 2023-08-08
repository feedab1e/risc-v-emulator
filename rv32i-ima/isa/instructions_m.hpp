//
// Created by Artyom on 8/8/2023.
//

#pragma once

#include "instructions.hpp"
#include <iostream>

namespace rv32i::instructions{
struct MUL: format::r {
  static constexpr uint32_t opcode = 0b011'0111;
  static constexpr uint32_t func3 =  0b000;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){

#ifdef DEBUG
    std::cout<<"[MUL] ";
#endif
  }
};

struct MULH: format::r {
  static constexpr uint32_t opcode = 0b011'0111;
  static constexpr uint32_t func3 =  0b001;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){

#ifdef DEBUG
    std::cout<<"[MULH] ";
#endif
  }
};

struct MULHSU: format::r {
  static constexpr uint32_t opcode = 0b011'0111;
  static constexpr uint32_t func3 =  0b010;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){

#ifdef DEBUG
    std::cout<<"[MULHSU] ";
#endif
  }
};

struct MULHU: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b011;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){

#ifdef DEBUG
    std::cout<<"[MULHU] ";
#endif
  }
};

struct DIV: format::r {
  static constexpr uint32_t opcode = 0b011'0011;
  static constexpr uint32_t func3 =  0b100;
  static constexpr uint32_t func7 =  0b000'0001;
  void invoke(auto &machine){

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

#ifdef DEBUG
    std::cout<<"[REMU] ";
#endif
  }
};
}