// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact/InteractableBase.h"
#include "RB_AC_ActorDebugger.h"


static const int32 DEBUG_ALL = 1;
static const int32 DEBUG_OTHER = 2;
static const int32 DEBUG_INTERACT = 3;
static int32 DebugInteractableBase = 0;
FAutoConsoleVariableRef CVARDebugInteractableBase(TEXT("Reborn.DebugInteractableBase"), DebugInteractableBase, TEXT("Draw debug info for InteractableBase"), ECVF_Cheat);

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorDebuggerComp = CreateDefaultSubobject<URB_AC_ActorDebugger>(TEXT("ActorDebuggerComp"));

}

void AInteractableBase::TimelineProgress(float Value)
{
  FVector NewLocation = FMath::Lerp(StartLoc, EndLoc, Value);
  SetActorLocation(NewLocation);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	ActorDebuggerComp->AddDebuggerActor(this);

	if (CurveFloat) {
		FOnTimelineFloat TimelineProgress;
    TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(true);

		StartLoc = EndLoc = GetActorLocation();
    EndLoc.Z += ZOffset;

    CurveTimeline.PlayFromStart();
	}
	
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  CurveTimeline.TickTimeline(DeltaTime);

}

void AInteractableBase::OnInteract_Implementation(AActor* Caller)
{
	//if (DebugInteractableBase == DEBUG_INTERACT || DebugInteractableBase == DEBUG_ALL) {
  //  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("On Interact"));
  //}
	ActorDebuggerComp->PrintString(GetWorld(), TEXT("On Interact"), true, false, FColor::Green, ActorDebuggerComp->GetGroupEnumValue(1), this);
	Destroy();
}

void AInteractableBase::StartFocus_Implementation()
{

  //if (DebugInteractableBase == DEBUG_INTERACT || DebugInteractableBase == DEBUG_ALL) {
  //  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Start Focus"));
  //}
	ActorDebuggerComp->PrintString(GetWorld(), TEXT("Start Focus"), true, false, FColor::Green, ActorDebuggerComp->GetGroupEnumValue(1), this);
}

void AInteractableBase::EndFocus_Implementation()
{
  //if (DebugInteractableBase == DEBUG_INTERACT || DebugInteractableBase == DEBUG_ALL) {
  //  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("End Focus"));
  //}
	ActorDebuggerComp->PrintString(GetWorld(), TEXT("End Focus"), true, false, FColor::Green, ActorDebuggerComp->GetGroupEnumValue(1), this);
}

