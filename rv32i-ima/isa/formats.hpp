#pragma once

#include <climits>
#include <cstdint>
#include <bit>
#include "../detail/meta.hpp"

namespace rv32i{
namespace detail{
template<typename T>
class bitfield{
    T val;
public:
     inline T operator[](const int start, const int end) const{
        T mask = ((1u << (end + 1 - start)) - 1) << start;
        return (val & mask) >> start;
    }
    inline bool operator[](const int i) const{
        return (val & (1<<i));
    }
    inline T fillbit(const int index, const int len){
        return (val & (1 << index) ? (-1<< (sizeof(T)* CHAR_BIT - len)) : 0);
    }
};
}

namespace format{
struct r {
    uint32_t opcode: 7;
    uint32_t rd: 5;
    uint32_t func3: 3;
    uint32_t rs1: 5;
    uint32_t rs2: 5;
    uint32_t func7: 7;
    [[nodiscard]] uint32_t get_immediate() const{
        auto tmp = std::bit_cast<detail::bitfield<uint32_t>>(*this);
        return tmp[7, 11] | tmp[25, 30] << 5 | tmp.fillbit(31, 21);
    }
    template<class I, class... Is>
    void dispatch(auto& machine){
        if((I::func3) == this->func3 && (I::func7) == this->func7)
          std::bit_cast<I>(*this).invoke(machine);
        else if constexpr(sizeof...(Is))
          dispatch<Is...>(machine);
        else
          machine.illegal_instruction();
    }
};
struct i {
    uint32_t opcode: 7;         // [6 : 0]
    uint32_t rd: 5;             // [11: 7]
    uint32_t func3: 3;          // [14:12]
    uint32_t rs1: 5;            // [19:15]
    uint32_t imm: 12;           // [31:20]
    [[nodiscard]] uint32_t get_immediate() const{
        auto tmp = std::bit_cast<detail::bitfield<uint32_t>>(*this);
        return tmp[20, 30] | tmp.fillbit(31, 21);
    }
    template<class I, class... Is>
    void dispatch(auto& machine){
      if((I::func3) == this->func3)
        std::bit_cast<I>(*this).invoke(machine);
      else if constexpr(sizeof...(Is))
        dispatch<Is...>(machine);
      else
          machine.illegal_instruction();
    }
};
struct s {
    uint32_t opcode: 7;         // [6 : 0]
    uint32_t imm_5: 5;          // [11: 7]
    uint32_t func3: 3;          // [14:12]
    uint32_t rs1: 5;            // [19:15]
    uint32_t rs2: 5;            // [24:20]
    uint32_t imm_7: 7;          // [31:25]
    [[nodiscard]] uint32_t get_immediate() const{
        auto tmp = std::bit_cast<detail::bitfield<uint32_t>>(*this);
        return tmp[7, 11] | tmp[25, 30] << 5 | tmp.fillbit(31, 21);
    }
    template<class I, class... Is>
    void dispatch(auto& machine){
      if((I::func3) == this->func3)
        std::bit_cast<I>(*this).invoke(machine);
      else if constexpr(sizeof...(Is))
        dispatch<Is...>(machine);
      else
          machine.illegal_instruction();
    }
};
struct u {
    uint32_t opcode: 7;       // [6 : 0]
    uint32_t rd: 5;           // [11: 8]
    uint32_t imm: 20;         // [31:12]
    [[nodiscard]] uint32_t get_immediate() const{
        return 0 | imm << 12;
    }
    template <class I>
    void dispatch(auto & machine){
        std::bit_cast<I>(*this).invoke(machine);
    }
};
struct b{
    uint32_t opcode: 7;       // [6 : 0]
    uint32_t imm_5: 5;        // [11: 7]
    uint32_t func3: 3;        // [14:12]
    uint32_t rs1: 5;          // [19:15]
    uint32_t rs2: 5;          // [24:20]
    uint32_t imm_7: 7;        // [31:25]
    [[nodiscard]] uint32_t get_immediate() const{
        auto tmp = std::bit_cast<detail::bitfield<uint32_t>>(*this);
        return 0 | tmp[8, 11] << 1 | tmp[25, 30] << 5 | tmp[7] << 11 | tmp.fillbit(31, 20);
    }
    template<class I, class... Is>
    void dispatch(auto& machine){
        if((I::func3) == this->func3)
          std::bit_cast<I>(*this).invoke(machine);
        else if constexpr(sizeof...(Is))
          dispatch<Is...>(machine);
        else
          machine.illegal_instruction();
    }
};
struct j {
    uint32_t opcode: 7;      // [6 : 0]
    uint32_t rd: 5;          // [11: 8]
    uint32_t imm: 20;        // [31:12]
    [[nodiscard]] uint32_t get_immediate() const{
        auto tmp = std::bit_cast<detail::bitfield<uint32_t>>(*this);
        return 0 | tmp[21, 24] << 1 | tmp[25, 30] << 5 | tmp[20] << 11 | tmp[12, 19] << 12 | tmp.fillbit(31, 12);
    }
    template <class I>
    void dispatch(auto & machine){
        std::bit_cast<I>(*this).invoke(machine);
    }
};

auto rv32i_default_formats = ::rv32i::detail::meta::tuple(
  ::rv32i::detail::meta::lift_type<i>{},
  ::rv32i::detail::meta::lift_type<j>{},
  ::rv32i::detail::meta::lift_type<b>{},
  ::rv32i::detail::meta::lift_type<r>{},
  ::rv32i::detail::meta::lift_type<s>{},
  ::rv32i::detail::meta::lift_type<u>{});

}
static_assert(sizeof(format::r) == 4);
static_assert(sizeof(format::i) == 4);
static_assert(sizeof(format::s) == 4);
static_assert(sizeof(format::u) == 4);
static_assert(sizeof(format::b) == 4);
static_assert(sizeof(format::j) == 4);
}
