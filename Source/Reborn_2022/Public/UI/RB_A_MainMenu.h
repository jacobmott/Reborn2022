// Copyright Vortex Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/RB_A_ActorUI.h"
#include "RB_A_MainMenu.generated.h"

class URB_UO_AudioPlayer;
class USoundCue;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class REBORN_2022_API ARB_A_MainMenu : public ARB_A_ActorUI
{
	GENERATED_BODY()
	
public:
  // Sets default values for this actor's properties
  ARB_A_MainMenu();

  //Floating widget
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenuHud")
  TObjectPtr<UWidgetComponent> MainMenuHudWidget;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
  TObjectPtr<USoundCue> MainSoundTrackCue;


protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
  TObjectPtr<URB_UO_AudioPlayer> AudioPlayer;

  virtual void UpdateFloatingTextHud(float DeltaTime);

  UFUNCTION()
  void OnHoveredBTNMusic();
  UFUNCTION()
  void OnHoveredBTNFps120();

  UFUNCTION()
  void OnClickedBTNMusic();
  UFUNCTION()
  void OnClickedBTNFps120();


  UButton* BTNMusic;
  UButton* BTNFps120;
  UTextBlock* TBMusic;
  UTextBlock* TBFps120;


};
