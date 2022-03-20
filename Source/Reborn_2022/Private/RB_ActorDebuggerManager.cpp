// Fill out your copyright notice in the Description page of Project Settings.


#include "RB_ActorDebuggerManager.h"
#include "Kismet/KismetSystemLibrary.h"

URB_ActorDebuggerManager::URB_ActorDebuggerManager()
{
  DebuggingDisabled = true;
  FString value1 = DebuggingDisabled ? TEXT("true") : TEXT("false");
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ActorDebuggerManager::URB_ActorDebuggerManager: DebuggingDisabled: ") + value1, true, false, FColor::Red, 6.0f);
}

void URB_ActorDebuggerManager::AddDebuggerActor(AActor* Actor)
{
  CurrentDebuggerActors.Add(Actor, true);
}


void URB_ActorDebuggerManager::PrintString(const UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration, AActor* Actor)
{
  if (DebuggingDisabled) {
    return;
  }

  bool ActorDebuggingEnabled = CurrentDebuggerActors[Actor];

  if (!ActorDebuggingEnabled) {
    return;
  }

  UKismetSystemLibrary::PrintString(WorldContextObject, InString, bPrintToScreen, bPrintToLog, TextColor, Duration);
}

void URB_ActorDebuggerManager::ClearDebuggedActors()
{
  CurrentDebuggerActors.Empty();
}

void URB_ActorDebuggerManager::SetDebugging(bool value)
{
  //FString result = FFrame::GetStackTrace();
  //FString value1 = value? TEXT("true") : TEXT("false");
  //FDebug::DumpStackTraceToLog(ELogVerbosity::All);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ActorDebuggerManager::SetDebugging: value: ")+ value1, true, false, FColor::Red, 6.0f);
  DebuggingDisabled = value;
  FString value1 = DebuggingDisabled ? TEXT("true") : TEXT("false");
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ActorDebuggerManager::SetDebugging: DebuggingDisabled: ") + value1, true, false, FColor::Red, 6.0f);
}


bool URB_ActorDebuggerManager::GetDebugging()
{
  return DebuggingDisabled;
}
