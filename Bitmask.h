//
// Created by Leonardo on 6/29/2017.
//

#ifndef COLDSPACE_BITMASK_H
#define COLDSPACE_BITMASK_H

#include <stdint.h>

//Cross-platform define.
using Bitset = uint32_t;

class Bitmask{
public:
    Bitmask() : bits(0){}
    Bitmask(const Bitset& l_bits) : bits(l_bits){}

    Bitset GetMask() const{ return bits; }
    void SetMask(const Bitset& l_value){ bits = l_value; }

    bool Matches(const Bitmask& l_bits,
                 const Bitset& l_relevant = 0)const
    {
        return(l_relevant ? ((l_bits.GetMask() & l_relevant) == (bits & l_relevant))
                          :(l_bits.GetMask() == bits));
    }

    bool GetBit(const unsigned int& l_pos)const{
        return ((bits&(1 << l_pos)) != 0);
    }

    void TurnOnBit(const unsigned int& l_pos){
        bits |= 1 << l_pos;
    }

    void TurnOnBits(const Bitset& l_bits){
        bits |= l_bits;
    }

    void ClearBit(const unsigned int& l_pos){
        bits &= ~(1 << l_pos);
    }

    void ToggleBit(const unsigned int& l_pos){
        bits ^= 1 << l_pos;
    }

    //reset bitset
    void Clear(){ bits = 0; }
private:

    //32 bits for every need.
    Bitset bits;
};

#endif //COLDSPACE_BITMASK_H
