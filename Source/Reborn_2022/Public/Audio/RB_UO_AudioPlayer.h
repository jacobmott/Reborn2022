// Copyright Vortex Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "UObject/ObjectMacros.h"
#include "RB_UO_AudioPlayer.generated.h"


class UAudioComponent;
class USoundCue;

/**
 * 
 */
UCLASS(Blueprintable)
class REBORN_2022_API URB_UO_AudioPlayer : public UObject
{
	GENERATED_BODY()

public:
	URB_UO_AudioPlayer();

  //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
  UFUNCTION(BlueprintCallable, Category = "Audio")
  void BeginPlay(USoundCue* MainSoundTrackCueParam);


  //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
  TObjectPtr<UAudioComponent> AudioComponent;


  //UPROPERTY(EditAnywhere, Category = "Audio")
  TObjectPtr<USoundCue> MainSoundTrackCue;

  void PauseMusic();
  void StopMusic();

protected:

  UFUNCTION()
  void OnAudioFinishedOrStoped();

private:
  bool IsPaused = false;
  bool DidClickStop = false;


	
};
