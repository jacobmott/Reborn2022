// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RB_AC_ActorDebuggerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARB_AC_ActorDebuggerManager::ARB_AC_ActorDebuggerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
  DebuggingDisabled = true;
  //FString value1 = DebuggingDisabled ? TEXT("true") : TEXT("false");
  SetReplicates(true);

  StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
  StaticMeshComp->SetupAttachment(RootComponent);

}


void ARB_AC_ActorDebuggerManager::AddDebuggerActor(AActor* Actor)
{
  CurrentDebuggerActors.Add(Actor, true);
}


void ARB_AC_ActorDebuggerManager::PrintString(const UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration, AActor* Actor)
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

void ARB_AC_ActorDebuggerManager::ClearDebuggedActors()
{
  CurrentDebuggerActors.Empty();
}

void ARB_AC_ActorDebuggerManager::SetDebugging_Implementation(bool value)
{
  //FString result = FFrame::GetStackTrace();
  //FString value1 = value? TEXT("true") : TEXT("false");
  //FDebug::DumpStackTraceToLog(ELogVerbosity::All);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_ActorDebuggerManager::SetDebugging: value: ")+ value1, true, false, FColor::Red, 6.0f);
  DebuggingDisabled = value;
  FString value1 = DebuggingDisabled ? TEXT("true") : TEXT("false");
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_ActorDebuggerManager::SetDebugging: DebuggingDisabled: ") + value1, true, false, FColor::Red, 6.0f);
}


bool ARB_AC_ActorDebuggerManager::GetDebugging()
{
  return DebuggingDisabled;
  
}




TMap<AActor*, bool> ARB_AC_ActorDebuggerManager::GetCurrentDebuggerActors()
{
  return CurrentDebuggerActors;
}

void ARB_AC_ActorDebuggerManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ARB_AC_ActorDebuggerManager, DebuggingDisabled);
}








// Called when the game starts or when spawned
void ARB_AC_ActorDebuggerManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void ARB_AC_ActorDebuggerManager::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ARB_AC_ActorDebuggerManager::OnRep_DebuggingDisabled()
{
  UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DebuggingDisabledEffect, GetActorLocation());
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_ActorDebuggerManager::OnRep_DebuggingDisabled: SpawnedEmiiter"), true, false, FColor::Red, 6.0f);
}
