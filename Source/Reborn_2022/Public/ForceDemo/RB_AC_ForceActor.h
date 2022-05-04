// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RB_AC_ForceActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class REBORN_2022_API ARB_AC_ForceActor : public AActor
{
  GENERATED_BODY()
  
public:	
  // Sets default values for this actor's properties
  ARB_AC_ForceActor();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force Cube");
  TObjectPtr<UStaticMeshComponent> StaticMeshComp;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:	
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UPROPERTY(EditAnywhere)
  float ForceStrength;

};
