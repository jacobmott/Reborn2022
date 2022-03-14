// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "RB_CC_MyCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UMatineeCameraShake;
class UCurveFloat;

struct ForwardTraceHitInformation {
  bool HadHit;
  FVector Start;
  FVector End;
  FHitResult HitResult;
  FRotator Rot;
};

UCLASS()
class REBORN_2022_API ARB_CC_MyCharacter : public ACharacter
{
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ARB_CC_MyCharacter();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
  USpringArmComponent* SpringArmComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
  UCameraComponent* CameraComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player");
  UStaticMeshComponent* StaticMeshComp;

  UFUNCTION()
  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);


  UPROPERTY(EditAnywhere, Category="Spawning")
  TSubclassOf<AActor> ActorToSpawn;


protected:
  // Called when the game starts or when spawned
  //virtual void BeginPlay() override;

  void MoveForward(float Value);
  void MoveRight(float Value);
  void TurnAtRate(float Value);
  void LookUpAtRate(float Value);
  void InteractPressed();

  UFUNCTION()
  void SpawnActor();

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera");
  float BaseTurnRate;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera");
  float BaseLookUpAtRate;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction");
  float TraceDistance;


  UFUNCTION(BlueprintNativeEvent)
  void TraceForward();
  void TraceForward_Implementation();


  //Impulse
  UPROPERTY(EditAnywhere, Category = "ImpulseForce");
  float ImpulseForce;
  void FireForward();

  //Radial Impulse
  UPROPERTY(EditAnywhere, Category = "RadialForce");
  bool ApplyRadialForce;
  UPROPERTY(EditAnywhere, Category = "RadialForce");
  float ImpactRadius;
  UPROPERTY(EditAnywhere, Category = "RadialForce");
  float RadialImpactForce;
  UPROPERTY(EditAnywhere, Category = "RadialForce");
  bool UseActorsCenterOfMassInCollisionCalculation;

  UPROPERTY(EditAnywhere, Category = "Camera");
  TSubclassOf<UMatineeCameraShake> CameraShake;

  UPROPERTY(EditAnywhere, Category = "Timeline")
  UCurveFloat* CurveFloat;

public:	
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  virtual void Landed(const FHitResult& Hit) override;


private:
  AActor* FocusedActor;

  void CameraShakeDemo(float Scale);

  ForwardTraceHitInformation GetForwardTraceHitInformation();

  void SpawnActorAtLocation(FVector Location, FRotator Rotation);

  UPROPERTY()
  FVector StartScale;
  UPROPERTY()
  FVector TargetScale;
  UPROPERTY()
  FTimeline SquashTimeline;

  UFUNCTION() 
  void SquashProgress(float Value);

};
