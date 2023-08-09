//
// Created by Artyom on 8/8/2023.
//

#pragma once

#include "instructions.hpp"
#include <iostream>

namespace rv32i::instructions{
INSTRUCTION( MUL, format::r, 0b011'0011, MEMBER(func3, 0b000), MEMBER(func7, 0b000'0001)){
  
    m.registers[i.rd] = ((int32_t)m.registers[i.rs1] * (int32_t)m.registers[i.rs2]);

#ifdef DEBUG
    std::cout<<"[MUL] "
        << reginfo(m, i.rd) << " = "
        << reginfo(m, i.rs1) << "* "
        << reginfo(m, i.rs1) << std::endl;
#endif
};

INSTRUCTION( MULH, format::r, 0b011'0011, MEMBER(func3,  0b001), MEMBER(func7,   0b000'0001)){
  
    m.registers[i.rd] = (((int64_t)(int32_t)m.registers[i.rs1] * (int64_t)(int32_t)m.registers[i.rs2]) >> 32);
#ifdef DEBUG
    std::cout<<"[MULH] "
              << reginfo(m, i.rd) << " = "
              << reginfo(m, i.rs1) << " * "
              << reginfo(m, i.rs1);
#endif
};

INSTRUCTION( MULHSU, format::r, 0b011'0011, MEMBER(func3,  0b010), MEMBER(func7,   0b000'0001)){
  

    m.registers[i.rd] = ((int64_t)(int32_t)m.registers[i.rs1] * (uint64_t)m.registers[i.rs2])>>32;
#ifdef DEBUG
    std::cout<<"[MULH] "
              << reginfo(m, i.rd) << " = "
              << reginfo(m, i.rs1) << " * "
              << reginfo(m, i.rs1);
#endif
};

INSTRUCTION( MULHU, format::r, 0b011'0011, MEMBER(func3,  0b011), MEMBER(func7,   0b000'0001)){
  

    m.registers[i.rd] = ((uint64_t)m.registers[i.rs1] * (uint64_t )m.registers[i.rs2])>>32;
#ifdef DEBUG
    std::cout<<"[MULH] "
              << reginfo(m, i.rd) << " = "
              << reginfo(m, i.rs1) << " * "
              << reginfo(m, i.rs1);
#endif
};

INSTRUCTION( DIV, format::r, 0b011'0011, MEMBER(func3,  0b100), MEMBER(func7,   0b000'0001)){
  

    if(m.registers[i.rs2] == 0){
      m.registers[i.rd] = -1;
    } else{
     m.registers[i.rd] = ((int32_t)m.registers[i.rs1] == INT32_MIN && (int32_t)m.registers[i.rs2] == -1) ? m.registers[i.rs1] : ((int32_t)m.registers[i.rs1] / (int32_t)m.registers[i.rs2]);
    }
#ifdef DEBUG
    std::cout<<"[DIV] ";
#endif
};

INSTRUCTION( DIVU, format::r, 0b011'0011, MEMBER(func3,  0b101), MEMBER(func7,   0b000'000)){
  
    if( m.registers[i.rs2] == 0 ){
     m.registers[i.rd]  = 0xffffffff;
    }  else{
     m.registers[i.rd] = m.registers[i.rs1] / m.registers[i.rs2];
    }
#ifdef DEBUG
    std::cout<<"[DIVU] ";
#endif
};
INSTRUCTION( REM, format::r, 0b011'0011, MEMBER(func3,  0b110), MEMBER(func7,  0b000'000)){
  
    if(m.registers[i.rs2] == 0){
     m.registers[i.rd] = m.registers[i.rs1];
    } else{
     m.registers[i.rd] = ((int32_t)m.registers[i.rs1] == INT32_MIN && (int32_t)m.registers[i.rs2] == -1) ? 0 : ((int32_t)m.registers[i.rs1] % (int32_t)m.registers[i.rs2]);
    }
#ifdef DEBUG
    std::cout<<"[REM] ";
#endif
};
INSTRUCTION( REMU, format::r, 0b011'0011, MEMBER(func3,  0b111), MEMBER(func7, 0b000'000)){
  
    if( m.registers[i.rs2] == 0 ){
     m.registers[i.rd]  = m.registers[i.rs1];
    }  else{
     m.registers[i.rd] =  m.registers[i.rs1] % m.registers[i.rs2];
    }
#ifdef DEBUG
    std::cout<<"[REMU] ";
#endif
};
auto rv32i_isa_m = ::rv32i::detail::meta::tuple(MUL, MULH, MULHU, MULHSU, DIV, DIVU, REM, REMU);
//#pragma pop_macro("INSTRUCTION")
//#pragma pop_macro("MEMBER")

}
