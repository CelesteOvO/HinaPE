// Copyright (c) 2018 Doyub Kim
//
// I am making my contributions/submissions to this project solely in my
// personal capacity and am not conveying any rights to any intellectual
// property of any third parties.

#include "math_lib/pch.h"

#include "math_lib/constants.h"
#include "physics_animation.h"
#include "timer.h"

using namespace jet;

PhysicsAnimation::PhysicsAnimation() { _currentFrame.index = -1; }

PhysicsAnimation::~PhysicsAnimation() = default;

auto PhysicsAnimation::isUsingFixedSubTimeSteps() const -> bool
{
    return _isUsingFixedSubTimeSteps;
}

void PhysicsAnimation::setIsUsingFixedSubTimeSteps(bool isUsing)
{
    _isUsingFixedSubTimeSteps = isUsing;
}

auto PhysicsAnimation::numberOfFixedSubTimeSteps() const -> unsigned int
{
    return _numberOfFixedSubTimeSteps;
}

void PhysicsAnimation::setNumberOfFixedSubTimeSteps(unsigned int numberOfSteps)
{
    _numberOfFixedSubTimeSteps = numberOfSteps;
}

void PhysicsAnimation::advanceSingleFrame()
{
    Frame f = _currentFrame;
    update(++f);
}

auto PhysicsAnimation::currentFrame() const -> Frame { return _currentFrame; }

void PhysicsAnimation::setCurrentFrame(const Frame &frame)
{
    _currentFrame = frame;
}

auto PhysicsAnimation::currentTimeInSeconds() const -> double { return _currentTime; }

auto PhysicsAnimation::numberOfSubTimeSteps(double timeIntervalInSeconds) const -> unsigned int
{
    UNUSED_VARIABLE(timeIntervalInSeconds);

    // Returns number of fixed sub-timesteps by default
    return _numberOfFixedSubTimeSteps;
}

void PhysicsAnimation::onUpdate(const Frame &frame)
{
    if (frame.index > _currentFrame.index)
    {
        if (_currentFrame.index < 0)
        {
            initialize();
        }

        int32_t numberOfFrames = frame.index - _currentFrame.index;

        for (int32_t i = 0; i < numberOfFrames; ++i)
        {
            advanceTimeStep(frame.timeIntervalInSeconds);
        }

        _currentFrame = frame;
    }
}

void PhysicsAnimation::advanceTimeStep(double timeIntervalInSeconds)
{
    _currentTime = _currentFrame.timeInSeconds();

    if (_isUsingFixedSubTimeSteps)
    {
        JET_INFO << "Using fixed sub-timesteps: " << _numberOfFixedSubTimeSteps;

        // Perform fixed time-stepping
        const double actualTimeInterval = timeIntervalInSeconds / static_cast<double>(_numberOfFixedSubTimeSteps);

        for (unsigned int i = 0; i < _numberOfFixedSubTimeSteps; ++i)
        {
            JET_INFO << "Begin onAdvanceTimeStep: " << actualTimeInterval << " (1/" << 1.0 / actualTimeInterval << ") seconds";

            Timer timer;
            onAdvanceTimeStep(actualTimeInterval);

            JET_INFO << "End onAdvanceTimeStep (took " << timer.durationInSeconds() << " seconds)";

            _currentTime += actualTimeInterval;
        }
    } else
    {
        JET_INFO << "Using adaptive sub-timesteps";

        // Perform adaptive time-stepping
        double remainingTime = timeIntervalInSeconds;
        while (remainingTime > kEpsilonD)
        {
            unsigned int numSteps = numberOfSubTimeSteps(remainingTime);
            double actualTimeInterval = remainingTime / static_cast<double>(numSteps);

            JET_INFO << "Number of remaining sub-timesteps: " << numSteps;

            JET_INFO << "Begin onAdvanceTimeStep: " << actualTimeInterval << " (1/" << 1.0 / actualTimeInterval << ") seconds";

            Timer timer;
            onAdvanceTimeStep(actualTimeInterval);
            std::cout << ">> " << remainingTime << std::endl;

            JET_INFO << "End onAdvanceTimeStep (took " << timer.durationInSeconds() << " seconds)";

            remainingTime -= actualTimeInterval;
            _currentTime += actualTimeInterval;
        }
    }
}

void PhysicsAnimation::initialize() { onInitialize(); }

void PhysicsAnimation::onInitialize()
{
    // Do nothing
}
