// Copyright Vortex Games, Inc. All Rights Reserved.


#include "Network/RB_AC_SessionEnd.h"

#include "Subsystems/RB_SessionSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"

#include "Components/Image.h"
#include "Misc/Paths.h"

#include "Characters/RB_CC_MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARB_AC_SessionEnd::ARB_AC_SessionEnd()
{
  StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARB_AC_SessionEnd::OnOverlapBegin);
  bReplicates = false;
  bNetLoadOnClient = true;
  
}


// Called when the game starts or when spawned
void ARB_AC_SessionEnd::BeginPlay()
{
  Super::BeginPlay();

  
  RB_SessionSubsystem->OnEndSessionCompleteEvent.AddDynamic(this, &ARB_AC_SessionEnd::OnEndSessionComplete);
  RB_SessionSubsystem->OnDestroySessionCompleteEvent.AddDynamic(this, &ARB_AC_SessionEnd::OnDestroySessionComplete);

  CurrentSessionName = RB_SessionSubsystem->GetCurrentSession();
  CurrentMapName = RB_SessionSubsystem->GetCurrentMap();
  CurrentOnlineSessionSearchResult = RB_SessionSubsystem->GetCurrentOnlineSessionSearchResult();

  //GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_SessionClient::RefreshSessionsList, 2.0f, false, 0.0f);
  //GetWorld()->GetTimerManager().SetTimer(UpdateCurrentSessionTimerHandle, this, &ARB_AC_SessionClient::RefreshAndRotateCurrentSession, 4.0f, true, 4.0f);


  UpdateFloatingTextHud();
}



void ARB_AC_SessionEnd::UpdateFloatingTextHud()
{

  Super::UpdateFloatingTextHud();

  FString Content = TEXT("ENDSession:\n");
  UUserWidget* T = TextHudWidget->GetWidget();

  //FString MapName = CurrentMap.MapName;
  //FString SessionName = CurrentSessionName.ToString();
  if (CurrentSessionName.ToString().Len() <= 0) {
    return;
  }
  UImage* IM = Cast<UImage>(T->GetWidgetFromName(FName(TEXT("Image_70"))));
  FString ImagePath = "/Game/MyStuff/Maps/Thumbnails/" + CurrentMapName;
  UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ImagePath));
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("CLIENT imagepath: ") + ImagePath, true, false, FColor::Blue, 20.0f);
  CurrentMap.Texture = Texture;
  IM->SetBrushFromTexture(CurrentMap.Texture);


  UTextBlock* TB = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("TextBlock_0"))));
  //Content += CurrentSelectedSession.GetSessionIdStr();
  FString Id = CurrentOnlineSessionSearchResult.GetSessionIdStr();
  FString SmallSessionId = Id.RightChop(Id.Len() - 3);
  Content += TEXT("Map: ") + CurrentMapName + TEXT("\n");
  Content += TEXT("SessId: ") + SmallSessionId + TEXT("\n");
  Content += TEXT("HostName: ") + CurrentOnlineSessionSearchResult.Session.OwningUserName + TEXT("\n");
  Content += TEXT("PubConns: ") + FString::FromInt(CurrentOnlineSessionSearchResult.Session.NumOpenPublicConnections) + TEXT("\n");
  Content += TEXT("Ping(Ms): ") + FString::FromInt(CurrentOnlineSessionSearchResult.PingInMs) + TEXT("\n");
  Content += TEXT("SessionName: ") + CurrentSessionName.ToString() + TEXT("\n");
  TB->SetText(FText::FromString(Content));

}



void ARB_AC_SessionEnd::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("EndSession Overlap"), true, false, FColor::Blue, 20.0f);

  if (!OtherActor) {
    return;
  }

  if (CurrentMapName.Len() <= 0) {
    return;
  }

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Calling DestroySession"), true, false, FColor::Blue, 20.0f);
  CurrentActorForEndSession = OtherActor;

  ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(CurrentActorForEndSession);
  APlayerController* MyCharacterController = Cast<APlayerController>(MyCharacter->GetController());
  APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  FString MapName = "Reborn_2022_MainLobby";
  
  //CurrentActorForEndSession->GetGameInstance()->Online

  //UGameInstance* TGameInstance = GetGameInstance();
  //RB_SessionSubsystem = TGameInstance->GetSubsystem<URB_SessionSubsystem>();
  //if (!RB_SessionSubsystem) {
  //  return;
  //}

  //Was it me (me as in im asking as the server or the client if its myself)
  // if so only then call this, that way the server only calls this for itself
  // and also skips calling it for the client
  //Theres other ways to do this(For example we could:
  // -When the player presses escape
  // -Open a in game menu
  // -do cursor screen collision
  // -player clicks something that triggers a call locally to destroy session
  // -then an animation plays to open a portal and leave the game
  if (MyCharacterController->IsLocalPlayerController()){
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Calling DestroySession"), true, false, FColor::Blue, 20.0f);
    RB_SessionSubsystem->DestroySession(CurrentSessionName);
  }

  //Only if we are the server do we want to call 
  //if ( PC->IsLocalPlayerController() && (MyCharacterController == PC) ){
  //  RB_SessionSubsystem->DestroySession(CurrentSessionName);
  //}
  

}