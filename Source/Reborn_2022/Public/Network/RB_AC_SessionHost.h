// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Network/RB_AC_Session.h"
#include "RB_AC_SessionHost.generated.h"

class UTexture2D;


USTRUCT(BlueprintType)
struct FMapInfo
{
  GENERATED_BODY();

public:
  /* Display name for Map */
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FString MapName;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  UTexture2D* Texture;
  
};


/**
 * 
 */
UCLASS()
class REBORN_2022_API ARB_AC_SessionHost : public ARB_AC_Session
{
	GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ARB_AC_SessionHost();


  UFUNCTION()
  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);


  UPROPERTY(EditAnywhere, meta = (TitleProperty = "MapName"))
  TArray<FMapInfo> Maps;


protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  virtual void UpdateFloatingTextHud() override;


  UFUNCTION()
  void OnStartSessionComplete(bool Successful);

  UFUNCTION()
  void RotateMapSelection();


private:
  FMapInfo CurrentMap;
  int32 CurrentMapIndex = 0;
	
};
