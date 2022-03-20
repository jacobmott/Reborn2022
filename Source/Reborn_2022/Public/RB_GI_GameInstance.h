// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RB_GI_GameInstance.generated.h"


class URB_ActorDebuggerManager;

/**
 * 
 */
UCLASS()
class REBORN_2022_API URB_GI_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
  // Accessor for C++ & BP. Should be self-explanatory
  UFUNCTION(BlueprintPure, Category = "Debugger", meta = (DisplayName = "Get Debugger Manager",Keywords = "DebuggerManager"))
  URB_ActorDebuggerManager* ActorDebuggerManager();

  // Needs to be an UPROPERTY(), so the GC does its thing
  // Transient, so it doesn't get serialized itself. Will be nulled when the
  // game starts. Initialization happens later by hand.
  UPROPERTY(EditAnywhere, Category = "Debugger")
  URB_ActorDebuggerManager* ActorDebuggerManagerInstance;


  /** virtual function to allow custom GameInstances an opportunity to set up what it needs */
  UFUNCTION(BlueprintCallable, Category = "GameInstance")
  virtual void Init() override;

protected:
  // This is where we will clean up, as the game is shut down
  virtual void Shutdown() override;


private:
  // Needs to be an UPROPERTY(), so the GC does its thing
  // Transient, so it doesn't get serialized itself. Will be nulled when the
  // game starts. Initialization happens later by hand.
  //UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  //URB_ActorDebuggerManager* ActorDebuggerManagerInstance;




};
