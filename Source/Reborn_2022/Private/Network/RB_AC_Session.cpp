// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RB_AC_Session.h"

#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Subsystems/RB_SessionSubsystem.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Characters/RB_CC_MyCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

#include "Characters/RB_CC_MyCharacter.h"

// Sets default values
ARB_AC_Session::ARB_AC_Session()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
  RootComponent = CapsuleComponent;

  StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
  StaticMeshComp->SetupAttachment(RootComponent);

  bReplicates = false;

  TextHudWidget = CreateDefaultSubobject<UWidgetComponent>("TextHudWidget");
  TextHudWidget->SetupAttachment(RootComponent);
  static ConstructorHelpers::FClassFinder<UUserWidget> TextHudWidgetObj(TEXT("/Game/MyStuff/Blueprints/Widgets/BP_W_FloatingText"));
  if (TextHudWidgetObj.Succeeded()) {
    UE_LOG(LogTemp, Warning, TEXT("ARB_AC_Session:ARB_AC_Session.Found UserWidget Floating text"));
    TextHudWidget->SetWidgetClass(TextHudWidgetObj.Class);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("TextHudWidgetObj widget could not initialize on ARB_AC_Session"));
  }

}               


void ARB_AC_Session::UpdateFloatingTextHud()
{


}

// Called when the game starts or when spawned
void ARB_AC_Session::BeginPlay()
{
	Super::BeginPlay();

  UGameInstance* TGameInstance = GetGameInstance();
  RB_SessionSubsystem = TGameInstance->GetSubsystem<URB_SessionSubsystem>();
  if (!RB_SessionSubsystem) {
    return;
  }

  CurrentSelectedSessionId = NoSessionYet;
  TextHudWidget->SetTwoSided(false);

}


void ARB_AC_Session::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{

  if (!Successful || SessionResults.Num() <= 0) {
    SessionsArray = SessionResults;
    GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_Session::RefreshSessionsList, 2.0f, false, 2.0f);
    return;
  }

  SessionsArray = SessionResults;
  GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_Session::RefreshSessionsList, 2.0f, false, 2.0f);
}


void ARB_AC_Session::OnCreateSessionComplete(bool Successful)
{

  if (!Successful) {
    return;
  }

  RB_SessionSubsystem->StartSession(CurrentSessionName);
}

void ARB_AC_Session::RefreshSessionsList()
{

  RB_SessionSubsystem->FindSessions(10, false);

}

void ARB_AC_Session::RefreshAndRotateCurrentSession()
{

  if (SessionsArray.Num() <= 0) {
    CurrentMap.MapName = TEXT("");
    CurrentMap.SessionName = FName(*CurrentMap.MapName);
    CurrentSelectedSession = FOnlineSessionSearchResult();
    UpdateFloatingTextHud();
    return;
  }


  //if (CurrentSelectedSessionId.Equals(NoSessionYet)) {
  //  CurrentSelectedSession = SessionsArray[0];
  //  CurrentSelectedSessionId = CurrentSelectedSession.Session.GetSessionIdStr();
  //  CurrentMap.MapName = TEXT("NoSession");
  //}
  //if (CurrentSelectedSessionId.Equals(NoSessionYet)) { 
    CurrentSessionIndex += 1;
    if (CurrentSessionIndex >= SessionsArray.Num()) {
      CurrentSessionIndex = 0;
    }
    CurrentSelectedSession = SessionsArray[CurrentSessionIndex];
    //CurrentSelectedSession.
    CurrentSelectedSessionId = CurrentSelectedSession.Session.GetSessionIdStr();
    FString MapName;
    if (CurrentSelectedSession.Session.SessionSettings.Get(SETTING_MAPNAME, MapName)){
      UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Yes")+ MapName, true, false, FColor::Blue, 20.0f);
    }
    else {
      UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No ")+MapName, true, false, FColor::Blue, 20.0f);
    }
    FString SessionName;
    if (CurrentSelectedSession.Session.SessionSettings.Get(FName(TEXT("SESSION_NAME")), SessionName)) {
      UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SessionName Yes") + SessionName, true, false, FColor::Blue, 20.0f);
    }
    else {
      UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SessionName No ") + SessionName, true, false, FColor::Blue, 20.0f);
    }

    CurrentMap.MapName = MapName;
    CurrentMap.SessionName = FName(*SessionName);


  //}

  UpdateFloatingTextHud();
}



void ARB_AC_Session::OnEndSessionComplete(bool Successful)
{


  //RB_SessionSubsystem->TryTravelToSession(CurrentSelectedSession);
}

void ARB_AC_Session::OnDestroySessionComplete(bool Successful)
{
  ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(CurrentActorForEndSession);
  APlayerController* MyCharacterController = Cast<APlayerController>(MyCharacter->GetController());
  APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  FString MapName = "Reborn_2022_MainLobby";

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Destroy Session Complete called"), true, false, FColor::Orange, 20.0f);
  MyCharacterController->ClientTravel(MapName, TRAVEL_Absolute);

  //RB_SessionSubsystem->TryTravelToSession(CurrentSelectedSession);
}


void ARB_AC_Session::OnJoinGameSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
  RB_SessionSubsystem->TryTravelToSession(CurrentSelectedSession);
}

void ARB_AC_Session::UpdateFloatingImageHud()
{
  
}


// Called every frame
//void ARB_AC_Session::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

