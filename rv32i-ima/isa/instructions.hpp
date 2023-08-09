#pragma once

#include "../detail/meta.hpp"
#include "formats.hpp"
#include <arpa/nameser_compat.h>
#include <iostream>
#include <sstream>

namespace rv32i::instructions {

namespace detail {

template <class Format, auto code, auto... ls> struct make_instruction {
  template <class body> struct insn : Format {
    static constexpr auto opcode = code;
    bool dispatch() { return (ls(*(Format *)this) && ... && true); }
    void invoke(auto &&machine) { body{}(*(Format *)this, machine); }
  };
  constexpr auto operator|(auto fn) {
    return ::rv32i::detail::meta::lift_type<insn<decltype(fn)>>{};
  }
};
template <auto F, size_t x> constexpr auto make_member_cmp() {
  return [](auto &&i) { return F(i) == x; };
}
}
#pragma push_macro("MEMBER")
#pragma push_macro("INSTRUCTION")
#define MEMBER(IDEXPR, value) detail::make_member_cmp<[](auto &&i) { return ((decltype(i))i).IDEXPR; }, value>()
#define INSTRUCTION(name, ...) constexpr auto name = detail::make_instruction<__VA_ARGS__>{} | [](auto &&i, auto &&m)

constexpr std::string reginfo(auto &machine, uint32_t reg) {
  std::stringstream ss;
  ss << "R" << reg << std::hex << " 0x" << machine.registers[reg] << std::dec
     << " ";
  return ss.str();
}

INSTRUCTION(LUI, format::u, 0b011'0111) {
  m.registers[i.rd] = i.get_immediate();
#ifdef DEBUG
  std::cout << "[LUI] " << reginfo(m, i.rd) << std::endl;
#endif
};

INSTRUCTION(AUIPC, format::u, 0b0010111) {
  m.registers[i.rd] = (m.pc + i.get_immediate());
#ifdef DEBUG
  std::cout << "[AUIPC] " << reginfo(m, i.rd) << std::endl;
#endif
};

INSTRUCTION(ADD, format::r, 0b0010111, MEMBER(func3, 0b000),
            MEMBER(func7, 0b0000'000)) {
  m.registers[i.rd] = (m.registers[i.rs1] + m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[ADD] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " + " << reginfo(m, i.rs2) << std::endl;
#endif
};

INSTRUCTION(ADDI, format::i, 0b0010011, MEMBER(func3, 0b000)) {
  m.registers[i.rd] = (m.registers[i.rs1] + i.get_immediate());
#ifdef DEBUG
  std::cout << "[ADDI] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " + " << i.get_immediate() << std::endl;
#endif
};

INSTRUCTION(SLT, format::r, 0b0110'011, MEMBER(func3, 0b010),
            MEMBER(func7, 0b0000'000)) {
  static constexpr uint32_t func7 = 0b0000'000;

  m.registers[i.rd] =
      ((int32_t)m.registers[i.rs1] < (int32_t)m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[SLT] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " < " << reginfo(m, i.rs2) << std::endl;
#endif
};

INSTRUCTION(SLTI, format::i, 0b0010011, MEMBER(func3, 0b010)) {
  static constexpr uint32_t func3 = 0b010;

  m.registers[i.rd] =
      ((int32_t)m.registers[i.rs1] < (int32_t)i.get_immediate());
#ifdef DEBUG
  std::cout << "[SLTI] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " < " << i.get_immediate() << std::endl;
#endif
};

INSTRUCTION(SLTU, format::r, 0b0110'011, MEMBER(func3, 0b011),
            MEMBER(func3, 0b0000'000)) {
  static constexpr uint32_t func3 = 0b011;
  static constexpr uint32_t func7 = 0b0000'000;

  m.registers[i.rd] = (m.registers[i.rs1] < m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[SLTU] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " < " << reginfo(m, i.rs2) << std::endl;
#endif
};
INSTRUCTION(SLTIU, format::i, 0b0010011, MEMBER(func3, 0b011)) {
  static constexpr uint32_t func3 = 0b011;

  m.registers[i.rd] = (m.registers[i.rs1] < i.get_immediate());
#ifdef DEBUG
  std::cout << "[SLTIU] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " < " << i.get_immediate() << std::endl;
#endif
};

INSTRUCTION(AND, format::r, 0b0110'011, MEMBER(func3, 0b111),
            MEMBER(func3,  0b0000'000)) {
  static constexpr uint32_t func3 = 0b111;
  static constexpr uint32_t func7 = 0b0000'000;

  m.registers[i.rd] = (m.registers[i.rs1] & m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[AND] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " & " << reginfo(m, i.rs2) << std::endl;
#endif
};
INSTRUCTION(ANDI, format::i, 0b0010011, MEMBER(func3, 0b111)) {
  static constexpr uint32_t func3 = 0b111;

  m.registers[i.rd] = (m.registers[i.rs1] & i.get_immediate());
#ifdef DEBUG
  std::cout << "[ANDI] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " & " << i.get_immediate() << std::endl;
#endif
};

INSTRUCTION(OR, format::r, 0b0110'011, MEMBER(func3, 0b110)
                                           MEMBER(func7,  0b0000'000)) {
  static constexpr uint32_t func3 = 0b110;
  static constexpr uint32_t func7 = 0b0000'000;

  m.registers[i.rd] = (m.registers[i.rs1] | m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[OR] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " | " << reginfo(m, i.rs2) << std::endl;
#endif
};
INSTRUCTION(ORI, format::i, 0b0010011, MEMBER(func3, 0b110)) {
  static constexpr uint32_t func3 = 0b110;

  m.registers[i.rd] = (m.registers[i.rs1] | i.get_immediate());
#ifdef DEBUG
  std::cout << "[ORI] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " | " << i.get_immediate() << std::endl;
#endif
};

INSTRUCTION(XOR, format::r, 0b0110'011, MEMBER(func3, 0b100)
                                            MEMBER(func7, 0b0000'000)) {
  static constexpr uint32_t func3 = 0b100;
  static constexpr uint32_t func7 = 0b0000'000;

  m.registers[i.rd] = (m.registers[i.rs1] ^ m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[XOR] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " ^ " << reginfo(m, i.rs2) << std::endl;
#endif
};
INSTRUCTION(XORI, format::i, 0b0010011, MEMBER(func3, 0b100)) {
  static constexpr uint32_t func3 = 0b100;

  m.registers[i.rd] = (m.registers[i.rs1] ^ i.get_immediate());
#ifdef DEBUG
  std::cout << "[XORI] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " ^ " << i.get_immediate() << std::endl;
#endif
};

INSTRUCTION(SLL, format::r, 0b0110'011, MEMBER(func3, 0b001), MEMBER(func7, 0b0000'000)) {
  static constexpr uint32_t func3 = 0b001;
  static constexpr uint32_t func7 = 0b0000'000;

  m.registers[i.rd] = (m.registers[i.rs1] << (m.registers[i.rs2] & 0x1F));
#ifdef DEBUG
  std::cout << "[SLL] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " << " << reginfo(m, i.rs2) << std::endl;
#endif
};
INSTRUCTION(SLLI, format::i, 0b0010011, MEMBER(func3, 0b001)) {
  static constexpr uint32_t func3 = 0b001;

  m.registers[i.rd] = (m.registers[i.rs1] << (i.get_immediate() & 0x1F));
#ifdef DEBUG
  std::cout << "[SLLI] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " << " << i.get_immediate() << std::endl;
#endif
};
INSTRUCTION(SRL, format::r, 0b0110'011, MEMBER(func3, 0b101), MEMBER(func7, 0b0000'000)) {
  static constexpr uint32_t func3 = 0b101;
  static constexpr uint32_t func7 = 0b0000'000;

  m.registers[i.rd] = (m.registers[i.rs1] >> (m.registers[i.rs2] & 0x1F));
#ifdef DEBUG
  std::cout << "[SRL] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " >> " << reginfo(m, i.rs2) << std::endl;
#endif
};
INSTRUCTION(SRA, format::r, 0b0110'011, MEMBER(func3, 0b101),
            MEMBER(func7, 0b0100'000)) {
  static constexpr uint32_t func3 = 0b101;
  static constexpr uint32_t func7 = 0b0100'000;

  m.registers[i.rd] =
      ((int32_t)m.registers[i.rs1] >> (m.registers[i.rs2] & 0x1F));
#ifdef DEBUG
  std::cout << "[SRL] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " >> " << reginfo(m, i.rs2) << std::endl;
#endif
};

INSTRUCTION(SRLI_SRAI, format::i, 0b0010011, MEMBER(func3, 0b101)) {
  static constexpr uint32_t func3 = 0b101;

  if ((i.imm & 0xFE0) == 0x000) {
    m.registers[i.rd] = (m.registers[i.rs1] >> (i.imm & 0x1F));
#ifdef DEBUG
    std::cout << "[SRLI] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
              << " << " << i.get_immediate() << std::endl;
#endif
  } else if ((i.imm & 0xFE0) == 0x400) {
    m.registers[i.rd] = ((int32_t)m.registers[i.rs1] >> (i.imm & 0x1F));
#ifdef DEBUG
    std::cout << "[SRAI] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
              << " << " << i.get_immediate() << std::endl;
#endif
  } else {
    m.illegal_instruction();
  }
};

INSTRUCTION(SUB, format::r, 0b0110'011, MEMBER(func3, 0b000),
            MEMBER(func7,  0b0100'000)) {
  static constexpr uint32_t func3 = 0b000;
  static constexpr uint32_t func7 = 0b0100'000;

  m.registers[i.rd] = (m.registers[i.rs1] - m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[SUB] " << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " >> " << reginfo(m, i.rs2) << std::endl;
#endif
};

INSTRUCTION(CSRRW, format::i, 0b111'0011, MEMBER(func3, 0b001)) {
  static constexpr uint32_t func3 = 0b001;

  m.registers[i.rd] = m.csr[i.imm];
  m.csr[i.imm] = m.registers[i.rs1];
#ifdef DEBUG
  std::cout << "[CSRRW] "
            << "old: " << reginfo(m, i.rd) << "new: " << reginfo(m, i.rs1)
            << std::endl;
#endif
};
INSTRUCTION(CSRRS, format::i, 0b111'0011, MEMBER(func3, 0b010)) {
  static constexpr uint32_t func3 = 0b010;

  m.registers[i.rd] = m.csr[i.imm];
  m.csr[i.imm] |= m.registers[i.rs1];
#ifdef DEBUG
  std::cout << "[CSRRS] "
            << "old: " << reginfo(m, i.rd) << "new: " << reginfo(m, i.rs1)
            << std::endl;
#endif
};
INSTRUCTION(CSRRC, format::i, 0b111'0011, MEMBER(func3, 0b011)) {
  static constexpr uint32_t func3 = 0b011;

  m.registers[i.rd] = m.csr[i.imm];
  m.csr[i.imm] &= ~m.registers[i.rs1];
#ifdef DEBUG
  std::cout << "[CSRRC] "
            << "old: " << reginfo(m, i.rd) << "new: " << reginfo(m, i.rs1)
            << std::endl;
#endif
};
INSTRUCTION(CSRRWI, format::i, 0b111'0011, MEMBER(func3, 0b101)) {
  static constexpr uint32_t func3 = 0b101;

  m.registers[i.rd] = m.csr[i.imm];
  m.csr[i.imm] = i.rs1;
#ifdef DEBUG
  std::cout << "[CSRRWI] "
            << "old: " << reginfo(m, i.rd) << "new: " << std::hex << i.rs1
            << std::dec << std::endl;
#endif
};
INSTRUCTION(CSRRSI, format::i, 0b111'0011, MEMBER(func3, 0b110)) {
  static constexpr uint32_t func3 = 0b110;

  m.registers[i.rd] = m.csr[i.imm];
  m.csr[i.imm] |= i.rs1;
#ifdef DEBUG
  std::cout << "[CSRRSI] "
            << "old: " << reginfo(m, i.rd) << "new: " << std::hex << i.rs1
            << std::dec << std::endl;
#endif
};
INSTRUCTION(CSRRCI, format::i, 0b111'0011, MEMBER(func3, 0b111)) {
  static constexpr uint32_t func3 = 0b111;

  m.registers[i.rd] = m.csr[i.imm];
  m.csr[i.imm] &= ~i.rs1;
#ifdef DEBUG
  std::cout << "[CSRRCI] "
            << "old: " << reginfo(m, i.rd) << "new: " << std::hex << i.rs1
            << std::dec << std::endl;
#endif
};

INSTRUCTION(JAL, format::j, 0b1101111, MEMBER(func3, 0b000)) {

  m.registers[i.rd] = m.pc + 4;
  m.pc += i.get_immediate() - 4;
#ifdef DEBUG
  std::cout << "[JAL] " << reginfo(m, i.rd) << "PC," << std::hex << m.pc
            << std::dec << std::endl;
#endif
};

INSTRUCTION(JALR, format::i, 0b1100111, MEMBER(func3, 0b000)) {
  static constexpr auto func3 = 0;

  uint32_t tmp = m.pc + 4;
  m.pc = (m.registers[i.rs1] + (i.get_immediate()) & (-1 << 1)) - 4;
  m.registers[i.rd] = tmp;
#ifdef DEBUG
  std::cout << "[JALR ]" << reginfo(m, i.rd) << reginfo(m, rs1) << "PC,"
            << std::hex << m.pc + 4 << std::dec << std::endl;
#endif
};
INSTRUCTION(BEQ, format::b, 0b1100011, MEMBER(func3, 0b000)) {
  static constexpr auto func3 = 0b000;

  if (m.registers[i.rs1] == m.registers[i.rs2])
    m.pc += i.get_immediate() - 4;
#ifdef DEBUG
  std::cout << "[BEQ ]" << reginfo(m, i.rs1) << " == " << reginfo(m, i.rs2)
            << "PC," << std::hex << m.pc << std::dec << std::endl;
#endif
};
INSTRUCTION(BNE, format::b, 0b1100011, MEMBER(func3, 0b001)) {
  static constexpr auto func3 = 0b001;

  if (m.registers[i.rs1] != m.registers[i.rs2])
    m.pc += i.get_immediate() - 4;
#ifdef DEBUG
  std::cout << "[BNE ]" << reginfo(m, i.rs1) << " != " << reginfo(m, i.rs2)
            << "PC: " << std::hex << m.pc << std::dec << std::endl;
#endif
};
INSTRUCTION(BLT, format::b, 0b1100011, MEMBER(func3, 0b100)) {
  static constexpr auto func3 = 0b100;

  using s = std::make_signed_t<typename decltype(m.registers)::value_type>;
  if ((s)m.registers[i.rs1] < (s)m.registers[i.rs2])
    m.pc += i.get_immediate() - 4;
#ifdef DEBUG
  std::cout << "[BLT ]" << reginfo(m, i.rs1) << " != " << reginfo(m, i.rs2)
            << "PC," << std::hex << m.pc << std::dec << std::endl;
#endif
};
INSTRUCTION(BGE, format::b, 0b1100011, MEMBER(func3, 0b101)) {
  static constexpr auto func3 = 0b101;

  using s = std::make_signed_t<typename decltype(m.registers)::value_type>;
  if ((s)m.registers[i.rs1] >= (s)m.registers[i.rs2])
    m.pc += i.get_immediate() - 4;
#ifdef DEBUG
  std::cout << "[BGE ]" << reginfo(m, i.rs1) << " != " << reginfo(m, i.rs2)
            << "PC," << std::hex << m.pc << std::dec << std::endl;
#endif
};
INSTRUCTION(BLTU, format::b, 0b1100011, MEMBER(func3, 0b110)) {
  static constexpr auto func3 = 0b110;

  if (m.registers[i.rs1] < m.registers[i.rs2])
    m.pc += i.get_immediate() - 4;
#ifdef DEBUG
  std::cout << "[BLTU ]" << reginfo(m, i.rs1) << " != " << reginfo(m, i.rs2)
            << "PC: " << std::hex << m.pc << std::dec << std::endl;
#endif
};
INSTRUCTION(BGEU, format::b, 0b1100011, MEMBER(func3, 0b111)) {
  static constexpr auto func3 = 0b111;

  if (m.registers[i.rs1] >= m.registers[i.rs2])
    m.pc += i.get_immediate() - 4;

#ifdef DEBUG
  std::cout << "[BGEU ]" << reginfo(m, i.rs1) << " != " << reginfo(m, i.rs2)
            << "PC: " << std::hex << m.pc << std::dec << std::endl;
#endif
};

INSTRUCTION(LB, format::i, 0b000'0011, MEMBER(func3, 0b000)) {
  static constexpr auto func3 = 0b000;

  auto addr = m.registers[i.rs1] + i.get_immediate();
  m.registers[i.rd] = (int32_t)m.template load<int8_t>(addr);
#ifdef DEBUG
  std::cout << "[LB ]" << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " + " << get_immediate() << std::endl;
#endif
};
INSTRUCTION(LH, format::i, 0b000'0011, MEMBER(func3, 0b001)) {
  static constexpr auto func3 = 0b001;

  auto addr = m.registers[i.rs1] + i.get_immediate();
  m.registers[i.rd] = (int32_t)m.template load<int16_t>(addr);
#ifdef DEBUG
  std::cout << "[LH ]" << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " + " << get_immediate() << std::endl;
#endif
};
INSTRUCTION(LW, format::i, 0b000'0011, MEMBER(func3, 0b010)) {
  static constexpr auto func3 = 0b010;

  auto addr = m.registers[i.rs1] + i.get_immediate();
  m.registers[i.rd] = (uint32_t)m.template load<uint32_t>(addr);
#ifdef DEBUG
  std::cout << "[LW ]" << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " + " << get_immediate() << std::endl;
#endif
};
INSTRUCTION(LBU, format::i, 0b000'0011, MEMBER(func3, 0b100)) {
  static constexpr auto func3 = 0b100;

  auto addr = m.registers[i.rs1] + i.get_immediate();
  m.registers[i.rd] = (uint32_t)m.template load<uint8_t>(addr);
#ifdef DEBUG
  std::cout << "[LBU ]" << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " + " << get_immediate() << std::endl;
#endif
};
INSTRUCTION(LHU, format::i, 0b000'0011, MEMBER(func3, 0b101)) {
  static constexpr auto func3 = 0b101;

  auto addr = m.registers[i.rs1] + i.get_immediate();
  m.registers[i.rd] = (uint32_t)m.template load<uint16_t>(addr);
#ifdef DEBUG
  std::cout << "[LHU ]" << reginfo(m, i.rd) << " = " << reginfo(m, i.rs1)
            << " + " << get_immediate() << std::endl;
#endif
};
INSTRUCTION(SB, format::s, 0b010'0011, MEMBER(func3, 0b000)) {
  static constexpr auto func3 = 0b000;

  auto addr = m.registers[i.rs1] + i.get_immediate();
  m.store(addr, (uint8_t)m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[SB ]"
            << " = " << reginfo(m, s::rs1) << " + " << get_immediate()
            << " <== " << reginfo(m, s::rs2) << std::endl;
#endif
};
INSTRUCTION(SH, format::s, 0b010'0011, MEMBER(func3, 0b001)) {
  static constexpr auto func3 = 0b001;

  auto addr = m.registers[i.rs1] + i.get_immediate();
  m.store(addr, (uint16_t)m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[SB ]"
            << " = " << reginfo(m, s::rs1) << " + " << get_immediate()
            << " <== " << reginfo(m, s::rs2) << std::endl;
#endif
};
INSTRUCTION(SW, format::s, 0b010'0011, MEMBER(func3, 0b010)) {
  static constexpr auto func3 = 0b010;

  auto addr = m.registers[i.rs1] + i.get_immediate();
  m.store(addr, (uint32_t)m.registers[i.rs2]);
#ifdef DEBUG
  std::cout << "[SB ]"
            << " = " << reginfo(m, s::rs1) << " + " << get_immediate()
            << " <== " << reginfo(m, s::rs2) << std::endl;
#endif
};

INSTRUCTION(ECALL, format::i, 0b111'0011, MEMBER(func3, 0b000)) {
  static constexpr auto func3 = 0b000;

#ifdef DEBUG
  std::cout << "[ECALL] idk" << std::endl;
#endif
  if (i.imm != 0)
    return;
  if (m.registers[3] == 1) {
    // std::cerr << "SUCCESS" << std::endl;
    throw std::runtime_error("SUCCESS");
  } else {
    std::cerr << "FAIL " << reginfo(m, 3) << std::endl;
    throw std::runtime_error("FAIL иди дебаж");
  }
};
INSTRUCTION(FENCE, format::i, 0b000'1111, MEMBER(func3, 0b000)) {
  static constexpr auto func3 = 0b000;

#ifdef DEBUG
  std::cout << "[FENCE] idk" << std::endl;
#endif
};
INSTRUCTION(FENCE_I, format::i, 0b000'1111, MEMBER(func3, 0b001)) {
  static constexpr auto func3 = 0b001;

#ifdef DEBUG
  std::cout << "[FENCE_I] idk" << std::endl;
#endif
};
#pragma pop_macro("INSTRUCTION")
#pragma pop_macro("MEMBER")

auto rv32i_isa = ::rv32i::detail::meta::tuple(LUI, AUIPC,
                                              ADDI, ADD, SLT, SLTIU, SLTU, ANDI, AND, ORI, OR, XORI, XOR, SLLI, SLL, SRLI_SRAI, SRL, SRA, SUB, JAL, JALR, BEQ, BNE, BGE, BLT, BGEU, BLTU, SLTI,
                                              LB, LH, LW, LBU, LHU,
                                              SB, SH, SW,
                                              CSRRW, CSRRS, CSRRC,
                                              CSRRWI, CSRRSI, CSRRCI,
                                              ECALL, FENCE, FENCE_I);

}