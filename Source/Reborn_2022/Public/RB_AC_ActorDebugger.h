// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RB_AC_ActorDebugger.generated.h"

class URB_ActorDebuggerManager;

//https://scylardor.fr/2020/09/02/unreal-engine-tips-c-bitflags-enums/
UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EGroupDebugBits : uint8
{
  EGDB_None = 0 UMETA(Hidden),
  EGDB_One = 1,
  EGDB_Two = 2,
  EGDB_Three = 4,
  EGDB_Four = 8,
  EGDB_Five = 16,
  EGDB_Six = 32,
  EGDB_Seven = 64,
  EGDB_Eight = 128
};

struct GroupInfo {
  int32 Id = 0;
  FString StringId = "";
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBORN_2022_API URB_AC_ActorDebugger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URB_AC_ActorDebugger();

  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Array")
  UPROPERTY(EditAnywhere, Category = "Debug")
  TArray<float> DurationGroupPrintToScreen;

  /*~ This property lists flags matching the names of values from EColorBits. */
  UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EGroupDebugBits"), Category = "Debug")
  uint8 EnableGroupPrintToScreen = 255;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  uint8 GetGroupEnumValue(int32 GroupId);

  void PrintString(const UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, uint8 GroupId, AActor* Actor);

  GroupInfo GetId(uint8 GroupId);

  void AddDebuggerActor(AActor* Actor);

private:
  URB_ActorDebuggerManager* ActorDebuggerManager;




		
};
