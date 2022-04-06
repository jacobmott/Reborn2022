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
class UCapsuleComponent;

UCLASS()
class REBORN_2022_API ARB_AC_Session : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARB_AC_Session();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
  UStaticMeshComponent* StaticMeshComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collisions")
  UCapsuleComponent* CapsuleComponent;
  

  //Floating widget
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextHud")
  UWidgetComponent* TextHudWidget;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  URB_SessionSubsystem* RB_SessionSubsystem;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;


protected:
  /* Handle to manage the timer */
  FTimerHandle SearchSessionsTimerHandle;
  FTimerHandle UpdateCurrentSessionTimerHandle;


  FTimerHandle RotateMapSelectionTimerHandle;

  UFUNCTION()
	void OnCreateSessionComplete(bool Successful);

  UFUNCTION()
  void RefreshSessionsList();

  UFUNCTION()
  void RefreshAndRotateCurrentSession();

  void OnJoinGameSessionComplete(EOnJoinSessionCompleteResult::Type Result);

  virtual void UpdateFloatingImageHud();
  virtual void UpdateFloatingTextHud();


	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);

	TMap<FString, bool> SessionsMap;
  TArray<FOnlineSessionSearchResult> SessionsArray;
  FOnlineSessionSearchResult CurrentSelectedSession;
  FString CurrentSelectedSessionId = FString(TEXT("NULLNULLNULL"));
  int32 CurrentSessionIndex = 0;

  bool Initial = true;


  FString NoSessionYet = TEXT("NULLNULLNULL");

private:


};
