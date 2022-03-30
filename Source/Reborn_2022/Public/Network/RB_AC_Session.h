// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OnlineSessionSettings.h"
#include "RB_AC_Session.generated.h"

class UStaticMeshComponent;
class URB_SessionSubsystem;
class UWidgetComponent;

UCLASS()
class REBORN_2022_API ARB_AC_Session : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARB_AC_Session();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
  UStaticMeshComponent* StaticMeshComp;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ServerConnect")
	bool ForServer;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ServerConnect")
  bool ForClient;


  UFUNCTION()
  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);

  UPROPERTY(EditAnywhere, Category = "Portal")
  TSubclassOf<AActor> PortalActorToSpawn;

  //Floating widget
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SessionList")
  UWidgetComponent* SessionListWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	URB_SessionSubsystem* RB_SessionSubsystem;
 
  UFUNCTION()
	void OnCreateSessionComplete(bool Successful);


  void UpdateFloatingTextHud(FOnlineSessionSearchResult& CurrentSessionSearchResult)''

  UFUNCTION()
  void OnStartSessionComplete(bool Successful);

	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);

	TMap<FString, FOnlineSessionSearchResult> Sessions;

  FString CurrentSessionSearchResult;





};
