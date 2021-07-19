#pragma once
#include "MovingStrategy.hpp"


struct IgnoringMovingStrategy : MovingStrategy {
    void Move(double offset) override {(void)offset;}
};
