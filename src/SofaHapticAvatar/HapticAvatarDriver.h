/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_HAPTICAVATAR_HAPTICAVATARDRIVER_H
#define SOFA_HAPTICAVATAR_HAPTICAVATARDRIVER_H

#include <SofaHapticAvatar/config.h>
#include <SofaHapticAvatar/HapticAvatarDefines.h>
#include <sofa/defaulttype/Vec.h>
#include <string>

namespace sofa 
{

namespace component 
{

namespace controller 
{

/**
* HapticAvatar driver
*/
class SOFA_HAPTICAVATAR_API HapticAvatarDriver
{
public:
    HapticAvatarDriver(const std::string& portName);

    virtual ~HapticAvatarDriver();

    bool IsConnected() { return m_connected; }

    /// Public API to device communication
    void connectDevice();


    /** Reset encoders, motor outputs, calibration flags, collision objects. 
    * Command: RESET
    * @param {int} mode: specify what to reset. See doc. 
    */
    int resetDevice(int mode = 15);

    /** Ask for the identity and build version of the firmware in the device.
    * Command: GET_IDENTITY
    */
    std::string getIdentity();

    /** To get an identification number of which tool is inserted (if any). The identification number is related to the length of the pin at the tip of the tool. 
    * Command: GET_TOOL_ID
    */
    int getToolID();

    /** To get the device status for calibration, amplifiers, hall effect sensors, battery, fan, power and board temperature. This is mainly for diagnostics and error detection. 
    * Command: GET_STATUS
    */
    int getDeviceStatus();



    /** get the angles of the port (Yaw + Pitch) and the insertion length and rotation of the tool.
    * Command: GET_ANGLES_AND_LENGTH
    * @returns {vec4f} {Tool rotation angle, Pitch angle, Pitch angle, Yaw angle}
    */
    sofa::helper::fixed_array<float, 4> getAngles_AndLength();

    /** Get the torque on the jaws around the jaw rotation pin. 
    * Command: GET_TOOL_JAW_TORQUE
    * @returns {float} The sum of the torque on the jaws 
    */
    float getJawTorque();

    /** Get the opening angle. 
    * Command: SET_TOOL_JAW_OPENING_ANGLE
    * @returns {float} The opening angle: 0.0 means fully closed. 1.0 means fully opened
    */
    float getJawOpeningAngle();



    /** Get information about the last pwm sent out to the motors. 
    * Command: GET_LAST_PWM
    * @returns {vec4f}  {Rot motor, Pitch motor, Z motor, Yaw motor} PWM values [-2040 .. 2040] 
    */
    sofa::helper::fixed_array<float, 4> getLastPWM();

    /** To get conversion factors from raw pwm values to torques or forces. 
    * Command: GET_MOTOR_SCALING_VALUES
    * @returns {vec4f} {Rot motor, Pitch motor, Z motor, Yaw motor} scaling factor (in Nmm/pwm-value) 
    */
    sofa::helper::fixed_array<float, 4> getMotorScalingValues();

    /** Get the most recent collision force sum. This is only related to collisions against primitives.
    * Command: GET_LAST_COLLISION_FORCE
    * @returns {vec3f} A vector (Fx, Fy, Fz) in the Device LCS
    */
    sofa::helper::fixed_array<float, 3> getLastCollisionForce();


   
    /** Set the force and torque output per motor. 
    * Command: SET_MOTOR_FORCE_AND_TORQUES
    * @param {vec4f} values: specify what to reset. See doc.
    */
    void setMotorForce_AndTorques(sofa::helper::fixed_array<float, 4> values);

    /** Set a force vector on the tool tip plus the tool rotation torque. This is an alternative way to output force (compared to SET_MOTOR_FORCE_AND_TORQUES) 
    * Command: SET_TIP_FORCE_AND_ROT_TORQUE
    * @param {vec3f} force: specify what to reset. See doc.
    * @param {float} RotTorque: specify what to reset. See doc.
    */
    void setTipForce_AndRotTorque(sofa::defaulttype::Vector3 force, float RotTorque);
    
       
    /** Will decompose a force vector in device coordinate system to compute torque and force to be sent to apply to the device. Using @sa writeRoughForce.
    * @param {vec3f} force: force vector to apply to the device in its coordinate space.
    */
    void setForceVector(sofa::defaulttype::Vector3 force);

    bool writeData(std::string msg);
    // Will send 0 torque and force values to the device. Using SET_MANUAL_PWM.
    void releaseForce();

    bool setSingleCommand(const std::string& cmdMsg, std::string& result);
    
    bool sendCommandToDevice(HapticAvatar::Cmd command, const std::string& arguments, char *result);

    std::string convertSingleData(char *buffer, bool forceRemoveEoL = false);

protected:
    int getDataImpl(char *buffer, bool do_flush);

    int ReadDataImpl(char *buffer, unsigned int nbChar, int *queue, bool do_flush);

    bool WriteDataImpl(char *buffer, unsigned int nbChar);

    // comparable to SET_MANUAL_PWM: Set the force output manually per motor with raw PWM-values. 
    void writeRoughForce(float rotTorque, float pitchTorque, float zforce, float yawTorque);

private:
    //Connection status
    bool m_connected;

    //Serial comm handler
    HANDLE m_hSerial;
    //Get various information about the connection
    COMSTAT m_status;
    //Keep track of last error
    DWORD m_errors;

    std::string m_portName;
};

} // namespace controller

} // namespace component

} // namespace sofa

#endif // SOFA_HAPTICAVATAR_HAPTICAVATARAPI_H
