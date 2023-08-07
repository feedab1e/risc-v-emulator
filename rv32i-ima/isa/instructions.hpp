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
    std::cout<<"reg["<< r::rd << "]: " << std::hex << machine.registers[u::rd]
              << std::dec << std::endl;
#endif
  }
};

struct ADD : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b000;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] + machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " + " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct SLT : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b010;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = ((int32_t)machine.registers[r::rs1] < (int32_t)machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " < " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct SLTU : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b011;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] < machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " < " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct AND: format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b111;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] & machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " & " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct OR : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b110;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] < machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " | " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct XOR : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b100;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] ^ machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << machine.registers[r::rs1]
              << " ^ " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct SLL : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b100;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] << (machine.registers[r::rs2] & 0x1F));
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << (machine.registers[r::rs1]& 0x1F)
              << " << " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};
struct SRL : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b101;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] >>(machine.registers[r::rs2] & 0x1F));
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << (machine.registers[r::rs2] & 0x1F)
              << " >> " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};
struct SRA : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b101;
  static constexpr uint32_t func7  = 0b0100'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = ((int32_t)machine.registers[r::rs1] >>(machine.registers[r::rs2] & 0x1F));
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << (machine.registers[r::rs2] & 0x1F)
              << " >> " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};
struct SUB : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b000;
  static constexpr uint32_t func7  = 0b0100'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] - (machine.registers[r::rs2] & 0x1F));
#ifdef DEBUG
    std::cout<<"reg["<< r::rd << "]: " << std::hex << machine.registers[r::rs2]
              << " - " << machine.registers[r::rs2]
              << std::dec << std::endl;
#endif
  }
};

struct ADDI: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b000;
  void invoke(auto& machine){
    machine.registers[i::rd] = (machine.registers[i::rs1] + i::get_immediate());
#ifdef DEBUG
    std::cout<<"reg["<< i::rd << "]: " << std::hex << machine.registers[i::rs1]
              << " + " << i::get_immediate()
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


struct JAL : format::j {
  static constexpr auto opcode = 0b1101111;
  void invoke(auto& machine){
    machine.registers[rd] = machine.ip + 4;
    machine.ip += get_immediate();
  }
};
struct JALR : format::i {
  static constexpr auto opcode = 0b1100111;
  static constexpr auto funct3 = 0;
  void invoke(auto& machine){
    machine.registers[rd] = machine.ip + 4;
    machine.ip = (machine.registers[rd] + get_immediate()) & (-1 << 1);
  }
};
struct BEQ : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto funct3 = 0b000;
  void invoke(auto& machine){
    if(machine.registers[rs1] == machine.registers[rs2])
      machine.ip += get_immediate();
  }
};
struct BNE : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto funct3 = 0b001;
  void invoke(auto& machine){
    if(machine.registers[rs1] != machine.registers[rs2])
      machine.ip += get_immediate();
  }
};
struct BLT : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto funct3 = 0b100;
  void invoke(auto& machine){
    using s = std::make_signed_t<decltype(machine.registers[0])>;
    if((s)machine.registers[rs1] < (s)machine.registers[rs2])
      machine.ip += get_immediate();
  }
};
struct BGE : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto funct3 = 0b101;
  void invoke(auto& machine){
    using s = std::make_signed_t<decltype(machine.registers[0])>;
    if((s)machine.registers[rs1] >= (s)machine.registers[rs2])
      machine.ip += get_immediate();
  }
};
struct BLTU : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto funct3 = 0b110;
  void invoke(auto& machine){
    if(machine.registers[rs1] < machine.registers[rs2])
      machine.ip += get_immediate();
  }
};
struct BGEU : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto funct3 = 0b111;
  void invoke(auto& machine){
    if(machine.registers[rs1] >= machine.registers[rs2])
      machine.ip += get_immediate();
  }
};

}
