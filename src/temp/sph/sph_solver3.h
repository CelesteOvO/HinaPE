// Copyright (c) 2018 Doyub Kim
//
// I am making my contributions/submissions to this project solely in my
// personal capacity and am not conveying any rights to any intellectual
// property of any third parties.

#ifndef INCLUDE_JET_SPH_SOLVER3_H_
#define INCLUDE_JET_SPH_SOLVER3_H_

#include "math_lib/constants.h"
#include "kernel/particle_system_solver3.h"
#include "sph_system_data3.h"

namespace jet
{

//!
//! \brief 3-D SPH solver.
//!
//! This class implements 3-D SPH solver. The main pressure solver is based on
//! equation-of-state (EOS).
//!
//! \see M{\"u}ller et al., Particle-based fluid simulation for interactive
//!      applications, SCA 2003.
//! \see M. Becker and M. Teschner, Weakly compressible SPH for free surface
//!      flows, SCA 2007.
//! \see Adams and Wicke, Meshless approximation methods and applications in
//!      physics based modeling and animation, Eurographics tutorials 2009.
//!
class SphSolver3 : public ParticleSystemSolver3
{
public:
    class Builder;

    //! Constructs a solver with empty particle set.
    SphSolver3();

    //! Constructs a solver with target density, spacing, and relative kernel
    //! radius.
    SphSolver3(double targetDensity, double targetSpacing, double relativeKernelRadius);

    ~SphSolver3() override;

    //! Returns the exponent part of the equation-of-state.
    auto eosExponent() const -> double;

    //!
    //! \brief Sets the exponent part of the equation-of-state.
    //!
    //! This function sets the exponent part of the equation-of-state.
    //! The value must be greater than 1.0, and smaller inputs will be clamped.
    //! Default is 7.
    //!
    void setEosExponent(double newEosExponent);

    //! Returns the negative pressure scale.
    auto negativePressureScale() const -> double;

    //!
    //! \brief Sets the negative pressure scale.
    //!
    //! This function sets the negative pressure scale. By setting the number
    //! between 0 and 1, the solver will scale the effect of negative pressure
    //! which can prevent the clumping of the particles near the surface. Input
    //! value outside 0 and 1 will be clamped within the range. Default is 0.
    //!
    void setNegativePressureScale(double newNegativePressureScale);

    //! Returns the viscosity coefficient.
    auto viscosityCoefficient() const -> double;

    //! Sets the viscosity coefficient.
    void setViscosityCoefficient(double newViscosityCoefficient);

    //! Returns the pseudo viscosity coefficient.
    auto pseudoViscosityCoefficient() const -> double;

    //!
    //! \brief Sets the pseudo viscosity coefficient.
    //!
    //! This function sets the pseudo viscosity coefficient which applies
    //! additional pseudo-physical damping to the system. Default is 10.
    //!
    void setPseudoViscosityCoefficient(double newPseudoViscosityCoefficient);

    //! Returns the speed of sound.
    auto speedOfSound() const -> double;

    //!
    //! \brief Sets the speed of sound.
    //!
    //! This function sets the speed of sound which affects the stiffness of the
    //! EOS and the time-step size. Higher value will make EOS stiffer and the
    //! time-step smaller. The input value must be higher than 0.0.
    //!
    void setSpeedOfSound(double newSpeedOfSound);

    //!
    //! \brief Multiplier that scales the max allowed time-step.
    //!
    //! This function returns the multiplier that scales the max allowed
    //! time-step. When the scale is 1.0, the time-step is bounded by the speed
    //! of sound and max acceleration.
    //!
    auto timeStepLimitScale() const -> double;

    //!
    //! \brief Sets the multiplier that scales the max allowed time-step.
    //!
    //! This function sets the multiplier that scales the max allowed
    //! time-step. When the scale is 1.0, the time-step is bounded by the speed
    //! of sound and max acceleration.
    //!
    void setTimeStepLimitScale(double newScale);

    //! Returns the SPH system data.
    auto sphSystemData() const -> SphSystemData3Ptr;

    //! Returns builder fox SphSolver3.
    static auto builder() -> Builder;

protected:
    //! Returns the number of sub-time-steps.
    auto numberOfSubTimeSteps(double timeIntervalInSeconds) const -> unsigned int override;

    //! Accumulates the force to the forces array in the particle system.
    void accumulateForces(double timeStepInSeconds) override;

    //! Performs pre-processing step before the simulation.
    void onBeginAdvanceTimeStep(double timeStepInSeconds) override;

    //! Performs post-processing step before the simulation.
    void onEndAdvanceTimeStep(double timeStepInSeconds) override;

    //! Accumulates the non-pressure forces to the forces array in the particle
    //! system.
    virtual void accumulateNonPressureForces(double timeStepInSeconds);

    //! Accumulates the pressure force to the forces array in the particle
    //! system.
    virtual void accumulatePressureForce(double timeStepInSeconds);

    //! Computes the pressure.
    void computePressure() const;

    //! Accumulates the pressure force to the given \p pressureForces array.
    void accumulatePressureForce(const ConstArrayAccessor1<Vector3D> &positions, const ConstArrayAccessor1<double> &densities, const ConstArrayAccessor1<double> &pressures, ArrayAccessor1<Vector3D> pressureForces) const;

    //! Accumulates the viscosity force to the forces array in the particle
    //! system.
    void accumulateViscosityForce() const;

    //! Computes pseudo viscosity.
    void computePseudoViscosity(double timeStepInSeconds) const;

private:
    //! Exponent component of equation-of-state (or Tait's equation).
    double _eosExponent = 7.0;

    //! Negative pressure scaling factor.
    //! Zero means clamping. One means do nothing.
    double _negativePressureScale = 0.0;

    //! Viscosity coefficient.
    double _viscosityCoefficient = 0.01;

    //! Pseudo-viscosity coefficient velocity filtering.
    //! This is a minimum "safety-net" for SPH solver which is quite
    //! sensitive to the parameters.
    double _pseudoViscosityCoefficient = 10.0;

    //! Speed of sound in medium to determin the stiffness of the system.
    //! Ideally, it should be the actual speed of sound in the fluid, but in
    //! practice, use lower value to trace-off performance and compressibility.
    double _speedOfSound = 100.0;

    //! Scales the max allowed time-step.
    double _timeStepLimitScale = 1.0;
};

//! Shared pointer type for the SphSolver3.
using SphSolver3Ptr = std::shared_ptr<SphSolver3>;

//!
//! \brief Base class for SPH-based fluid solver builder.
//!
template<typename DerivedBuilder>
class SphSolverBuilderBase3
{
public:
    //! Returns builder with target density.
    auto withTargetDensity(double targetDensity) -> DerivedBuilder &;

    //! Returns builder with target spacing.
    auto withTargetSpacing(double targetSpacing) -> DerivedBuilder &;

    //! Returns builder with relative kernel radius.
    auto withRelativeKernelRadius(double relativeKernelRadius) -> DerivedBuilder &;

protected:
    double _targetDensity = kWaterDensity;
    double _targetSpacing = 0.1;
    double _relativeKernelRadius = 1.8;
};

template<typename T>
auto SphSolverBuilderBase3<T>::withTargetDensity(double targetDensity) -> T &
{
    _targetDensity = targetDensity;
    return static_cast<T &>(*this);
}

template<typename T>
auto SphSolverBuilderBase3<T>::withTargetSpacing(double targetSpacing) -> T &
{
    _targetSpacing = targetSpacing;
    return static_cast<T &>(*this);
}

template<typename T>
auto SphSolverBuilderBase3<T>::withRelativeKernelRadius(double relativeKernelRadius) -> T &
{
    _relativeKernelRadius = relativeKernelRadius;
    return static_cast<T &>(*this);
}

//!
//! \brief Front-end to create SphSolver3 objects step by step.
//!
class SphSolver3::Builder final : public SphSolverBuilderBase3<SphSolver3::Builder>
{
public:
    //! Builds SphSolver3.
    auto build() const -> SphSolver3;

    //! Builds shared pointer of SphSolver3 instance.
    auto makeShared() const -> SphSolver3Ptr;
};

}  // namespace jet

#endif  // INCLUDE_JET_SPH_SOLVER3_H_
