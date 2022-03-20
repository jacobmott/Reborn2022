// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RB_ActorDebuggerManager.generated.h"

/**
 * 
 */
UCLASS()
class REBORN_2022_API URB_ActorDebuggerManager : public UObject
{
	GENERATED_BODY()

public:

  URB_ActorDebuggerManager();

  void AddDebuggerActor(AActor* Actor);

  void PrintString(const UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration, AActor* Actor);

  void ClearDebuggedActors();

  UFUNCTION(BlueprintCallable, Category = "Debug")
  void SetDebugging(bool value);

  UFUNCTION(BlueprintCallable, Category = "Debug")
  bool GetDebugging();

private:
  TMap<AActor*, bool> CurrentDebuggerActors;
  bool DebuggingDisabled;

};
