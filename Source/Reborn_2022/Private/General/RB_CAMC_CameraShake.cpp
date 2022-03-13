// Fill out your copyright notice in the Description page of Project Settings.


#include "General/RB_CAMC_CameraShake.h"

URB_CAMC_CameraShake::URB_CAMC_CameraShake() {
  OscillationDuration = 0.25f;
  OscillationBlendInTime = 0.05f;
  OscillationBlendOutTime = 0.05f;

  RotOscillation.Pitch.Amplitude = FMath::RandRange(3.0f, 5.0f);
  RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);

  RotOscillation.Yaw.Amplitude = FMath::RandRange(3.0f, 5.0f);
  RotOscillation.Yaw.Frequency = FMath::RandRange(25.0f, 35.0f);
}