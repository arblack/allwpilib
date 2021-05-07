// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <wpi/math>

#include "frc/geometry/Translation2d.h"
#include "frc/kinematics/MecanumDriveKinematics.h"
#include "gtest/gtest.h"
#include "units/angular_velocity.h"

using namespace frc;

class MecanumDriveKinematicsTest : public ::testing::Test {
 protected:
  Translation2d m_fl{12_m, 12_m};
  Translation2d m_fr{12_m, -12_m};
  Translation2d m_bl{-12_m, 12_m};
  Translation2d m_br{-12_m, -12_m};

  MecanumDriveKinematics kinematics{m_fl, m_fr, m_bl, m_br};
};

TEST_F(MecanumDriveKinematicsTest, StraightLineInverseKinematics) {
  ChassisSpeeds speeds{5_mps, 0_mps, 0_rad_per_s};
  auto moduleStates = kinematics.ToWheelSpeeds(speeds);

  EXPECT_NEAR(5.0, moduleStates.frontLeft.to<double>(), 0.1);
  EXPECT_NEAR(5.0, moduleStates.frontRight.to<double>(), 0.1);
  EXPECT_NEAR(5.0, moduleStates.rearLeft.to<double>(), 0.1);
  EXPECT_NEAR(5.0, moduleStates.rearRight.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, StraightLineForwardKinematics) {
  MecanumDriveWheelSpeeds wheelSpeeds{5_mps, 5_mps, 5_mps, 5_mps};
  auto chassisSpeeds = kinematics.ToChassisSpeeds(wheelSpeeds);

  EXPECT_NEAR(5.0, chassisSpeeds.vx.to<double>(), 0.1);
  EXPECT_NEAR(0.0, chassisSpeeds.vy.to<double>(), 0.1);
  EXPECT_NEAR(0.0, chassisSpeeds.omega.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, StrafeInverseKinematics) {
  ChassisSpeeds speeds{0_mps, 4_mps, 0_rad_per_s};
  auto moduleStates = kinematics.ToWheelSpeeds(speeds);

  EXPECT_NEAR(-4.0, moduleStates.frontLeft.to<double>(), 0.1);
  EXPECT_NEAR(4.0, moduleStates.frontRight.to<double>(), 0.1);
  EXPECT_NEAR(4.0, moduleStates.rearLeft.to<double>(), 0.1);
  EXPECT_NEAR(-4.0, moduleStates.rearRight.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, StrafeForwardKinematics) {
  MecanumDriveWheelSpeeds wheelSpeeds{-5_mps, 5_mps, 5_mps, -5_mps};
  auto chassisSpeeds = kinematics.ToChassisSpeeds(wheelSpeeds);

  EXPECT_NEAR(0.0, chassisSpeeds.vx.to<double>(), 0.1);
  EXPECT_NEAR(5.0, chassisSpeeds.vy.to<double>(), 0.1);
  EXPECT_NEAR(0.0, chassisSpeeds.omega.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, RotationInverseKinematics) {
  ChassisSpeeds speeds{0_mps, 0_mps,
                       units::radians_per_second_t(2 * wpi::math::pi)};
  auto moduleStates = kinematics.ToWheelSpeeds(speeds);

  EXPECT_NEAR(-150.79644737, moduleStates.frontLeft.to<double>(), 0.1);
  EXPECT_NEAR(150.79644737, moduleStates.frontRight.to<double>(), 0.1);
  EXPECT_NEAR(-150.79644737, moduleStates.rearLeft.to<double>(), 0.1);
  EXPECT_NEAR(150.79644737, moduleStates.rearRight.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, RotationForwardKinematics) {
  MecanumDriveWheelSpeeds wheelSpeeds{-150.79644737_mps, 150.79644737_mps,
                                      -150.79644737_mps, 150.79644737_mps};
  auto chassisSpeeds = kinematics.ToChassisSpeeds(wheelSpeeds);

  EXPECT_NEAR(0.0, chassisSpeeds.vx.to<double>(), 0.1);
  EXPECT_NEAR(0.0, chassisSpeeds.vy.to<double>(), 0.1);
  EXPECT_NEAR(2 * wpi::math::pi, chassisSpeeds.omega.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, MixedRotationTranslationInverseKinematics) {
  ChassisSpeeds speeds{2_mps, 3_mps, 1_rad_per_s};
  auto moduleStates = kinematics.ToWheelSpeeds(speeds);

  EXPECT_NEAR(-25.0, moduleStates.frontLeft.to<double>(), 0.1);
  EXPECT_NEAR(29.0, moduleStates.frontRight.to<double>(), 0.1);
  EXPECT_NEAR(-19.0, moduleStates.rearLeft.to<double>(), 0.1);
  EXPECT_NEAR(23.0, moduleStates.rearRight.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, MixedRotationTranslationForwardKinematics) {
  MecanumDriveWheelSpeeds wheelSpeeds{-17.677670_mps, 20.506097_mps,
                                      -13.435_mps, 16.26_mps};

  auto chassisSpeeds = kinematics.ToChassisSpeeds(wheelSpeeds);

  EXPECT_NEAR(1.41335, chassisSpeeds.vx.to<double>(), 0.1);
  EXPECT_NEAR(2.1221, chassisSpeeds.vy.to<double>(), 0.1);
  EXPECT_NEAR(0.707, chassisSpeeds.omega.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, OffCenterRotationInverseKinematics) {
  ChassisSpeeds speeds{0_mps, 0_mps, 1_rad_per_s};
  auto moduleStates = kinematics.ToWheelSpeeds(speeds, m_fl);

  EXPECT_NEAR(0, moduleStates.frontLeft.to<double>(), 0.1);
  EXPECT_NEAR(24.0, moduleStates.frontRight.to<double>(), 0.1);
  EXPECT_NEAR(-24.0, moduleStates.rearLeft.to<double>(), 0.1);
  EXPECT_NEAR(48.0, moduleStates.rearRight.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, OffCenterRotationForwardKinematics) {
  MecanumDriveWheelSpeeds wheelSpeeds{0_mps, 16.971_mps, -16.971_mps,
                                      33.941_mps};
  auto chassisSpeeds = kinematics.ToChassisSpeeds(wheelSpeeds);

  EXPECT_NEAR(8.48525, chassisSpeeds.vx.to<double>(), 0.1);
  EXPECT_NEAR(-8.48525, chassisSpeeds.vy.to<double>(), 0.1);
  EXPECT_NEAR(0.707, chassisSpeeds.omega.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest,
       OffCenterTranslationRotationInverseKinematics) {
  ChassisSpeeds speeds{5_mps, 2_mps, 1_rad_per_s};
  auto moduleStates = kinematics.ToWheelSpeeds(speeds, m_fl);

  EXPECT_NEAR(3.0, moduleStates.frontLeft.to<double>(), 0.1);
  EXPECT_NEAR(31.0, moduleStates.frontRight.to<double>(), 0.1);
  EXPECT_NEAR(-17.0, moduleStates.rearLeft.to<double>(), 0.1);
  EXPECT_NEAR(51.0, moduleStates.rearRight.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest,
       OffCenterTranslationRotationForwardKinematics) {
  MecanumDriveWheelSpeeds wheelSpeeds{2.12_mps, 21.92_mps, -12.02_mps,
                                      36.06_mps};
  auto chassisSpeeds = kinematics.ToChassisSpeeds(wheelSpeeds);

  EXPECT_NEAR(12.02, chassisSpeeds.vx.to<double>(), 0.1);
  EXPECT_NEAR(-7.07, chassisSpeeds.vy.to<double>(), 0.1);
  EXPECT_NEAR(0.707, chassisSpeeds.omega.to<double>(), 0.1);
}

TEST_F(MecanumDriveKinematicsTest, NormalizeTest) {
  MecanumDriveWheelSpeeds wheelSpeeds{5_mps, 6_mps, 4_mps, 7_mps};
  wheelSpeeds.Normalize(5.5_mps);

  double kFactor = 5.5 / 7.0;

  EXPECT_NEAR(wheelSpeeds.frontLeft.to<double>(), 5.0 * kFactor, 1E-9);
  EXPECT_NEAR(wheelSpeeds.frontRight.to<double>(), 6.0 * kFactor, 1E-9);
  EXPECT_NEAR(wheelSpeeds.rearLeft.to<double>(), 4.0 * kFactor, 1E-9);
  EXPECT_NEAR(wheelSpeeds.rearRight.to<double>(), 7.0 * kFactor, 1E-9);
}
