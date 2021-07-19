#pragma once


struct MovingStrategy {
    virtual void Move(double offset) = 0;
    virtual ~MovingStrategy() = default;
};
