#ifndef GPR_READER_POSITION_H
#define GPR_READER_POSITION_H

struct Position {
    int trace;
    int sample;

    bool operator==(const Position& other) const {
        return trace == other.trace && sample == other.sample;
    }
};

#endif //GPR_READER_POSITION_H
