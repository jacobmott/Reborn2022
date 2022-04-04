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
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnOverlapBegin. Could not find SessionSubsystem"), true, false, FColor::Red, 10.0f);
    return;
  }

  CurrentSelectedSessionId = NoSessionYet;
  TextHudWidget->SetTwoSided(false);
  UpdateFloatingTextHud();

}


void ARB_AC_Session::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{

  if (!Successful) {
    return;
  }

  SessionsArray = SessionResults;
  GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_Session::RefreshSessionsList, 2.0f, false, 2.0f);

}


void ARB_AC_Session::OnCreateSessionComplete(bool Successful)
{

  if (!Successful) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnCreateSessionComplete, Not successful"), true, false, FColor::Red, 10.0f);
    return;
  }

  RB_SessionSubsystem->StartSession();
}

void ARB_AC_Session::RefreshSessionsList()
{

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("RefreshSessionsList"), true, false, FColor::Black, 10.0f);
  RB_SessionSubsystem->FindSessions(10, false);

}

void ARB_AC_Session::RefreshAndRotateCurrentSession()
{


  if (SessionsArray.Num() <= 0) {
    return;
  }

  if (CurrentSelectedSessionId.Equals(NoSessionYet)) {
    CurrentSelectedSession = SessionsArray[0];
    CurrentSelectedSessionId = CurrentSelectedSession.Session.GetSessionIdStr();
  }
  else if (!CurrentSelectedSessionId.Equals(NoSessionYet)) { 
    CurrentSessionIndex += 1;
    if (CurrentSessionIndex >= SessionsArray.Num()) {
      CurrentSessionIndex = 0;
    }
    CurrentSelectedSession = SessionsArray[CurrentSessionIndex];
    CurrentSelectedSessionId = CurrentSelectedSession.Session.GetSessionIdStr();
  }

  UpdateFloatingTextHud();
}

void ARB_AC_Session::OnJoinGameSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
  RB_SessionSubsystem->TryTravelToSession(CurrentSelectedSession);
}

void ARB_AC_Session::UpdateFloatingImageHud()
{
  
}

void ARB_AC_Session::OnStartSessionComplete(bool Successful)
{

  if (!Successful) {
    return;
  }
  if (GetWorld()->ServerTravel("Reborn_2022?listen", true, false)) {
  }
}


// Called every frame
//void ARB_AC_Session::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

