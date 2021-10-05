/******************************************************************************
* License version                                                             *
*                                                                             *
* Authors:                                                                    *
* Contact information:                                                        *
******************************************************************************/
#pragma once

#include <SofaHapticAvatar/HapticAvatar_RigidDeviceController.h>
#include <sofa/core/collision/NarrowPhaseDetection.h>
#include <sofa/core/collision/Intersection.h>

namespace sofa::HapticAvatar
{

using namespace sofa::defaulttype;
using namespace sofa::simulation;

struct SOFA_HAPTICAVATAR_API HapticContact
{
    Vector3 m_toolPosition;
    Vector3 m_objectPosition;
    Vector3 m_normal;
    Vector3 m_force;
    SReal distance;
    int tool; //0 = shaft, 1 = upjaw, 2 = downJaw
};

/**
* Haptic Avatar driver
*/
class SOFA_HAPTICAVATAR_API HapticAvatar_RigidGrasperDeviceController : public HapticAvatar_RigidDeviceController
{
public:
    SOFA_CLASS(HapticAvatar_RigidGrasperDeviceController, HapticAvatar_RigidDeviceController);
    typedef type::vector<core::collision::DetectionOutput> ContactVector;

    /// Default constructor
    HapticAvatar_RigidGrasperDeviceController();

    /// handleEvent component method to catch collision info
    void handleEvent(core::objectmodel::Event *) override;
    
    /// General Haptic thread methods
    static void Haptics(std::atomic<bool>& terminate, void * p_this, void * p_driver);

    /// Thread methods to cpy data from m_hapticData to m_simuData
    static void CopyData(std::atomic<bool>& terminate, void * p_this);

protected:
    /// Internal method to init specific collision components
    void initImpl() override;

    /// override method to create the different threads
    bool createHapticThreads() override;

    /// override method to update specific tool position
    void updatePositionImpl() override;

    /// Internal method to retrieve the collision information per simulation step
    void retrieveCollisions();

    /// Internal method to draw specific informations
    void drawImpl(const sofa::core::visual::VisualParams*) override {}

public:
    /// collision distance value, need public to be accessed by haptic thread
    SReal m_distance;

    /// list of contact detected by the collision
    std::vector<HapticContact> contactsSimu, contactsHaptic;

protected:
    // Pointer to the scene detection Method component (Narrow phase only)
    sofa::core::collision::NarrowPhaseDetection* m_detectionNP;

    // Pointer to the scene intersection Method component
    sofa::core::collision::Intersection* m_intersectionMethod;
};

} // namespace sofa::HapticAvatar
