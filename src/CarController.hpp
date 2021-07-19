#pragma once
#include "MovingStrategy.hpp"
#include "IgnoringMovingStrategy.hpp"
#include <memory>


class CarController {
public:

    void SetMovingStrategy(std::unique_ptr<MovingStrategy> strategy) {
        m_strategy = std::move(strategy);
    }
    
    /**
     *  Move
     *  @param distance signed distance to desired position
     */
    void Move(double offset) {
        m_strategy->Move(offset);
    }

private:
    std::unique_ptr<MovingStrategy> m_strategy
        = std::make_unique<IgnoringMovingStrategy>();
};
