#include "c8080.h"

//Performs the following operations:
//  1. Grab opcode from mem[pc]
//  2. Switch over opcode to perform function
//      2.1. Fetch additional addresses from memory
//  3. Increment pc by appropiate amount
int c8080::cycle()
{
    uint8_t opcode = mem[pc];
    switch (opcode) {
    case 0x00: //nop
        nop();
        break;
    case 0x01: //LXI B, D16
        //loads byte 2[pc + 1] into C and byte 3[pc + 2] into B
        //Increments pc by 3
        C.data = mem[pc + 1];
        B.data = mem[pc + 2];
        pc += 3;
        break;
    case 0x02: //stax b
        mem[(B.data << 8) | C.data] = A.data;
        pc++;
        break;
    case 0x03: //inx B
    {
        uint16_t val = (B.data << 8) | C.data;
        val += 1;
        B.data = val >> 8;
        C.data = val; //will be truncated 
        pc++;
        break;
    }
    case 0x04:
        inr(B);
        break;
    case 0x05:
        dcr(B);
        break;
    case 0x06: //MVI B, D8
        mov(B, mem[pc + 1]);
        break;
    case 0x07: //rlc 
    {
        FLAGS.data |= ((A.data & 0x80) >> 7); //set carry flag to MSB of accumulator 
        uint16_t res = A.data << 1; //result of bitshift left
        uint16_t msb = res & 0x100; //isolate MSB after shl
        res |= (msb >> 8); //incorporate msb into original result
        A.data = res; //truncates value to 8 bit 
        pc++;
        break;
    }
    case 0x08:
        nop();
        break;
    case 0x09: //dad b
    {
        uint16_t res = ((H.data << 8) | L.data) + ((B.data << 8) | C.data);
        (((H.data << 8) | L.data) + ((B.data << 8) | C.data) > UINT16_MAX) ? (FLAGS.data |= 0x01) : (FLAGS.data &= 0xFE); //set carry flag
        H.data = res >> 8;
        L.data = res & 0xFF;
        pc++;
        break;
    }
    case 0x0a: //ldax b
        A.data = mem[(B.data << 8) | C.data];
        pc++;
        break;
    case 0x0b: //dcx b
    {
        uint16_t res = ((B.data << 8) | (C.data)) - 1;
        B.data = res >> 8;
        C.data = res & 0xFF;
        pc++;
        break;
    }
    case 0x0c: //inr c
        inr(C);
        break;
    case 0x0d:
        dcr(C);
        break;
    case 0x0e: //mvi c, D8
        mov(C, mem[pc + 1]);
        break;
    case 0x0f:
        break;

    //0x10 - 0x1f
    case 0x10:
        nop();
        break;
    case 0x11: //lxi d, d16
        E.data = mem[pc + 1];
        D.data = mem[pc + 2];
        pc += 3;
        break;
    case 0x12: //stax d
        mem[(D.data << 8) | E.data] = A.data;
        pc++;
        break;
    case 0x13: //inx d
    {
        uint16_t val = (D.data << 8) | E.data;
        val += 1;
        D.data = val >> 8;
        E.data = val; //will be truncated 
        pc++;
        break;
    }
    case 0x14:
        inr(D);
        break;
    case 0x15:
        dcr(D);
        break;
    case 0x16: //MVI D, D8
        mov(D, mem[pc + 1]);
        break;
    case 0x17:
        break;
    case 0x18:
        nop();
        break;
    case 0x19: //dad d
    {
        uint16_t res = ((H.data << 8) | L.data) + ((D.data << 8) | E.data);
        (((H.data << 8) | L.data) + ((D.data << 8) | E.data) > UINT16_MAX) ? (FLAGS.data |= 0x01) : (FLAGS.data &= 0xFE); //set carry flag
        H.data = res >> 8;
        L.data = res & 0xFF;
        pc++;
        break;
    }
    case 0x1a: //ldax d
        A.data = mem[(D.data << 8) | E.data];
        pc++;
        break;
    case 0x1b: //dcx d
    {
        uint16_t res = ((D.data << 8) | (E.data)) - 1;
        D.data = res >> 8;
        E.data = res & 0xFF;
        pc++;
        break;
    }
    case 0x1c:
        inr(E);
        break;
    case 0x1d:
        dcr(E);
        break;
    case 0x1e: //mvi E, D8
        mov(E, mem[pc + 1]);
        break;
    case 0x1f:
        break;

    //0x20 - 0x2f
    case 0x20:
        nop();
        break;
    case 0x21: //lxi h, d16
        L.data = mem[pc + 1];
        H.data = mem[pc + 2];
        pc += 3;
        break;
    case 0x22: //shld, a16 //NOTE: check on this functionality 
    {
        uint16_t addr = (mem[pc + 2] << 8) | mem[pc + 1];
        mem[addr] = L.data;
        mem[addr + 1] = H.data;
        pc += 3;
        break;
    }
    case 0x23: //inx h
    {
        uint16_t val = (H.data << 8) | L.data;
        val += 1;
        H.data = val >> 8;
        L.data = val; //will be truncated 
        pc++;
        break;
    }
    case 0x24:
        inr(H);
        break;
    case 0x25:
        dcr(H);
        break;
    case 0x26: //mvi H, D8
        mov(H, mem[pc + 1]);
        break;
    case 0x27: //daa
    {
        //NOTE: this may not be the correct implementation...although this follows the steps laid out in
        //the 8080 manual. 
        //the 8080 manual. 
        //Although, it should be noted this code produces the same output as the manuals example. 
        if (((A.data & 0x0F) > 9) || (getFlagStatus(4))) {
            setACFlag(A.data, 6, 0, ADD);
            A.data += 6;
            if (((A.data & 0xF0) > 9) || (getFlagStatus(0))) {
                uint16_t highNibble = (A.data & 0xF0) >> 4;
                highNibble += 6;
                ((highNibble & 0xF0) > 0x00) ? (FLAGS.data |= 0x01) : (FLAGS.data = FLAGS.data); //set carry flag
                A.data |= (highNibble << 4);
            }
        }
        setZeroFlag(A.data);
        setParityFlag(A.data);
        setSignFlag(A.data);
        pc++;
        break;
    }
    case 0x28:
        nop();
        break;
    case 0x29: //dad h
    {
        uint16_t res = ((H.data << 8) | L.data) + ((H.data << 8) | L.data);
        (((H.data << 8) | L.data) + ((H.data << 8) | L.data) > UINT16_MAX) ? (FLAGS.data |= 0x01) : (FLAGS.data &= 0xFE); //set carry flag
        H.data = res >> 8;
        L.data = res & 0xFF;
        pc++;
        break;
    }
    case 0x2a: //lhld addr
    {
        L.data = mem[(mem[pc + 2] << 8) | mem[pc + 1]];
        H.data = mem[((mem[pc + 2] << 8) | mem[pc + 1]) + 1];
        pc += 3;
        break;
    }
    case 0x2b: //dcx h
    {
        uint16_t res = ((H.data << 8) | (L.data)) - 1;
        H.data = res >> 8;
        L.data = res & 0xFF;
        pc++;
        break;
    }
    case 0x2c:
        inr(L);
        break;
    case 0x2d:
        dcr(L);
        break;
    case 0x2e: //mvi L, D8
        mov(L, mem[pc + 1]);
        break;
    case 0x2f: //cma
        A.data = ~A.data;
        pc++;
        break;

    //0x30 - 0x3f
    case 0x30:
        nop();
        break;
    case 0x31: //lxi sp, d16
        sp = mem[pc + 2] << 8;
        sp |= mem[pc + 1];
        pc += 3;
        break;
    case 0x32: //sta a16
        mem[(mem[pc + 2] << 8) | mem[pc + 1]] = A.data;
        pc += 3;
        break;
    case 0x33: //inx sp
    {
        sp += 1;
        pc++;
        break;
    }
    case 0x34: //inr M
    {
        uint16_t addr = (H.data << 8) | L.data;
        uint8_t val = mem[addr];
        setACFlag(val, 1, 0, ADD);

        val++;
        mem[addr] = val;

        setZeroFlag(val);
        setSignFlag(val);
        setParityFlag(val);
        pc++;
        break;
    }
    case 0x35: //dcr M
    {
        uint16_t addr = (H.data << 8) | L.data;
        uint8_t val = mem[addr];
        setACFlag(val, 1, 0, SUB);

        val--;
        mem[addr] = val;

        setZeroFlag(val);
        setSignFlag(val);
        setParityFlag(val);
        pc++;
        break;
    }
    case 0x36: //mvi m, D8
        mem[getM()] = mem[pc + 1];
        pc += 2;
        break;
    case 0x37: //stc
        FLAGS.data |= 0x01;
        pc++;
        break;
    case 0x38:
        nop();
        break;
    case 0x39: //dad sp
    {
        uint32_t res = ((H.data << 8) | L.data) + sp;
        (res > UINT16_MAX) ? (FLAGS.data |= 0x01) : (FLAGS.data &= 0xFE); //set carry flag
        H.data = (res & 0xFFFF) >> 8;
        L.data = res & 0xFF;
        pc++;
        break;
    }
    case 0x3a: //lda a16
    {
        A.data = mem[(mem[pc + 2] << 8) | mem[pc + 1]];
        pc += 3;
        break;
    }
    case 0x3b: //dcx sp
    {
        sp -= 1;
        pc++;
        break;
    }
    case 0x3c:
        inr(A);
        break;
    case 0x3d:
        dcr(A);
        break;
    case 0x3e: //mvi A, D8
        mov(A, mem[pc + 1]);
        break;
    case 0x3f: //cmc
        FLAGS.data ^= 1;
        pc++;
        break;
    //switch through moves 0x40 - 0x4f
    case 0x40: //mov b,b
        mov(B, B);
        break;
    case 0x41: //mov b, c
        mov(B, C);
        break;
    case 0x42: //mov b, d
        mov(B, D);
        break;
    case 0x43: //mov b, e
        mov(B, E);
        break;
    case 0x44: //mov b, h
        mov(B, H);
        break;
    case 0x45: //mov b, l
        mov(B, L);
        break;
    case 0x46: //mov b, m(HL)
        B.data = mem[((H.data << 8) | L.data)];
        pc++;
        break;
    case 0x47: //mov b, a
        mov(B, A);
        break;
    case 0x48: //mov c, b
        mov(C, B);
        break;
    case 0x49: //mov c,c
        mov(C, C);
        break;
    case 0x4a: //mov c,d
        mov(C, D);
        break;
    case 0x4b: //mov c, e
        mov(C, E);
        break;
    case 0x4c: //mov c, h
        mov(C, H);
        break;
    case 0x4d: //mov c, l
        mov(C, L);
        break;
    case 0x4e: //mov c, m(HL)
        C.data = mem[((H.data << 8) | L.data)];
        pc++;
        break;
    case 0x4f: //mov c , a
        mov(C, A);
        break;

    //switches 0x50 - 0x5f
    case 0x50: //mov D,b
        mov(D, B);
        break;
    case 0x51: //mov D, c
        mov(D, C);
        break;
    case 0x52: //mov d, d
        mov(D, D);
        break;
    case 0x53: //mov d, e
        mov(D, E);
        break;
    case 0x54: //mov d, h
        mov(D, H);
        break;
    case 0x55: //mov d, l
        mov(D, L);
        break;
    case 0x56: //mov d, m(HL)
        D.data = mem[((H.data << 8) | L.data)];
        pc++;
        break;
    case 0x57: //mov d, a
        mov(D, A);
        break;
    case 0x58: //mov e, b
        mov(E, B);
        break;
    case 0x59: //mov e,c
        mov(E, C);
        break;
    case 0x5a: //mov e,d
        mov(E, D);
        break;
    case 0x5b: //mov e, e
        mov(E, E);
        break;
    case 0x5c: //mov e, h
        mov(E, H);
        break;
    case 0x5d: //mov e, l
        mov(E, L);
        break;
    case 0x5e: //mov e, m(HL)
        E.data = mem[((H.data << 8) | L.data)];
        pc++;
        break;
    case 0x5f: //mov e , a
        mov(E, A);
        break;

    //switches 0x60 - 0x6f
    case 0x60: //mov h,b
        mov(H, B);
        break;
    case 0x61: //mov h, c
        mov(H, C);
        break;
    case 0x62: //mov h, d
        mov(H, D);
        break;
    case 0x63: //mov h, e
        mov(H, E);
        break;
    case 0x64: //mov h, h
        mov(H, H);
        break;
    case 0x65: //mov h, l
        mov(H, L);
        break;
    case 0x66: //mov h, m(HL)
        H.data = mem[((H.data << 8) | L.data)];
        pc++;
        break;
    case 0x67: //mov h, a
        mov(H, A);
        break;
    case 0x68: //mov l, b
        mov(L, B);
        break;
    case 0x69: //mov l,c
        mov(L, C);
        break;
    case 0x6a: //mov l,d
        mov(L, D);
        break;
    case 0x6b: //mov l, e
        mov(L, E);
        break;
    case 0x6c: //mov l, h
        mov(L, H);
        break;
    case 0x6d: //mov l, l
        mov(L, L);
        break;
    case 0x6e: //mov l, m(HL)
        L.data = mem[((H.data << 8) | L.data)];
        pc++;
        break;
    case 0x6f: //mov l , a
        mov(L, A);
        break;

    //switches 0x70 - 0x7f
    //TODO:  Look into the logic used for 0x70-0x77 as I am assuming the 8 bit registers can only fit the L(low) register 
    //M register is the same as (H << 8) | L 
    case 0x70: //mov M,b
        mem[(H.data << 8) | L.data] = B.data;
        pc++;
        break;
    case 0x71: //mov M, c
        mem[(H.data << 8) | L.data] = C.data;
        pc++;
        break;
    case 0x72: //mov M, d
        mem[(H.data << 8) | L.data] = D.data;
        pc++;
        break;
    case 0x73: //mov M, e
        mem[(H.data << 8) | L.data] = E.data;
        pc++;
        break;
    case 0x74: //mov M, h
        mem[(H.data << 8) | L.data] = H.data;
        pc++;
        break;
    case 0x75: //mov M, l
        mem[(H.data << 8) | L.data] = L.data;
        pc++;
        break;
    case 0x76: //HLT TODO: check on halt functionality
        pc++; 
       // stateUpdate();
       // return std::pair<bool, uint16_t>(0, 0xFFFF); //break 
        return -1;
        break;
    case 0x77: //mov M, a
        mem[(H.data << 8) | L.data] = A.data;
        pc++;
        break;
    case 0x78: //mov a, b
        mov(A, B);
        break;
    case 0x79: //mov a,c
        mov(A, C);
        break;
    case 0x7a: //mov a,d
        mov(A, D);
        break;
    case 0x7b: //mov a, e
        mov(A, E);
        break;
    case 0x7c: //mov a, h
        mov(A, H);
        break;
    case 0x7d: //mov a, l
        mov(A, L);
        break;
    case 0x7e: //mov a, m(HL)
        A.data = mem[((H.data << 8) | L.data)];
        pc++;
        break;
    case 0x7f: //mov a , a
        mov(A, A);
        break;

    //0x80 - 0x8f
    case 0x80:
        add(A, B);
        break;
    case 0x81:
        add(A, C);
        break;
    case 0x82:
        add(A, D);
        break;
    case 0x83:
        add(A, E);
        break;
    case 0x84:
        add(A, H);
        break;
    case 0x85:
        add(A, L);
        break;
    case 0x86: //TODO: test this function
        add(A, mem[getM()]);
        break;
    case 0x87:
        add(A, A);
        break;
    case 0x88:
        adc(A, B);
        break;
    case 0x89:
        adc(A, C);
        break;
    case 0x8a:
        adc(A, D);
        break;
    case 0x8b:
        adc(A, E);
        break;
    case 0x8c:
        adc(A, H);
        break;
    case 0x8d:
        adc(A, L);
        break;
    case 0x8e://TODO: test this one functionality 
        adc(A, mem[getM()]);
        break;
    case 0x8f:
        adc(A, A);
        break;

    //0x90 - 0x9f
    case 0x90:
        sub(A, B);
        break;
    case 0x91:
        sub(A, C);
        break;
    case 0x92:
        sub(A, D);
        break;
    case 0x93:
        sub(A, E);
        break;
    case 0x94:
        sub(A, H);
        break;
    case 0x95:
        sub(A, L);
        break;
    case 0x96: //TODO: look into why this set a carry in other emulator but not mine
        sub(A, mem[getM()]);
        break;
    case 0x97:
        sub(A, A);
        break;
    case 0x98:
        sbb(A, B);
        break;
    case 0x99:
        sbb(A, C);
        break;
    case 0x9a:
        sbb(A, D);
        break;
    case 0x9b:
        sbb(A, E);
        break;
    case 0x9c:
        sbb(A, H);
        break;
    case 0x9d:
        sbb(A, L);
        break;
    case 0x9e: //TODO: look into why this set a carry in other emulator but not mine
        sbb(A, mem[getM()]);
        break;
    case 0x9f:
        sbb(A, A);
        break;

    //0xa0 - 0xaf
    case 0xa0: //ana b
        ana(A, B);
        break;
    case 0xa1: //ana c
        ana(A, C);
        break;
    case 0xa2: //ana d
        ana(A, D);
        break;
    case 0xa3: //ana e
        ana(A, E);
        break;
    case 0xa4: //ana h
        ana(A, H);
        break;
    case 0xa5: //ana l
        ana(A, L);
        break;
    case 0xa6: //ana m //TODO: test this
        ana(A, mem[getM()]);
        break;
    case 0xa7: //ana a
        ana(A, A);
        break;
    case 0xa8: //xra b
        xra(A, B);
        break;
    case 0xa9: //xra c
        xra(A, C);
        break;
    case 0xaa: //xra d
        xra(A, D);
        break;
    case 0xab: //xra e
        xra(A, E);
        break;
    case 0xac: //xra h
        xra(A, H);
        break;
    case 0xad: //xra l
        xra(A, L);
        break;
    case 0xae: //xra m
        xra(A, mem[getM()]);
        break;
    case 0xaf: //xra a
        xra(A, A);
        break;

    //0xb0 - 0xbf
    case 0xb0: //ora b
        ora(A, B);
        break;
    case 0xb1: //ora c
        ora(A, C);
        break;
    case 0xb2: //ora d
        ora(A, D);
        break;
    case 0xb3: //ora e
        ora(A, E);
        break;
    case 0xb4: //ora h
        ora(A, H);
        break;
    case 0xb5: //ora l
        ora(A, L);
        break;
    case 0xb6: //ora m
        ora(A, mem[getM()]);
        break;
    case 0xb7: //ora a
        ora(A, A);
        break;
    case 0xb8: //cmp b
        cmp(A, B);
        break;
    case 0xb9: //cmp c
        cmp(A, C);
        break;
    case 0xba: //cmp d
        cmp(A, D);
        break;
    case 0xbb: //cmp e
        cmp(A, E);
        break;
    case 0xbc: //cmp h
        cmp(A, H);
        break;
    case 0xbd: //cmp l
        cmp(A, L);
        break;
    case 0xbe: //cmp M
        cmp(A, mem[getM()]);
        break;
    case 0xbf: //cmp a
        cmp(A, A);
        break;

    //0xc0 - 0xcf
    case 0xc0: //rnz
    {
        if (!getFlagStatus(6)) {
            ret();
            break;
        }
        pc++;
        break;
    }
    case 0xc1: //pop B
    {
        C.data = mem[sp];
        B.data = mem[sp + 1]; 
        sp += 2;
        pc++;
        break;
    }
    case 0xc2: //jnz
    {
        if (!getFlagStatus(6)) {
            jmp();
            break;
        }
        pc += 3;
        break;
    }
    case 0xc3: //jmp a16
        jmp();
        break;
    case 0xc4: //cnz
    {
        if (!getFlagStatus(6)) {
            call();
            break;
        }
        pc += 3;
        break;
    }
    case 0xc5: //push B
    {
        mem[sp - 2] = C.data;
        mem[sp - 1] = B.data;
        sp -= 2;
        pc++;
        break;
    }
    case 0xc6: //adi d8
        add(A, mem[pc + 1]);
        pc++; //add 1 to pc for additional byte used
        break;
    case 0xc7: //rst 0
        rst(0);
        break;
    case 0xc8: //rz
    {
        if (getFlagStatus(6)) {
            ret();
            break;
        }
        pc++;
        break;
    }
    case 0xc9: //ret
        ret();
        break;
    case 0xca: //jz
    {
        if (getFlagStatus(6)) {
            jmp();
            break;
        }
        pc += 3;
        break;
    }
    case 0xcb: //jmp a16
        jmp();
        break;
    case 0xcc: //cz a16
    {
        if (getFlagStatus(6)) {
            call();
            break;
        }
        pc += 3;
        break;
    }
    case 0xcd: //call a16
        call();
        break;
    case 0xce: //ACI d8
        adc(A, mem[pc + 1]);
        pc++; //add 1 to pc to acount for extra byte in instruction
        break;
    case 0xcf: //rst 1
        rst(1);
        break;

    //0xd0 - 0xdf
    case 0xd0: //rnc
    {
        if (!getFlagStatus(0)) {
            ret();
            break;
        }
        pc++;
        break;
    }
    case 0xd1: //pop D
    {
        E.data = mem[sp];
        D.data = mem[sp + 1];
        sp += 2;
        pc++;
        break;
    }
    case 0xd2: //jnc
    {
        if (!getFlagStatus(0)) {
            jmp();
            break;
        }
        pc += 3;
        break;
    }
    case 0xd3: //out d8
    {
        uint8_t data = A.data;
        output = (mem[pc + 1] << 8) | data;
        pc += 2;
        out = true;
        break;
    }
    case 0xd4: //cnc
    {
        if (!getFlagStatus(0)) {
            call();
            break;
        }
        pc += 3;
        break;
    }
    case 0xd5: //push D
    {
        mem[sp - 2] = E.data;
        mem[sp - 1] = D.data;
        sp -= 2;
        pc++;
        break;
    }
    case 0xd6: //sui d8
        sub(A, mem[pc + 1]);
        pc++; //to account for extra byte used
        break;
    case 0xd7: //rst 2
        rst(2);
        break;
    case 0xd8: //rc
    {
        if (getFlagStatus(0)) {
            ret();
            break;
        }
        pc++;
        break;
    }
    case 0xd9: //ret
        ret();
        break;
    case 0xda: //jc
    {
        if (getFlagStatus(0)) {
            jmp();
            break;
        }
        pc += 3;
        break;
    }
    case 0xdb: //in d8
    {
        uint8_t data = A.data;
        pc += 2;
        output = data;
        in = true;
        break;
    }
    case 0xdc: //cc 
    {
        if (getFlagStatus(0)) {
            call();
            break;
        }
        pc += 3;
        break;
    }
    case 0xdd: //call
        call();
        break;
    case 0xde: //sbi d8
        sbb(A, mem[pc + 1]);
        pc++; //to account for extra byte used
        break;
    case 0xdf: //rst 3
        rst(3);
        break;

    //0xe0 - 0xef
    case 0xe0: //rpo 
    {
        if (!getFlagStatus(2)) {
            ret();
            break;
        }
        pc++;
        break;
    }
    case 0xe1: //pop H
    {
        L.data = mem[sp];
        H.data = mem[sp + 1];
        sp += 2;
        pc++;
        break;
    }
    case 0xe2: //jpo
    {
        if (!getFlagStatus(2)) {
            jmp();
            break;
        }
        pc += 3;
        break;
    }
    case 0xe3: //xthl
    {
        uint8_t temp = L.data;
        L.data = mem[sp];
        mem[sp] = temp;

        temp = H.data;
        H.data = mem[sp + 1];
        mem[sp + 1] = temp;
        pc++;
        break;
    }
    case 0xe4: //cpo
    {
        if (!getFlagStatus(2)) {
            call();
            break;
        }
        pc += 3;
        break;
    }
    case 0xe5: //push H
    {
        mem[sp - 2] = L.data;
        mem[sp - 1] = H.data;
        sp -= 2;
        pc++;
        break;
    }
    case 0xe6: //ani d8
        ana(A, mem[pc + 1]);
        pc++; //add 1 to pc to account for extra byte
        break;
    case 0xe7: //rst 4
        rst(4);
        break;
    case 0xe8: //rpe
    {
        if (getFlagStatus(2)) {
            ret();
            break;
        }
        pc++;
        break;
    }
    case 0xe9: //pchl
        pc = (H.data << 8) | L.data;
        break;
    case 0xea: //jpe
    {
        if (getFlagStatus(2)) {
            jmp();
            break;
        }
        pc += 3;
        break;
    }
    case 0xeb: //xchg
    {
        uint8_t temp = 0x00;
        temp = H.data;
        H.data = D.data;
        D.data = temp;

        temp = L.data;
        L.data = E.data;
        E.data = temp;
        pc++;
        break;
    }
    case 0xec: //cpe
    {
        if (getFlagStatus(2)) {
            call();
            break;
        }
        pc += 3;
        break;
    }
    case 0xed: //call
        call();
        break;
    case 0xee: //xri d8
        xra(A, mem[pc + 1]);
        pc++; //add 1 to pc to account for extra byte
        break;
    case 0xef: //rst 5
        rst(5);
        break;

    //0xf0 - 0xff
    case 0xf0: //rp
    {
        if (!getFlagStatus(7)) {
            ret();
            break;
        }
        pc++;
        break;
    }
    case 0xf1: //pop psw
    {
        FLAGS.data = mem[sp];
        A.data = mem[sp + 1];
        sp += 2;
        pc++;
        break;
    }
    case 0xf2: //jp
    {
        if (!getFlagStatus(7)) {
            jmp();
            break;
        }
        pc += 3;
        break;
    }
    case 0xf3: //di
        interruptEnable = 0;
        pc++;
        break;
    case 0xf4: //cp
    {
        if (!getFlagStatus(7)) {
            call();
            break;
        }
        pc += 3;
        break;
    }
    case 0xf5: //push PSW
    {
        mem[sp - 2] = FLAGS.data;
        mem[sp - 1] = A.data;
        sp -= 2;
        pc++;
        break;
    }
    case 0xf6: //ori d8
        ora(A, mem[pc + 1]);
        pc++; //to account for extra byte
        break;
    case 0xf7: //rst 6
        rst(6);
        break;
    case 0xf8: //rm
    {
        if (getFlagStatus(7)) {
            ret();
            break;
        }
        pc++;
        break;
    }
    case 0xf9: //sphl
        sp = (H.data << 8) | L.data;
        pc++;
        break;
    case 0xfa: //jm
    {
        if (getFlagStatus(7)) {
            jmp();
            break;
        }
        pc += 3;
        break;
    }
    case 0xfb: //ei
        interruptEnable = 1;
        pc++;
        break;
    case 0xfc: //cm 
    {
        if (getFlagStatus(7)) {
            call();
            break;
        }
        pc += 3;
        break;
    }

    case 0xfd: call();      break; //call
    case 0xfe: cmp(A, mem[pc + 1]); pc++;   break; //cmp d8, adds 1 extra byte to pc
    case 0xff:  rst(7);     break; //rst 7
    default:  break;
    }
    cycles++;
    return 0;
}

//move register into register
void c8080::mov(reg& f, reg& s)
{
    f.data = s.data;
    pc++;
    //increase cycle by one
}

//move immediate into register
//uses [pc + 1], then increments pc by 2
void c8080::mov(reg& f, uint8_t s)
{
    f.data = s;
    pc += 2;
    //TODO: increase cycle by one
}

//Adds two registers together, setting appropriate flags 
//TODO: check on if ac flag is correct
void c8080::add(reg& f, reg& s)
{
    setCarryFlag(f.data, s.data);
    setACFlag(f.data, s.data);

    f.data += s.data; //add numbers

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::add(reg& f, uint8_t s)
{
    setCarryFlag(f.data, s);
    setACFlag(f.data, s);

    f.data += s; //add numbers

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::adc(reg& f, reg& s)
{
    //Method to set bit: or the flags register with 2^i where i is the bits position(from 0 to 7)
    uint8_t carryData = getFlagStatus(0); //store carry data

  //  resetFlags();
    setCarryFlag(f.data, s.data, carryData);
    setACFlag(f.data, s.data, carryData);

    f.data += s.data + carryData; //add numbers

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::adc(reg& f, uint8_t s)
{
    //Method to set bit: or the flags register with 2^i where i is the bits position(from 0 to 7)
    uint8_t carryData = getFlagStatus(0); //store carry data

  //  resetFlags();
    setCarryFlag(f.data, s, carryData);
    setACFlag(f.data, s, carryData);

    f.data += s + carryData; //add numbers

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::sub(reg& f, reg& s)
{
    setCarryFlag(f.data, s.data, 0, SUB);
    setACFlag(f.data, s.data, 0, SUB);

    f.data -= s.data; //add numbers

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::sub(reg& f, uint8_t s)
{
    setCarryFlag(f.data, s, 0, SUB);
    setACFlag(f.data, s, 0, SUB);

    f.data -= s; //add numbers

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::sbb(reg& f, reg& s) //TODO: the other emulator is giving carrie
{
    //Method to set bit: or the flags register with 2^i where i is the bits position(from 0 to 7)
    uint8_t carryData = getFlagStatus(0); //store carry data

    setCarryFlag(f.data, s.data, carryData, SUB);
    setACFlag(f.data, s.data, carryData, SUB);

    f.data -= (s.data + carryData); //add numbers

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::sbb(reg& f, uint8_t s)
{
    //Method to set bit: or the flags register with 2^i where i is the bits position(from 0 to 7)
    uint8_t carryData = getFlagStatus(0); //store carry data

    setCarryFlag(f.data, s, carryData, SUB);
    setACFlag(f.data, s, carryData, SUB);

    f.data -= (s + carryData); //add numbers

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::ana(reg& f, reg& s)
{
    setCarryFlag(f.data, s.data, 0, AND);
    setACFlag(f.data, s.data, 0, AND);

    f.data &= s.data;

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::ana(reg& f, uint8_t s)
{
    setCarryFlag(f.data, s, 0, AND);
    setACFlag(f.data, s, 0, AND);

    f.data &= s;

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::xra(reg& f, reg& s)
{
    setCarryFlag(f.data, s.data, 0, XOR);
    setACFlag(f.data, s.data, 0, XOR);

    f.data ^= s.data;

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::xra(reg& f, uint8_t s)
{
    setCarryFlag(f.data, s, 0, XOR);
    setACFlag(f.data, s, 0, XOR);

    f.data ^= s;

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::ora(reg& f, reg& s)
{
    setCarryFlag(f.data, s.data, 0, OR);
    setACFlag(f.data, s.data, 0, OR);

    f.data |= s.data;

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::ora(reg& f, uint8_t s)
{
    setCarryFlag(f.data, s, 0, OR);
    setACFlag(f.data, s, 0, OR);

    f.data |= s;

    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

//sets condition flags and increments pc
void c8080::cmp(reg& f, reg& s)
{
    setCarryFlag(f.data, s.data, 0, SUB);
    setACFlag(f.data, s.data, 0, SUB);
    setSignFlag(f.data - s.data);
    setZeroFlag(f.data - s.data);
    setParityFlag(f.data - s.data);
    pc++;
}

void c8080::cmp(reg& f, uint8_t s)
{
    setCarryFlag(f.data, s, 0, SUB);
    setACFlag(f.data, s, 0, SUB);
    setSignFlag(f.data - s);
    setZeroFlag(f.data - s);
    setParityFlag(f.data - s);
    pc++;
}

void c8080::inr(reg& f)
{
    setACFlag(f.data, 1, 0, ADD);
    f.data += 1;
    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

void c8080::dcr(reg& f)
{
    setACFlag(f.data, 1, 0, SUB);
    f.data = f.data - 1;
    setSignFlag(f.data);
    setZeroFlag(f.data);
    setParityFlag(f.data);
    pc++;
}

//TODO: fix hack arounds
//return instruction
//does not increment pc
void c8080::ret()
{
    pc = mem[sp];
    pc |= (mem[sp + 1] << 8);
    sp += 2;
    if (sp = 0xFFFF) {
        sp = 0;
    }
}
//jumps to addr
void c8080::jmp()
{
    pc = (mem[pc + 2] << 8) | mem[pc + 1];
}

//TODO: fix hackarounds
void c8080::call() 
{
    uint16_t addr = sp - 1;
    uint16_t retAddr = pc + 3; //+3 to return ahead of call instruction
    if (addr == 0xFFFF) {
        sp = 0xFFFF;
    }
    mem[sp - 1] = (retAddr >> 8);
    mem[sp - 2] = (retAddr & 0xFF);
    sp -= 2;
    pc = (mem[pc + 2] << 8) | mem[pc + 1];
}

//pushes the current pc to the stack 
//sets the new pc to 8 * base
void c8080::rst(uint8_t base)
{
    mem[sp - 1] = (pc >> 8);
    mem[sp - 2] = (pc & 0x0F);
    sp -= 2;
    pc = 0x08 * base;
}


void c8080::nop()
{
    pc++;
}

//returns the value of a flag(denoted by i)
//Below are the values i for certain flags
//  7. sign
//  6. zero
//  4. AC
//  2. Parity
//  0. Carry
int c8080::getFlagStatus(int i)
{
    return (FLAGS.data & (0x00 | (uint8_t)pow(2, i))) >> i;
}

//returns 1 for even parity, 0 for odd parity
int c8080::calculateParity(uint16_t f)
{
    int onBits = 0;
    for (int i = 0; i < 16; i++) {
        if ((f >> i) & 0x01) {
            onBits++;
        }
    }
    return (onBits % 2) == 0 ? 1 : 0;
}

//loads a program(in hexadecimal format) into memory starting at startAddr
void c8080::loadProgram(uint16_t startAddr, std::string program)
{
    uint8_t memAddr = 0x00;
    for (int i = 0; i < program.size(); i += 2) {
        std::stringstream s;
        s << program.at(i);
        s << program.at(i + 1);
        int value;
        s >> std::hex >> value;
        mem[memAddr++] = value ;
    }
}

//takes a uint8_t and sets appropiate bit in flag if it is zero
void c8080::setZeroFlag(uint8_t f)
{
    if (f == 0) {
        FLAGS.data |= 0x40;
        return;
    }
    FLAGS.data &= 0xBF; //turn off AC Flag bit
}

//takes two uint8_t variables, f and s, and sets appropiate bit in flag if their is a carry from low nibble to high nibble in the result
//of their sum
//The op argument is used to signify if this function is checking for AC flag with subtraction or addition
void c8080::setACFlag(uint8_t f, uint8_t s, uint8_t c, operation op)
{
    bool enable = false; //set to true if the ac flag should be set
    switch (op) {
    case ADD:
    {
        if ((f & 0x10) != ((f + s + c) & 0x10)) {
            enable = true;
        }
        break;
    }
    case SUB:
        if ((f & 0x10) != ((f - s - c) & 0x10)) {
            enable = true;
        }
        break;
    case AND: //logical operations do not affect ac flag
        break;
    case XOR:
        break;
    case OR:
        break;
    }
    if (enable)
        FLAGS.data |= 0x10;
    else
        FLAGS.data &= 0xEF;
}

//takes a uint8_t and sets appropiate bit in flag if the first bit is a sign bit
void c8080::setSignFlag(uint8_t f)
{
    if (f >> 7) {
        FLAGS.data |= 0x80;
        return;
    }
    FLAGS.data &= 0x7F; //turn off AC Flag bit
}

//TODO: Check into when exactly carry flag is set. Is it set anytime there is a change in MSB or just when the MSB goes from 1 to 0?
// 
//takes two uint8_t, f and s, and sets appropriate bit in flag if their sum produces a carry
//The op argument is used to signify if this function is checking for AC flag with subtraction or addition
void c8080::setCarryFlag(uint8_t f, uint8_t s, uint8_t c, operation op)
{
    bool enable = false; //set to true if the carry flag should be set
    switch (op) {
    case ADD:
        if (f + s + c > UINT8_MAX) {
            enable = true;
        }
        break;
    case SUB:
    {
        //Carry flag is set for subtraction if the twos comliment of the subtraction does not produce a carry 
        //Current works for all values tested besides a number minus itself

        //TODO: hacked solution for number equal to itself, fix later
        if (((((~(f - s - c)) + 1) & 0x100) != 0x100) && (f != s)) {
            enable = true;
        }
        break;
    }
    case AND: //logical operations do not affect the c flag
        break;
    case XOR:
        break;
    case OR:
        break;
    }
    if (enable)
        FLAGS.data |= 0x01;
    else
        FLAGS.data &= 0xFE;
}

//takes a uint8_t and sets appropriate bit in flag to 1 if the parity of f is even, 0 otherwise
void c8080::setParityFlag(uint8_t f)
{
    if (calculateParity(f)) {
        FLAGS.data |= 0x04;
        return;
    }
    FLAGS.data &= 0xFB; //turn off P Flag bit
}

//resets the flags to their default value (0x02)
void c8080::resetFlags()
{
    FLAGS.data = 0x02; //sinces the flags register always have bit 1 = 1
}

void c8080::reset()
{
    sp = 0x00;
    pc = 0x00;
    A.data = 0x00;
    B.data = 0x00;
    C.data = 0x00;
    D.data = 0x00;
    E.data = 0x00;
    H.data = 0x00;
    L.data = 0x00;
    resetFlags();
    memset(mem, 0, sizeof(mem));
}

//returns the value given by H << 8 | L 
uint16_t c8080::getM()
{
    return ((H.data << 8) | L.data);
}
