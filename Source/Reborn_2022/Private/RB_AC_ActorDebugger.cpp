// Fill out your copyright notice in the Description page of Project Settings.


#include "RB_AC_ActorDebugger.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RB_ActorDebuggerManager.h"
#include "RB_GI_GameInstance.h"

// Sets default values for this component's properties
URB_AC_ActorDebugger::URB_AC_ActorDebugger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
  //Set 5 values to 3.0f;
  DurationGroupPrintToScreen.Init(3.0f, 8);


  //EnableGroupPrintToScreen |= (uint8)EGroupDebugBits::EGDB_One;
  //EnableGroupPrintToScreen |= (uint8)EGroupDebugBits::EGDB_Two;
  //EnableGroupPrintToScreen |= (uint8)EGroupDebugBits::EGDB_Three;
  //EnableGroupPrintToScreen |= (uint8)EGroupDebugBits::EGDB_Four;

}


// Called when the game starts
void URB_AC_ActorDebugger::BeginPlay()
{
	Super::BeginPlay();
  URB_GI_GameInstance* GameInstance = Cast<URB_GI_GameInstance>(GetWorld()->GetGameInstance());
  ActorDebuggerManager = GameInstance->ActorDebuggerManager();
	// ...
	
}


// Called every frame
//void URB_AC_ActorDebugger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void URB_AC_ActorDebugger::PrintString(const UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, uint8 GroupId, AActor* Actor)
{
  if (EnableGroupPrintToScreen & GroupId) {
    GroupInfo GroupInfo = GetId(GroupId);
    int32 Id = GroupInfo.Id;
    FString StringId = GroupInfo.StringId;
    ActorDebuggerManager->PrintString(WorldContextObject, TEXT(": GROUP(") + StringId + TEXT("): ") + InString, bPrintToScreen, bPrintToLog, TextColor, DurationGroupPrintToScreen[Id], Actor);
  }
}


uint8 URB_AC_ActorDebugger::GetGroupEnumValue(int32 GroupId)
{
  uint8 GroupEnum = (uint8)EGroupDebugBits::EGDB_One;
  if (GroupId == 1) {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_One;
  }
  else if (GroupId == 2) {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_Two;
  }
  else if (GroupId == 3) {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_Three;
  }
  else if (GroupId == 4) {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_Four;
  }
  else if (GroupId == 5) {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_Five;
  }
  else if (GroupId == 6) {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_Six;
  }
  else if (GroupId == 7) {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_Seven;
  }
  else if (GroupId == 8) {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_Eight;
  }
  else  {
    GroupEnum = (uint8)EGroupDebugBits::EGDB_One;
  }
  return GroupEnum;
}

GroupInfo URB_AC_ActorDebugger::GetId(uint8 GroupId)
{

  GroupInfo GroupInfo;

  if (GroupId == (uint8)EGroupDebugBits::EGDB_One) {
    GroupInfo.Id = 0;
    GroupInfo.StringId = TEXT("EGDB_One");
  }
  else if (GroupId == (uint8)EGroupDebugBits::EGDB_Two) {
    GroupInfo.Id = 1;
    GroupInfo.StringId = TEXT("EGDB_Two");
  }
  else if (GroupId == (uint8)EGroupDebugBits::EGDB_Three) {
    GroupInfo.Id = 2;
    GroupInfo.StringId = TEXT("EGDB_Three");
  }
  else if (GroupId == (uint8)EGroupDebugBits::EGDB_Four) {
    GroupInfo.Id = 3;
    GroupInfo.StringId = TEXT("EGDB_Four");
  }
  else if (GroupId == (uint8)EGroupDebugBits::EGDB_Five) {
    GroupInfo.Id = 4;
    GroupInfo.StringId = TEXT("EGDB_Five");
  }
  else if (GroupId == (uint8)EGroupDebugBits::EGDB_Six) {
    GroupInfo.Id = 5;
    GroupInfo.StringId = TEXT("EGDB_Six");
  }
  else if (GroupId == (uint8)EGroupDebugBits::EGDB_Seven) {
    GroupInfo.Id = 6;
    GroupInfo.StringId = TEXT("EGDB_Seven");
  }
  else if (GroupId == (uint8)EGroupDebugBits::EGDB_Eight) {
    GroupInfo.Id = 7;
    GroupInfo.StringId = TEXT("EGDB_Eight");
  }
  else {
    GroupInfo.Id = 0;
    GroupInfo.StringId = TEXT("EGDB_One");
  }
  return GroupInfo;
}

void URB_AC_ActorDebugger::AddDebuggerActor(AActor* Actor)
{
  ActorDebuggerManager->AddDebuggerActor(Actor);
}

