#pragma once

#include <iostream>
#include "formats.hpp"

namespace rv32i::instructions{
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
struct add : format::i {
  static constexpr uint32_t opcode = 46;
  static constexpr uint32_t func3 = 0;
  void invoke(auto&){
    std::cout << "add\n";
  }
};

}
