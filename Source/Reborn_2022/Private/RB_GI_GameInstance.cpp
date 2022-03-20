// Fill out your copyright notice in the Description page of Project Settings.


#include "RB_GI_GameInstance.h"
#include "RB_ActorDebuggerManager.h"
#include "Kismet/KismetSystemLibrary.h"



// Singleton-like accessor. I only use this as a workaround for PIE
// The GameInstance lifecycle is different in PIE
// In proper builds, this wouldn't be necessary (I'll explain this further down)
URB_ActorDebuggerManager* URB_GI_GameInstance::ActorDebuggerManager()
{
  
  if (!IsValid(ActorDebuggerManagerInstance)){
    ActorDebuggerManagerInstance = NewObject<URB_ActorDebuggerManager>(this,FName("ActorDebuggerManager"));
  }

  return ActorDebuggerManagerInstance;
}

void URB_GI_GameInstance::Init()
{

  UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("URB_GI_GameInstance::Init: Address? = %p"), (void*)this), true, true, FColor::Red, 5.0f);
  ActorDebuggerManagerInstance = NewObject<URB_ActorDebuggerManager>(this, FName("ActorDebuggerManager"));
  //ActorDebuggerManagerInstance->SetDebugging(false);
}

// Cleanup. This acts as a quasi-destructor
void URB_GI_GameInstance::Shutdown()
{
  // Important, if you overwrite functionality in UE4.
  // Omitting this, may cause errors (depending on the class).
  Super::Shutdown();

  // PIE may cause this to be invalid. During the game, it will always be valid
  if (IsValid(ActorDebuggerManagerInstance))
  {
    ActorDebuggerManagerInstance->ClearDebuggedActors();
    //ActorDebuggerManagerInstance->ClearLevelStorage();
  }
}
