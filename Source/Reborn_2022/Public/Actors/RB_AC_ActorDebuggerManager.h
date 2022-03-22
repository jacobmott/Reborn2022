// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RB_AC_ActorDebuggerManager.generated.h"

class UStaticMeshComponent;

UCLASS()
class REBORN_2022_API ARB_AC_ActorDebuggerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARB_AC_ActorDebuggerManager();


  void AddDebuggerActor(AActor* Actor);

  void PrintString(const UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration, AActor* Actor);

  void ClearDebuggedActors();

  UFUNCTION(Server, unreliable, BlueprintCallable, Category = "Debugger")
  void SetDebugging(bool value);

  UFUNCTION(BlueprintCallable, Category = "Debugger")
  bool GetDebugging();

  UFUNCTION(BlueprintCallable, Category = "Debugger")
  TMap<AActor*, bool> GetCurrentDebuggerActors();

  /* Change this when the game starts via menus, not via the blueprint template/class, we change this on the instance during runtime/play*/
  UPROPERTY(ReplicatedUsing = OnRep_DebuggingDisabled, VisibleAnywhere, BlueprintReadOnly, Category = "Debugger");
  bool DebuggingDisabled = true;

  UFUNCTION(BlueprintCallable)
  void OnRep_DebuggingDisabled();

  UPROPERTY(EditDefaultsOnly, Category = "FX")
  UParticleSystem* DebuggingDisabledEffect;


  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player");
  UStaticMeshComponent* StaticMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
  TMap<AActor*, bool> CurrentDebuggerActors;


};
