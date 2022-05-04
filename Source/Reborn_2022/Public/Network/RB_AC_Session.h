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
  TObjectPtr<UTexture2D> Texture;

  FName SessionName;

};


UCLASS()
class REBORN_2022_API ARB_AC_Session : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARB_AC_Session();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
  TObjectPtr<UStaticMeshComponent> StaticMeshComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collisions")
  TObjectPtr<UCapsuleComponent> CapsuleComponent;
  

  //Floating widget
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextHud")
  TObjectPtr<UWidgetComponent> TextHudWidget;


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
  void OnEndSessionComplete(bool Successful);

  UFUNCTION()
  void OnDestroySessionComplete(bool Successful);

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
  int32 CurrentSessionIndex = -1;

  bool Initial = true;

  FMapInfo CurrentMap;
  int32 CurrentMapIndex = 0;

  FString NoSessionYet = TEXT("NULLNULLNULL");

  FName CurrentSessionName;
  FString CurrentMapName;
  FOnlineSessionSearchResult CurrentOnlineSessionSearchResult;

  AActor* CurrentActorForEndSession;

private:


};
