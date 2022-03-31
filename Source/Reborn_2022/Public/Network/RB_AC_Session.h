// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "RB_AC_Session.generated.h"

class UStaticMeshComponent;
class URB_SessionSubsystem;
class UWidgetComponent;


UENUM()
enum SESSION_ACTOR_TYPE
{
  Client     UMETA(DisplayName = "Client"),
  Host       UMETA(DisplayName = "Host"),
};

UCLASS()
class REBORN_2022_API ARB_AC_Session : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARB_AC_Session();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
  UStaticMeshComponent* StaticMeshComp;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
  TEnumAsByte<SESSION_ACTOR_TYPE> SessionActorType;


  UFUNCTION()
  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);

  //UPROPERTY(EditAnywhere, Category = "Portal")
  //TSubclassOf<AActor> PortalActorToSpawn;

  //Floating widget
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentSession")
  UWidgetComponent* SessionListWidget;

  //Floating widget
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SessionActorType")
  UWidgetComponent* SessionActorTypeWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	URB_SessionSubsystem* RB_SessionSubsystem;


  /* Handle to manage the timer */
  FTimerHandle SearchSessionsTimerHandle;
 
  UFUNCTION()
	void OnCreateSessionComplete(bool Successful);

  UFUNCTION()
  void RefreshSessionsList();

  void OnJoinGameSessionComplete(EOnJoinSessionCompleteResult::Type Result);


  void UpdateFloatingTextHud();
  void UpdateFloatingTextTypeHud();

  UFUNCTION()
  void OnStartSessionComplete(bool Successful);

	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);

	TMap<FString, FOnlineSessionSearchResult> Sessions;
  FOnlineSessionSearchResult CurrentSelectedSession;
  FString CurrentSelectedSessionId = FString(TEXT("NULLNULLNULL"));

  float ElapsedTime = 0.0f;


};
