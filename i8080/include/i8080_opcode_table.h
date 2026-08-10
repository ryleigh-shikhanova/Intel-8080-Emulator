#ifndef I8080_OPCODE_TABLE_H
#define I8080_OPCODE_TABLE_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

struct OpcodeInfo {
    std::uint8_t hex;
    std::string_view opcode;
    std::uint8_t t_states;
    std::uint8_t t_states_taken;

    [[nodiscard]] constexpr bool hasConditionalTiming() const noexcept {
        return t_states != t_states_taken;
    }
};

// For conditional CALL/RET instructions, t_states is the not-taken time and
// t_states_taken is the taken time. For every other instruction they match.
// Asterisks identify undocumented 8080 aliases.
inline constexpr std::array<OpcodeInfo, 256> kOpcodeTable{{
    // 0x00
    {0x00, "NOP",       4,  4}, {0x01, "LXI B,d16", 10, 10},
    {0x02, "STAX B",    7,  7}, {0x03, "INX B",     5,  5},
    {0x04, "INR B",     5,  5}, {0x05, "DCR B",     5,  5},
    {0x06, "MVI B,d8",  7,  7}, {0x07, "RLC",       4,  4},
    {0x08, "NOP*",      4,  4}, {0x09, "DAD B",    10, 10},
    {0x0A, "LDAX B",    7,  7}, {0x0B, "DCX B",     5,  5},
    {0x0C, "INR C",     5,  5}, {0x0D, "DCR C",     5,  5},
    {0x0E, "MVI C,d8",  7,  7}, {0x0F, "RRC",       4,  4},

    // 0x10
    {0x10, "NOP*",      4,  4}, {0x11, "LXI D,d16", 10, 10},
    {0x12, "STAX D",    7,  7}, {0x13, "INX D",     5,  5},
    {0x14, "INR D",     5,  5}, {0x15, "DCR D",     5,  5},
    {0x16, "MVI D,d8",  7,  7}, {0x17, "RAL",       4,  4},
    {0x18, "NOP*",      4,  4}, {0x19, "DAD D",    10, 10},
    {0x1A, "LDAX D",    7,  7}, {0x1B, "DCX D",     5,  5},
    {0x1C, "INR E",     5,  5}, {0x1D, "DCR E",     5,  5},
    {0x1E, "MVI E,d8",  7,  7}, {0x1F, "RAR",       4,  4},

    // 0x20
    {0x20, "NOP*",      4,  4}, {0x21, "LXI H,d16", 10, 10},
    {0x22, "SHLD a16", 16, 16}, {0x23, "INX H",     5,  5},
    {0x24, "INR H",     5,  5}, {0x25, "DCR H",     5,  5},
    {0x26, "MVI H,d8",  7,  7}, {0x27, "DAA",       4,  4},
    {0x28, "NOP*",      4,  4}, {0x29, "DAD H",    10, 10},
    {0x2A, "LHLD a16", 16, 16}, {0x2B, "DCX H",     5,  5},
    {0x2C, "INR L",     5,  5}, {0x2D, "DCR L",     5,  5},
    {0x2E, "MVI L,d8",  7,  7}, {0x2F, "CMA",       4,  4},

    // 0x30
    {0x30, "NOP*",      4,  4}, {0x31, "LXI SP,d16",10, 10},
    {0x32, "STA a16",  13, 13}, {0x33, "INX SP",    5,  5},
    {0x34, "INR M",    10, 10}, {0x35, "DCR M",    10, 10},
    {0x36, "MVI M,d8", 10, 10}, {0x37, "STC",       4,  4},
    {0x38, "NOP*",      4,  4}, {0x39, "DAD SP",   10, 10},
    {0x3A, "LDA a16",  13, 13}, {0x3B, "DCX SP",    5,  5},
    {0x3C, "INR A",     5,  5}, {0x3D, "DCR A",     5,  5},
    {0x3E, "MVI A,d8",  7,  7}, {0x3F, "CMC",       4,  4},

    // 0x40: MOV destination,source (M means memory at HL)
    {0x40, "MOV B,B",   5,  5}, {0x41, "MOV B,C",   5,  5},
    {0x42, "MOV B,D",   5,  5}, {0x43, "MOV B,E",   5,  5},
    {0x44, "MOV B,H",   5,  5}, {0x45, "MOV B,L",   5,  5},
    {0x46, "MOV B,M",   7,  7}, {0x47, "MOV B,A",   5,  5},
    {0x48, "MOV C,B",   5,  5}, {0x49, "MOV C,C",   5,  5},
    {0x4A, "MOV C,D",   5,  5}, {0x4B, "MOV C,E",   5,  5},
    {0x4C, "MOV C,H",   5,  5}, {0x4D, "MOV C,L",   5,  5},
    {0x4E, "MOV C,M",   7,  7}, {0x4F, "MOV C,A",   5,  5},

    // 0x50
    {0x50, "MOV D,B",   5,  5}, {0x51, "MOV D,C",   5,  5},
    {0x52, "MOV D,D",   5,  5}, {0x53, "MOV D,E",   5,  5},
    {0x54, "MOV D,H",   5,  5}, {0x55, "MOV D,L",   5,  5},
    {0x56, "MOV D,M",   7,  7}, {0x57, "MOV D,A",   5,  5},
    {0x58, "MOV E,B",   5,  5}, {0x59, "MOV E,C",   5,  5},
    {0x5A, "MOV E,D",   5,  5}, {0x5B, "MOV E,E",   5,  5},
    {0x5C, "MOV E,H",   5,  5}, {0x5D, "MOV E,L",   5,  5},
    {0x5E, "MOV E,M",   7,  7}, {0x5F, "MOV E,A",   5,  5},

    // 0x60
    {0x60, "MOV H,B",   5,  5}, {0x61, "MOV H,C",   5,  5},
    {0x62, "MOV H,D",   5,  5}, {0x63, "MOV H,E",   5,  5},
    {0x64, "MOV H,H",   5,  5}, {0x65, "MOV H,L",   5,  5},
    {0x66, "MOV H,M",   7,  7}, {0x67, "MOV H,A",   5,  5},
    {0x68, "MOV L,B",   5,  5}, {0x69, "MOV L,C",   5,  5},
    {0x6A, "MOV L,D",   5,  5}, {0x6B, "MOV L,E",   5,  5},
    {0x6C, "MOV L,H",   5,  5}, {0x6D, "MOV L,L",   5,  5},
    {0x6E, "MOV L,M",   7,  7}, {0x6F, "MOV L,A",   5,  5},

    // 0x70
    {0x70, "MOV M,B",   7,  7}, {0x71, "MOV M,C",   7,  7},
    {0x72, "MOV M,D",   7,  7}, {0x73, "MOV M,E",   7,  7},
    {0x74, "MOV M,H",   7,  7}, {0x75, "MOV M,L",   7,  7},
    {0x76, "HLT",       7,  7}, {0x77, "MOV M,A",   7,  7},
    {0x78, "MOV A,B",   5,  5}, {0x79, "MOV A,C",   5,  5},
    {0x7A, "MOV A,D",   5,  5}, {0x7B, "MOV A,E",   5,  5},
    {0x7C, "MOV A,H",   5,  5}, {0x7D, "MOV A,L",   5,  5},
    {0x7E, "MOV A,M",   7,  7}, {0x7F, "MOV A,A",   5,  5},

    // 0x80
    {0x80, "ADD B",     4,  4}, {0x81, "ADD C",     4,  4},
    {0x82, "ADD D",     4,  4}, {0x83, "ADD E",     4,  4},
    {0x84, "ADD H",     4,  4}, {0x85, "ADD L",     4,  4},
    {0x86, "ADD M",     7,  7}, {0x87, "ADD A",     4,  4},
    {0x88, "ADC B",     4,  4}, {0x89, "ADC C",     4,  4},
    {0x8A, "ADC D",     4,  4}, {0x8B, "ADC E",     4,  4},
    {0x8C, "ADC H",     4,  4}, {0x8D, "ADC L",     4,  4},
    {0x8E, "ADC M",     7,  7}, {0x8F, "ADC A",     4,  4},

    // 0x90
    {0x90, "SUB B",     4,  4}, {0x91, "SUB C",     4,  4},
    {0x92, "SUB D",     4,  4}, {0x93, "SUB E",     4,  4},
    {0x94, "SUB H",     4,  4}, {0x95, "SUB L",     4,  4},
    {0x96, "SUB M",     7,  7}, {0x97, "SUB A",     4,  4},
    {0x98, "SBB B",     4,  4}, {0x99, "SBB C",     4,  4},
    {0x9A, "SBB D",     4,  4}, {0x9B, "SBB E",     4,  4},
    {0x9C, "SBB H",     4,  4}, {0x9D, "SBB L",     4,  4},
    {0x9E, "SBB M",     7,  7}, {0x9F, "SBB A",     4,  4},

    // 0xA0
    {0xA0, "ANA B",     4,  4}, {0xA1, "ANA C",     4,  4},
    {0xA2, "ANA D",     4,  4}, {0xA3, "ANA E",     4,  4},
    {0xA4, "ANA H",     4,  4}, {0xA5, "ANA L",     4,  4},
    {0xA6, "ANA M",     7,  7}, {0xA7, "ANA A",     4,  4},
    {0xA8, "XRA B",     4,  4}, {0xA9, "XRA C",     4,  4},
    {0xAA, "XRA D",     4,  4}, {0xAB, "XRA E",     4,  4},
    {0xAC, "XRA H",     4,  4}, {0xAD, "XRA L",     4,  4},
    {0xAE, "XRA M",     7,  7}, {0xAF, "XRA A",     4,  4},

    // 0xB0
    {0xB0, "ORA B",     4,  4}, {0xB1, "ORA C",     4,  4},
    {0xB2, "ORA D",     4,  4}, {0xB3, "ORA E",     4,  4},
    {0xB4, "ORA H",     4,  4}, {0xB5, "ORA L",     4,  4},
    {0xB6, "ORA M",     7,  7}, {0xB7, "ORA A",     4,  4},
    {0xB8, "CMP B",     4,  4}, {0xB9, "CMP C",     4,  4},
    {0xBA, "CMP D",     4,  4}, {0xBB, "CMP E",     4,  4},
    {0xBC, "CMP H",     4,  4}, {0xBD, "CMP L",     4,  4},
    {0xBE, "CMP M",     7,  7}, {0xBF, "CMP A",     4,  4},

    // 0xC0
    {0xC0, "RNZ",       5, 11}, {0xC1, "POP B",    10, 10},
    {0xC2, "JNZ a16",  10, 10}, {0xC3, "JMP a16",  10, 10},
    {0xC4, "CNZ a16",  11, 17}, {0xC5, "PUSH B",   11, 11},
    {0xC6, "ADI d8",    7,  7}, {0xC7, "RST 0",    11, 11},
    {0xC8, "RZ",        5, 11}, {0xC9, "RET",      10, 10},
    {0xCA, "JZ a16",   10, 10}, {0xCB, "JMP* a16", 10, 10},
    {0xCC, "CZ a16",   11, 17}, {0xCD, "CALL a16", 17, 17},
    {0xCE, "ACI d8",    7,  7}, {0xCF, "RST 1",    11, 11},

    // 0xD0
    {0xD0, "RNC",       5, 11}, {0xD1, "POP D",    10, 10},
    {0xD2, "JNC a16",  10, 10}, {0xD3, "OUT d8",   10, 10},
    {0xD4, "CNC a16",  11, 17}, {0xD5, "PUSH D",   11, 11},
    {0xD6, "SUI d8",    7,  7}, {0xD7, "RST 2",    11, 11},
    {0xD8, "RC",        5, 11}, {0xD9, "RET*",     10, 10},
    {0xDA, "JC a16",   10, 10}, {0xDB, "IN d8",    10, 10},
    {0xDC, "CC a16",   11, 17}, {0xDD, "CALL* a16",17, 17},
    {0xDE, "SBI d8",    7,  7}, {0xDF, "RST 3",    11, 11},

    // 0xE0
    {0xE0, "RPO",       5, 11}, {0xE1, "POP H",    10, 10},
    {0xE2, "JPO a16",  10, 10}, {0xE3, "XTHL",     18, 18},
    {0xE4, "CPO a16",  11, 17}, {0xE5, "PUSH H",   11, 11},
    {0xE6, "ANI d8",    7,  7}, {0xE7, "RST 4",    11, 11},
    {0xE8, "RPE",       5, 11}, {0xE9, "PCHL",      5,  5},
    {0xEA, "JPE a16",  10, 10}, {0xEB, "XCHG",      5,  5},
    {0xEC, "CPE a16",  11, 17}, {0xED, "CALL* a16",17, 17},
    {0xEE, "XRI d8",    7,  7}, {0xEF, "RST 5",    11, 11},

    // 0xF0
    {0xF0, "RP",        5, 11}, {0xF1, "POP PSW",  10, 10},
    {0xF2, "JP a16",   10, 10}, {0xF3, "DI",         4,  4},
    {0xF4, "CP a16",   11, 17}, {0xF5, "PUSH PSW", 11, 11},
    {0xF6, "ORI d8",    7,  7}, {0xF7, "RST 6",    11, 11},
    {0xF8, "RM",        5, 11}, {0xF9, "SPHL",       5,  5},
    {0xFA, "JM a16",   10, 10}, {0xFB, "EI",         4,  4},
    {0xFC, "CM a16",   11, 17}, {0xFD, "CALL* a16",17, 17},
    {0xFE, "CPI d8",    7,  7}, {0xFF, "RST 7",    11, 11},
}};

[[nodiscard]] constexpr const OpcodeInfo& opcodeInfo(std::uint8_t opcode) noexcept {
    return kOpcodeTable[opcode];
}

constexpr bool opcodeTableIsIndexedCorrectly() noexcept {
    for (std::size_t i = 0; i < kOpcodeTable.size(); ++i) {
        if (kOpcodeTable[i].hex != i) {
            return false;
        }
    }
    return true;
}

static_assert(opcodeTableIsIndexedCorrectly());

#endif
