// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RB_AC_SessionHost.h"

#include "Subsystems/RB_SessionSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"

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

}

void ARB_AC_SessionHost::UpdateFloatingTextHud()
{

  Super::UpdateFloatingTextHud();

  FString Content = TEXT("HOST:\n");
  UUserWidget* T = TextHudWidget->GetWidget();
  UTextBlock* TB = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("TextBlock_0"))));
  TB->SetText(FText::FromString(Content));


  UImage* IM = Cast<UImage>(T->GetWidgetFromName(FName(TEXT("Image_70"))));
  //IM->SourceFileTagName

}


void ARB_AC_SessionHost::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Host/Server Overlap"), true, false, FColor::Green, 20.0f);
  if (!OtherActor) {
    return;
  }

  RB_SessionSubsystem->CreateSession(5, false);
}