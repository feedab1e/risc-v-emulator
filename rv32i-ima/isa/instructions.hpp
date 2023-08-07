#pragma once

#include "formats.hpp"
#include <iostream>
#include <sstream>

namespace rv32i::instructions{

constexpr std::string reginfo (auto& machine, uint32_t reg){
  std::stringstream ss;
  ss << "R" << reg << std::hex << " 0x" << machine.registers[reg] << std::dec << " ";
  return ss.str();
}

struct LUI: format::u {
  static constexpr uint32_t opcode = 0b0110111;
  void invoke(auto &machine){
    //machine
    machine.registers[u::rd] = u::get_immediate();

#ifdef DEBUG
    std::cout<<"[LUI] " << reginfo(machine, u::rd) << std::endl;
#endif
  }
};
struct AUIPC: format::u {
  static constexpr uint32_t opcode = 0b0010111;
  void invoke(auto &machine){

    machine.registers[u::rd] = (machine.pc +u::get_immediate());
#ifdef DEBUG
    std::cout<<"[AUIPC] " << reginfo(machine, u::rd) << std::endl;
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
    std::cout << "[ADD] " << reginfo(machine, r::rd) << " = " << reginfo(machine, r::rs1) << " + "
              << reginfo(machine, r::rs2)
              << std::endl;
#endif
  }
};

struct ADDI: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b000;
  void invoke(auto& machine){
    machine.registers[i::rd] = (machine.registers[i::rs1] + i::get_immediate());
#ifdef DEBUG
    std::cout << "[ADDI] " << reginfo(machine, i::rd) << " = " << reginfo(machine, i::rs1) << " + "
              << i::get_immediate()
              << std::endl;
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
    std::cout<<"[SLT] " << reginfo(machine, r::rd) << " = "
        << reginfo(machine, r::rs1) << " < " << reginfo(machine, r::rs2)
        << std::endl;
#endif
  }
};

struct SLTI: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b010;
  void invoke(auto& machine){
    machine.registers[i::rd] = ((int32_t)machine.registers[i::rs1] < (int32_t)i::get_immediate());
#ifdef DEBUG
    std::cout<<"[SLTI] " <<reginfo(machine, i::rd) << " = "
              << reginfo(machine, i::rs1) << " < "
              << i::get_immediate()
              << std::endl;
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
    std::cout<<"[SLTU] " << reginfo(machine, r::rd) << " = "
              << reginfo(machine, r::rs1) << " < " << reginfo(machine, r::rs2)
              << std::endl;
#endif
  }
};
struct SLTIU: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b011;
  void invoke(auto& machine){
    machine.registers[i::rd] = (machine.registers[i::rs1] < i::get_immediate());
#ifdef DEBUG
    std::cout<<"[SLTIU] " <<reginfo(machine, i::rd) << " = "
              << reginfo(machine, i::rs1) << " < "
              << i::get_immediate()
              << std::endl;
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
    std::cout << "[AND] " << reginfo(machine, r::rd) << " = "
              << reginfo(machine, r::rs1) << " & "
              << reginfo(machine, r::rs2) << std::endl;
#endif
  }
};
struct ANDI: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b111;
  void invoke(auto& machine){
    machine.registers[i::rd] = (machine.registers[i::rs1] & i::get_immediate());
#ifdef DEBUG
    std::cout << "[ANDI] " << reginfo(machine, i::rd) << " = "
              << reginfo(machine, i::rs1) << " & "
              << i::get_immediate() << std::endl;
#endif
  }
};

struct OR : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b110;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] | machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout << "[OR] " << reginfo(machine, r::rd) << " = "
              << reginfo(machine, r::rs1) << " | "
              << reginfo(machine, r::rs2) << std::endl;
#endif
  }
};
struct ORI: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b110;
  void invoke(auto& machine){
    machine.registers[i::rd] = (machine.registers[i::rs1] | i::get_immediate());
#ifdef DEBUG
    std::cout << "[ORI] " << reginfo(machine, i::rd) << " = "
              << reginfo(machine, i::rs1) << " | "
              << i::get_immediate() << std::endl;
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
    std::cout << "[XOR] " << reginfo(machine, r::rd) << " = "
              << reginfo(machine, r::rs1) << " ^ "
              << reginfo(machine, r::rs2) << std::endl;
#endif
  }
};
struct XORI: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b100;
  void invoke(auto& machine){
    machine.registers[i::rd] = (machine.registers[i::rs1] ^ i::get_immediate());
#ifdef DEBUG
    std::cout << "[XORI] " << reginfo(machine, i::rd) << " = "
              << reginfo(machine, i::rs1) << " ^ "
              << i::get_immediate() << std::endl;
#endif
  }
};


struct SLL : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b001;
  static constexpr uint32_t func7  = 0b0000'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] << (machine.registers[r::rs2] & 0x1F));
#ifdef DEBUG
    std::cout << "[SLL] " << reginfo(machine, r::rd) << " = "
              << reginfo(machine, r::rs1) << " << "
              << reginfo(machine, r::rs2) << std::endl;
#endif
  }
};
struct SLLI: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b001;
  void invoke(auto& machine){
    machine.registers[i::rd] = (machine.registers[i::rs1] << (i::get_immediate() & 0x1F));
#ifdef DEBUG
    std::cout << "[SLLI] " << reginfo(machine, i::rd) << " = "
              << reginfo(machine, i::rs1) << " << "
              << i::get_immediate() << std::endl;
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
    std::cout << "[SRL] " << reginfo(machine, r::rd) << " = "
              << reginfo(machine, r::rs1) << " >> "
              << reginfo(machine, r::rs2) << std::endl;
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
    std::cout << "[SRL] " << reginfo(machine, r::rd) << " = "
              << reginfo(machine, r::rs1) << " >> "
              << reginfo(machine, r::rs2) << std::endl;
#endif
  }
};

struct SRLI_SRAI: format::i{
  static constexpr uint32_t opcode = 0b0010011;
  static constexpr uint32_t func3 =  0b101;
  void invoke(auto& machine){
    if((i::imm&0xFE0) == 0x000){
      machine.registers[i::rd] = (machine.registers[i::rs1] >> (i::imm & 0x1F));
#ifdef DEBUG
      std::cout << "[SRLI] " << reginfo(machine, i::rd) << " = "
                << reginfo(machine, i::rs1) << " << "
                << i::get_immediate() << std::endl;
#endif
    }else if((i::imm&0xFE0) == 0x400){
      machine.registers[i::rd] = ((int32_t)machine.registers[i::rs1] >>(i::imm & 0x1F));
#ifdef DEBUG
      std::cout << "[SRAI] " << reginfo(machine, i::rd) << " = "
                << reginfo(machine, i::rs1) << " << "
                << i::get_immediate() << std::endl;
#endif
    }else{
      machine.illegal_instruction();
    }
  }
};

struct SUB : format::r {
  static constexpr uint32_t opcode = 0b0110'011;
  static constexpr uint32_t func3  = 0b000;
  static constexpr uint32_t func7  = 0b0100'000;
  void invoke(auto& machine){
    machine.registers[r::rd] = (machine.registers[r::rs1] - machine.registers[r::rs2]);
#ifdef DEBUG
    std::cout << "[SUB] " << reginfo(machine, r::rd) << " = "
              << reginfo(machine, r::rs1) << " >> "
              << reginfo(machine, r::rs2) << std::endl;
#endif
  }
};

struct CSRRW: format::i{
  static constexpr uint32_t opcode = 0b111'0011;
  static constexpr uint32_t func3 =  0b001;
  void invoke(auto& machine){
    machine.registers[i::rd] = machine.csr[i::imm];
    machine.csr[i::imm] = machine.registers[i::rs1];
#ifdef DEBUG
    std::cout << "[CSRRW] "
              << "old: " << reginfo(machine, i::rd)
              << "new: " << reginfo(machine, i::rs1)
              << std::endl;
#endif
  }
};
struct CSRRS: format::i {
  static constexpr uint32_t opcode = 0b111'0011;
  static constexpr uint32_t func3 =  0b010;
  void invoke(auto& machine){
    machine.registers[i::rd] = machine.csr[i::imm];
    machine.csr[i::imm] |= machine.registers[i::rs1];
#ifdef DEBUG
    std::cout << "[CSRRS] "
              << "old: " << reginfo(machine, i::rd)
              << "new: " << reginfo(machine, i::rs1)
              << std::endl;
#endif
  }
};
struct CSRRC: format::i {
  static constexpr uint32_t opcode = 0b111'0011;
  static constexpr uint32_t func3 =  0b011;
  void invoke(auto& machine){
    machine.registers[i::rd] = machine.csr[i::imm];
    machine.csr[i::imm] &= ~machine.registers[i::rs1];
#ifdef DEBUG
    std::cout << "[CSRRC] "
              << "old: " << reginfo(machine, i::rd)
              << "new: " << reginfo(machine, i::rs1)
              << std::endl;
#endif
  }
};
struct CSRRWI: format::i {
  static constexpr uint32_t opcode = 0b111'0011;
  static constexpr uint32_t func3 =  0b101;
  void invoke(auto& machine){
    machine.registers[i::rd] = machine.csr[i::imm];
    machine.csr[i::imm] = i::rs1;
#ifdef DEBUG
    std::cout << "[CSRRWI] "
              << "old: " << reginfo(machine, i::rd)
              << "new: " << std::hex << i::rs1 << std::dec
              << std::endl;
#endif
  }
};
struct CSRRSI: format::i {
  static constexpr uint32_t opcode = 0b111'0011;
  static constexpr uint32_t func3 =  0b110;
  void invoke(auto& machine){
    machine.registers[i::rd] = machine.csr[i::imm];
    machine.csr[i::imm] |= i::rs1;
#ifdef DEBUG
    std::cout << "[CSRRSI] "
              << "old: " << reginfo(machine, i::rd)
              << "new: " << std::hex << i::rs1 << std::dec
              << std::endl;
#endif
  }
};
struct CSRRCI: format::i {
  static constexpr uint32_t opcode = 0b111'0011;
  static constexpr uint32_t func3 =  0b111;
  void invoke(auto& machine){
    machine.registers[i::rd] = machine.csr[i::imm];
    machine.csr[i::imm] &= ~i::rs1;
#ifdef DEBUG
    std::cout << "[CSRRCI] "
              << "old: " << reginfo(machine, i::rd)
              << "new: " << std::hex << i::rs1 << std::dec
              << std::endl;
#endif
  }
};


struct JAL : format::j {
  static constexpr auto opcode = 0b1101111;
  void invoke(auto& machine){
    machine.registers[rd] = machine.pc + 4;
    machine.pc += get_immediate() - 4;
#ifdef DEBUG
    std::cout << "[JAL] " << reginfo(machine, j::rd)
              << "PC: " << std::hex << machine.pc << std::dec
              << std::endl;
#endif
  }
};

struct JALR : format::i {
  static constexpr auto opcode = 0b1100111;
  static constexpr auto func3 = 0;
  void invoke(auto& machine){
    machine.registers[rd] = machine.pc + 4;
    machine.pc = (machine.registers[rs1] + (get_immediate()) & (-1 << 1)) - 4;
#ifdef DEBUG
    std::cout << "[JALR ]" << reginfo(machine, i::rd)
              << "PC: " << std::hex << machine.pc << std::dec
              << std::endl;
#endif
  }
};
struct BEQ : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto func3 = 0b000;
  void invoke(auto& machine){
    if(machine.registers[rs1] == machine.registers[rs2])
      machine.pc += get_immediate() - 4;
#ifdef DEBUG
    std::cout << "[BEQ ]" << reginfo(machine, b::rs1)
              << " == " << reginfo(machine, b::rs2)
              << "PC: " << std::hex << machine.pc << std::dec
              << std::endl;
#endif
  }
};
struct BNE : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto func3 = 0b001;
  void invoke(auto& machine){
    if(machine.registers[rs1] != machine.registers[rs2])
      machine.pc += get_immediate() - 4;
#ifdef DEBUG
    std::cout << "[BNE ]" << reginfo(machine, b::rs1)
              << " != " << reginfo(machine, b::rs2)
              << "PC: " << std::hex << machine.pc << std::dec
              << std::endl;
#endif
  }
};
struct BLT : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto func3 = 0b100;
  void invoke(auto& machine){
    using s = std::make_signed_t<typename decltype(machine.registers)::value_type>;
    if((s)machine.registers[rs1] < (s)machine.registers[rs2])
      machine.pc += get_immediate() - 4;
#ifdef DEBUG
    std::cout << "[BLT ]" << reginfo(machine, b::rs1)
              << " != " << reginfo(machine, b::rs2)
              << "PC: " << std::hex << machine.pc << std::dec
              << std::endl;
#endif
  }
};
struct BGE : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto func3 = 0b101;
  void invoke(auto& machine){
    using s = std::make_signed_t<typename decltype(machine.registers)::value_type>;
    if((s)machine.registers[rs1] >= (s)machine.registers[rs2])
      machine.pc += get_immediate() - 4;
#ifdef DEBUG
    std::cout << "[BGE ]" << reginfo(machine, b::rs1)
              << " != " << reginfo(machine, b::rs2)
              << "PC: " << std::hex << machine.pc << std::dec
              << std::endl;
#endif
  }
};
struct BLTU : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto func3 = 0b110;
  void invoke(auto& machine){
    if(machine.registers[rs1] < machine.registers[rs2])
      machine.pc += get_immediate() - 4;
#ifdef DEBUG
    std::cout << "[BLTU ]" << reginfo(machine, b::rs1)
              << " != " << reginfo(machine, b::rs2)
              << "PC: " << std::hex << machine.pc << std::dec
              << std::endl;
#endif
  }
};
struct BGEU : format::b {
  static constexpr auto opcode = 0b1100011;
  static constexpr auto func3 = 0b111;
  void invoke(auto& machine){
    if(machine.registers[rs1] >= machine.registers[rs2])
      machine.pc += get_immediate() - 4;

#ifdef DEBUG
    std::cout << "[BGEU ]" << reginfo(machine, b::rs1)
              << " != " << reginfo(machine, b::rs2)
              << "PC: " << std::hex << machine.pc << std::dec
              << std::endl;
#endif
  }
};

struct LB : format::i {
  static constexpr auto opcode = 0b000'0011;
  static constexpr auto func3 = 0b000;
  void invoke(auto& machine){
    auto addr = machine.registers[rs1] + get_immediate();
    machine.registers[rd] = (int32_t)machine.template load<int8_t>(addr);
#ifdef DEBUG
    std::cout << "[LB ]" << reginfo(machine, i::rd)
              << " = " << reginfo(machine, i::rs1)
              << " + " << get_immediate()
              << std::endl;
#endif
  }
};
struct LH : format::i {
  static constexpr auto opcode = 0b000'0011;
  static constexpr auto func3 = 0b001;
  void invoke(auto& machine){
    auto addr = machine.registers[rs1] + get_immediate();
    machine.registers[rd] = (int32_t)machine.template load<int16_t>(addr);
#ifdef DEBUG
    std::cout << "[LH ]" << reginfo(machine, i::rd)
              << " = " << reginfo(machine, i::rs1)
              << " + " << get_immediate()
              << std::endl;
#endif
  }
};
struct LW : format::i {
  static constexpr auto opcode = 0b000'0011;
  static constexpr auto func3 = 0b010;
  void invoke(auto& machine){
    auto addr = machine.registers[rs1] + get_immediate();
    machine.registers[rd] = (uint32_t)machine.template load<uint32_t>(addr);
#ifdef DEBUG
    std::cout << "[LW ]" << reginfo(machine, i::rd)
              << " = " << reginfo(machine, i::rs1)
              << " + " << get_immediate()
              << std::endl;
#endif
  }
};
struct LBU : format::i {
  static constexpr auto opcode = 0b000'0011;
  static constexpr auto func3 = 0b100;
  void invoke(auto& machine){
    auto addr = machine.registers[rs1] + get_immediate();
    machine.registers[rd] = (uint32_t)machine.template load<uint8_t>(addr);
#ifdef DEBUG
    std::cout << "[LBU ]" << reginfo(machine, i::rd)
              << " = " << reginfo(machine, i::rs1)
              << " + " << get_immediate()
              << std::endl;
#endif
  }
};
struct LHU : format::i {
  static constexpr auto opcode = 0b000'0011;
  static constexpr auto func3 = 0b101;
  void invoke(auto& machine){
    auto addr = machine.registers[rs1] + get_immediate();
    machine.registers[rd] = (uint32_t)machine.template load<uint16_t>(addr);
#ifdef DEBUG
    std::cout << "[LHU ]" << reginfo(machine, i::rd)
              << " = " << reginfo(machine, i::rs1)
              << " + " << get_immediate()
              << std::endl;
#endif
  }
};
struct SB : format::s {
  static constexpr auto opcode = 0b010'0011;
  static constexpr auto func3 = 0b000;
  void invoke(auto& machine){
    auto addr = machine.registers[rs1] + get_immediate();
    machine.store(addr, (uint8_t)machine.registers[rs2]);
#ifdef DEBUG
    std::cout << "[SB ]"
              << " = " << reginfo(machine, s::rs1)
              << " + " << get_immediate()
              << " <== " << reginfo(machine, s::rs2)
              << std::endl;
#endif
  }
};
struct SH : format::s {
  static constexpr auto opcode = 0b010'0011;
  static constexpr auto func3 = 0b001;
  void invoke(auto& machine){
    auto addr = machine.registers[rs1] + get_immediate();
    machine.store(addr, (uint16_t)machine.registers[rs2]);
#ifdef DEBUG
    std::cout << "[SB ]"
              << " = " << reginfo(machine, s::rs1)
              << " + " << get_immediate()
              << " <== " << reginfo(machine, s::rs2)
              << std::endl;
#endif
  }
};
struct SW : format::s {
  static constexpr auto opcode = 0b010'0011;
  static constexpr auto func3 = 0b010;
  void invoke(auto& machine){
    auto addr = machine.registers[rs1] + get_immediate();
    machine.store(addr, (uint32_t)machine.registers[rs2]);
#ifdef DEBUG
    std::cout << "[SB ]"
              << " = " << reginfo(machine, s::rs1)
              << " + " << get_immediate()
              << " <== " << reginfo(machine, s::rs2)
              << std::endl;
#endif
  }
};

struct ECALL : format::i {
  static constexpr auto opcode = 0b111'0011;
  static constexpr auto func3 = 0b000;
  void invoke(auto& machine){
#ifdef DEBUG
    std::cout << "[ECALL] idk" << std::endl;
#endif
    if(i::imm!=0)
      return;
    if(machine.registers[3] == 1){
      std::cerr << "SUCCESS" << std::endl;
      throw std::runtime_error("SUCCESS");
    } else{
      std::cerr << "FAIL " << reginfo(machine, 3) << std::endl;
      throw std::runtime_error("FAIL иди дебаж");
    }

  }
};
struct FENCE : format::i {
  static constexpr auto opcode = 0b000'1111;
  static constexpr auto func3 = 0b000;
  void invoke(auto& machine){
#ifdef DEBUG
    std::cout << "[FENCE] idk" << std::endl;
#endif
  }
};

}
