//
// Created by Leonardo paroli on 04/05/2017.
//

#ifndef ENTROPY_ENUMHASHER_H
#define ENTROPY_ENUMHASHER_H

struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

#endif //ENTROPY_ENUMHASHER_H
