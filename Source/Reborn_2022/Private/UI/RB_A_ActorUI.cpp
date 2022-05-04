// Copyright Vortex Games, Inc. All Rights Reserved.


#include "UI/RB_A_ActorUI.h"


#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

#include "Fonts/SlateFontInfo.h"
#include "Widgets/Text/STextBlock.h"

// Sets default values
ARB_A_ActorUI::ARB_A_ActorUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
  RootComponent = CapsuleComponent;

  StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
  StaticMeshComp->SetupAttachment(RootComponent);

  TextHudWidget = CreateDefaultSubobject<UWidgetComponent>("TextHudWidget");
  TextHudWidget->SetupAttachment(RootComponent);

  static ConstructorHelpers::FClassFinder<UUserWidget> TextHudWidgetObj(TEXT("/Game/MyStuff/Blueprints/Widgets/BP_W_FloatingTextOnly"));
  if (TextHudWidgetObj.Succeeded()) {
    UE_LOG(LogTemp, Warning, TEXT("ARB_AC_ActorUI:ARB_AC_ActorUI.Found UserWidget Floating text"));
    TextHudWidget->SetWidgetClass(TextHudWidgetObj.Class);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("TextHudWidgetObj widget could not initialize on ARB_AC_ActorUI"));
  }


}

// Called when the game starts or when spawned
void ARB_A_ActorUI::BeginPlay()
{
	Super::BeginPlay();
  TextHudWidget->SetTwoSided(true);

  UUserWidget* T = TextHudWidget->GetWidget();
  TB = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("TextBlock_0"))));

}

// Called every frame
void ARB_A_ActorUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  UpdateFloatingTextHud(DeltaTime);
}


void ARB_A_ActorUI::UpdateFloatingTextHud(float DeltaTime)
{
  float Msec = DeltaTime * 1000.0;
  float FPS = 1.0 / DeltaTime;

  float MaxFPS = GEngine->GetMaxFPS();
  //float MaxTickRate = GEngine->GetMaxTickRate();

  //GEngine->bUseFixedFrameRate = true;

  FString AreUsingFixedFrameRate = GEngine->bUseFixedFrameRate ? TEXT("True") : TEXT("False");

  FString Content = TEXT("");

  FString TheFloatFPS = FString::SanitizeFloat(FPS);
  FString TheFloatMsec = FString::SanitizeFloat(Msec);
  FString TheFloatMaxFPS = FString::SanitizeFloat(MaxFPS);
  FString IsRunningBattery = FPlatformMisc::IsRunningOnBattery() ? TEXT("True") : TEXT("False");
  FString ShouldThrottleCPUUsage = GEngine->ShouldThrottleCPUUsage() ? TEXT("True") : TEXT("False");
  
  TB->SetAutoWrapText(false);
  //TB->Wrap
  FSlateFontInfo InFontInfo = TB->Font;

  InFontInfo.Size = Size;

  TB->SetFont(InFontInfo);


  if (Size <= 0) {
    float MinX = StaticMeshComp->GetStaticMesh()->GetBoundingBox().Min.X;
    float MaxX = StaticMeshComp->GetStaticMesh()->GetBoundingBox().Max.X;
    float Size2 = FMath::Abs(MinX - MaxX);
    //Size = Size2;
  }

  
  //Content += CurrentSelectedSession.GetSessionIdStr();
  Content += TEXT("FPS: ") + TheFloatFPS + TEXT("\n");
  Content += TEXT("DeltaMsec: ") + TheFloatMsec + TEXT("\n");
  Content += TEXT("MaxFPS: ") + TheFloatMaxFPS + TEXT("\n");
  Content += TEXT("FixedFrameRate?: ") + AreUsingFixedFrameRate + TEXT("\n");
  Content += TEXT("PowerMode(Laptop)?: ") + IsRunningBattery + TEXT("\n");
  Content += TEXT("ShouldThrottleCPUUsage?: ") + ShouldThrottleCPUUsage + TEXT("\n");
  TB->SetText(FText::FromString(Content));
  //TB->SetWrapTextAt(StaticMeshComp->GetStaticMesh()->GetBounds().BoxExtent.X);


}



void ARB_A_ActorUI::ToggleHidden()
{

  IsHidden = !IsHidden;

  // Hides visible components
  SetActorHiddenInGame(IsHidden);

  // Disables collision components
  SetActorEnableCollision(!IsHidden);

  // Stops the Actor from ticking
  SetActorTickEnabled(!IsHidden);


}

