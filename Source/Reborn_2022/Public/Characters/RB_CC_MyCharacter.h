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
class URB_ACC_HealthComponent;
class URB_UserWidget;
class UWidgetComponent;
class ARB_A_ActorUI;

struct ForwardTraceHitInformation {
  bool HadHit;
  FVector Start;
  FVector End;
  FHitResult HitResult;
  FRotator Rot;
  bool Error;
};


UCLASS()
class REBORN_2022_API ARB_CC_MyCharacter : public ACharacter
{
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ARB_CC_MyCharacter();
  
  //Test

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
  TObjectPtr<USpringArmComponent> SpringArmComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
  TObjectPtr<UCameraComponent> CameraComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
  TObjectPtr<UStaticMeshComponent> StaticMeshComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ActorUI")
  TObjectPtr<ARB_A_ActorUI> ActorUI;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
  TObjectPtr<URB_ACC_HealthComponent> HealthComponent;

  UFUNCTION()
  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);

  UPROPERTY(EditAnywhere, Category="Spawning")
  TSubclassOf<AActor> ActorToSpawn;

  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
  TSubclassOf<URB_UserWidget> HudClass;

  TObjectPtr<URB_UserWidget> Hud;

  void UpdateHud();
  void UpdateFloatingHealthHud();
  
  //Floating widget
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
  TObjectPtr<UWidgetComponent> MyHealthWidget;

  //Floating widget
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
  TObjectPtr<UWidgetComponent> MyHealthWidget2;


  //Floating widget
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
  TObjectPtr<UWidgetComponent> MyHealthWidget3;


  //Floating widget
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
  TObjectPtr<UWidgetComponent> MyHealthWidget4;


protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void MoveForward(float Value);
  void MoveRight(float Value);
  void TurnAtRate(float Value);
  void LookUpAtRate(float Value);
  void InteractPressed();

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
  float BaseTurnRate;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
  float BaseLookUpAtRate;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
  float TraceDistance;


  UFUNCTION(BlueprintNativeEvent)
  void TraceForward();
  void TraceForward_Implementation();

  UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Network")
  void FireForward();

  UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Network")
  void SpawnActorAtLocation();

  UFUNCTION()
  void ShowUIAtLocation();

  UFUNCTION()
  void FireForwardClientTrace();

  //UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Network")
  //void AddClientOnScreenDebugMessage(int32 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage);

  UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Network")
  void SpawnExplosion(FVector Location, FVector Scale);


  UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Network")
  void AddClientDrawDebugSphere(const UWorld* InWorld, FVector const& Center, float Radius, int32 Segments, FColor const& Color, bool bPersistentLines = false, float LifeTime = -1.f);


  FTimerHandle TimerHandle_Health;

  float Current = 0.0f;
  float MaxHealth = 100.0f;

  void SetPercent();

  //Impulse
  UPROPERTY(EditAnywhere, Category = "ImpulseForce")
  float ImpulseForce;

  //Radial Impulse
  UPROPERTY(EditAnywhere, Category = "RadialForce")
  bool ApplyRadialForce;
  UPROPERTY(EditAnywhere, Category = "RadialForce")
  float ImpactRadius;
  UPROPERTY(EditAnywhere, Category = "RadialForce")
  float RadialImpactForce;
  UPROPERTY(EditAnywhere, Category = "RadialForce")
  bool UseActorsCenterOfMassInCollisionCalculation;

  UPROPERTY(EditAnywhere, Category = "Camera")
  TSubclassOf<UMatineeCameraShake> CameraShake;

  UPROPERTY(EditAnywhere, Category = "Timeline")
  TObjectPtr<UCurveFloat> CurveFloat;


  UPROPERTY(EditAnywhere, Category = "RadialForce")
  TObjectPtr<UParticleSystem> RadialExplosionEffect;




public:	
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  virtual void Landed(const FHitResult& Hit) override;


private:
  TObjectPtr<AActor> FocusedActor;

  void CameraShakeDemo(float Scale);

  ForwardTraceHitInformation GetForwardTraceHitInformation(bool DrawDebugTraceLine);


  bool ShowUI = false;

  UPROPERTY()
  FVector StartScale;
  UPROPERTY()
  FVector TargetScale;
  UPROPERTY()
  FTimeline SquashTimeline;

  UFUNCTION() 
  void SquashProgress(float Value);

};
