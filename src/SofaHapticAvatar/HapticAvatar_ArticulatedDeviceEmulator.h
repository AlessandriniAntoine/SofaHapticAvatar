/******************************************************************************
* License version                                                             *
*                                                                             *
* Authors:                                                                    *
* Contact information:                                                        *
******************************************************************************/
#pragma once

#include <SofaHapticAvatar/HapticAvatar_BaseDeviceController.h>
#include <SofaHapticAvatar/HapticAvatar_IBoxController.h>

#include <atomic>

namespace sofa::HapticAvatar
{

/**
* Haptic Avatar driver
*/
class SOFA_HAPTICAVATAR_API HapticAvatar_ArticulatedDeviceEmulator : public HapticAvatar_BaseDeviceController
{
public:
    SOFA_CLASS(HapticAvatar_ArticulatedDeviceEmulator, HapticAvatar_BaseDeviceController);

    typedef Vec1Types::Coord Articulation;
    typedef Vec1Types::VecCoord VecArticulation;
    typedef Vec1Types::VecDeriv VecArtiDeriv;
    typedef sofa::component::controller::LCPForceFeedback<sofa::defaulttype::Vec1dTypes> LCPForceFeedback1D;

    /// Default constructor
    HapticAvatar_ArticulatedDeviceEmulator();


    /// Method to handle various event like keyboard or omni.
    void handleEvent(sofa::core::objectmodel::Event* event) override;

    void moveRotationAxe1(Articulation value);
    void moveRotationAxe2(Articulation value);
    void moveRotationAxe3(Articulation value);
    void moveTranslationAxe1(Articulation value);
    void openJaws(Articulation value);

    Data<VecArticulation> d_articulations;

    /// link to the IBox controller component 
    SingleLink<HapticAvatar_ArticulatedDeviceEmulator, HapticAvatar_IBoxController, BaseLink::FLAG_STOREPATH | BaseLink::FLAG_STRONGLINK> l_iboxCtrl;
    /// Pointer to the ForceFeedback component
    LCPForceFeedback1D::SPtr m_forceFeedback1D;

    /// General Haptic thread methods
    static void Haptics(std::atomic<bool>& terminate, void * p_this, void * p_driver);

    /// Thread methods to cpy data from m_hapticData to m_simuData
    static void CopyData(std::atomic<bool>& terminate, void * p_this);

protected:
    /// Pointer to the IBoxController component
    HapticAvatar_IBoxController * m_iboxCtrl;

    /// override method to create the different threads
    bool createHapticThreads() override;

    /// override method to update specific tool position
    void updatePositionImpl() override;

    /// Internal method to init specific collision components
    void initImpl() override;
};

}
