// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RB_AC_SessionHost.h"

#include "Subsystems/RB_SessionSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"

#include "Misc/Paths.h"

// Sets default values
ARB_AC_SessionHost::ARB_AC_SessionHost()
{
  StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARB_AC_SessionHost::OnOverlapBegin);
}



// Called when the game starts or when spawned
void ARB_AC_SessionHost::BeginPlay()
{
  Super::BeginPlay();

  RB_SessionSubsystem->OnCreateSessionCompleteEvent.AddDynamic(this, &ARB_AC_SessionHost::OnCreateSessionComplete);
  RB_SessionSubsystem->OnStartSessionCompleteEvent.AddDynamic(this, &ARB_AC_SessionHost::OnStartSessionComplete);
  CurrentMap = Maps[0];

  GetWorld()->GetTimerManager().SetTimer(RotateMapSelectionTimerHandle, this, &ARB_AC_SessionHost::RotateMapSelection, 4.0f, true, 0.0f);

  UpdateFloatingTextHud();

}

void ARB_AC_SessionHost::UpdateFloatingTextHud()
{

  Super::UpdateFloatingTextHud();

  UUserWidget* T = TextHudWidget->GetWidget();

  UImage* IM = Cast<UImage>(T->GetWidgetFromName(FName(TEXT("Image_70"))));
  //FString ImagePath = FPaths::ProjectContentDir() + "MyStuff/Maps/Thumbnails/" + CurrentMap;
  //UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(ImagePath)));
  IM->SetBrushFromTexture(CurrentMap.Texture);

  FString Content = TEXT("HOST:\n");
  UTextBlock* TB = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("TextBlock_0"))));
  Content += CurrentMap.MapName;
  TB->SetText(FText::FromString(Content));

}


void ARB_AC_SessionHost::RotateMapSelection()
{

  if (Maps.Num() <= 0) {
    return;
  }

  CurrentMapIndex += 1;
  if (CurrentMapIndex >= Maps.Num()) {
    CurrentMapIndex = 0;
  }
  CurrentMap = Maps[CurrentMapIndex];

  UpdateFloatingTextHud();
}


void ARB_AC_SessionHost::OnStartSessionComplete(bool Successful)
{

  if (!Successful) {
    return;
  }
  if (GetWorld()->ServerTravel(CurrentMap.MapName+"?listen", true, false)) {
  }
}


void ARB_AC_SessionHost::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  if (!OtherActor) {
    return;
  }

  RB_SessionSubsystem->CreateSession(5, false, CurrentMap.MapName);
}