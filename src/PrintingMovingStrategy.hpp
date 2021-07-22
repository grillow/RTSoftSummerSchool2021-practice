#pragma once
#include "MovingStrategy.hpp"
#include <ostream>


struct PrintingMovingStrategy : MovingStrategy {
    PrintingMovingStrategy(std::ostream & out) : out(out) {}

    void Move(double offset) override {
        out << "offset = " << offset << std::endl;
    }

private:
    std::ostream & out;
};
