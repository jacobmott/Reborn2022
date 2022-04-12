// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RB_AC_SessionClient.h"

#include "Subsystems/RB_SessionSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"

#include "Components/Image.h"
#include "Misc/Paths.h"

// Sets default values
ARB_AC_SessionClient::ARB_AC_SessionClient()
{
  StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARB_AC_SessionClient::OnOverlapBegin);
}



// Called when the game starts or when spawned
void ARB_AC_SessionClient::BeginPlay()
{
  Super::BeginPlay();

  RB_SessionSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ARB_AC_SessionClient::OnFindSessionsComplete);
  RB_SessionSubsystem->OnJoinGameSessionCompleteEvent.AddUObject(this, &ARB_AC_SessionClient::OnJoinGameSessionComplete);
  GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_SessionClient::RefreshSessionsList, 2.0f, false, 0.0f);
  GetWorld()->GetTimerManager().SetTimer(UpdateCurrentSessionTimerHandle, this, &ARB_AC_SessionClient::RefreshAndRotateCurrentSession, 4.0f, true, 4.0f);

  UpdateFloatingTextHud();
}



void ARB_AC_SessionClient::UpdateFloatingTextHud()
{

  Super::UpdateFloatingTextHud();

  FString Content = TEXT("CLIENT:\n");
  UUserWidget* T = TextHudWidget->GetWidget();

  FString MapName = CurrentMap.MapName;
  if(MapName.Len() <= 0){
    MapName = TEXT("NoSession");
  } 
  UImage* IM = Cast<UImage>(T->GetWidgetFromName(FName(TEXT("Image_70"))));
  FString ImagePath = "/Game/MyStuff/Maps/Thumbnails/" + MapName;
  UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ImagePath));
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("CLIENT imagepath: ") + ImagePath, true, false, FColor::Blue, 20.0f);
  CurrentMap.Texture = Texture;
  IM->SetBrushFromTexture(CurrentMap.Texture);


  UTextBlock* TB = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("TextBlock_0"))));
  //Content += CurrentSelectedSession.GetSessionIdStr();
  FString Id = CurrentSelectedSession.GetSessionIdStr();
  FString SmallSessionId = Id.RightChop(Id.Len()-3);
  Content += TEXT("Map: ") + CurrentMap.MapName + TEXT("\n");
  Content += TEXT("SessId: ") + SmallSessionId + TEXT("\n");
  Content += TEXT("HostName: ") + CurrentSelectedSession.Session.OwningUserName + TEXT("\n");
  Content += TEXT("PubConns: ") + FString::FromInt(CurrentSelectedSession.Session.NumOpenPublicConnections) + TEXT("\n");
  Content += TEXT("Ping(Ms): ") + FString::FromInt(CurrentSelectedSession.PingInMs) + TEXT("\n");
  Content += TEXT("SessionName: ") + CurrentMap.SessionName.ToString() + TEXT("\n");
  TB->SetText(FText::FromString(Content));

}


void ARB_AC_SessionClient::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Client Overlap"), true, false, FColor::Blue, 20.0f);

  if (!OtherActor) {
    return;
  }

  if (CurrentMap.MapName.Len() <= 0) {
    return;
  }
  RB_SessionSubsystem->JoinGameSession(CurrentMap.MapName, CurrentMap.SessionName, CurrentSelectedSession);
}