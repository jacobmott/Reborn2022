// Copyright Vortex Games, Inc. All Rights Reserved.


#include "UI/RB_A_MainMenu.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

#include "Fonts/SlateFontInfo.h"
#include "Widgets/Text/STextBlock.h"

#include "Audio/RB_UO_AudioPlayer.h"


// Sets default values
ARB_A_MainMenu::ARB_A_MainMenu()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;

  //CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
  //RootComponent = CapsuleComponent;

  //StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
  //StaticMeshComp->SetupAttachment(RootComponent);

  MainMenuHudWidget = CreateDefaultSubobject<UWidgetComponent>("MainMenuHudWidget");
  MainMenuHudWidget->SetupAttachment(RootComponent);


  AudioPlayer = CreateDefaultSubobject<URB_UO_AudioPlayer>("AudioPlayer");
  //AudioPlayer = NewObject<URB_UO_AudioPlayer>(MainSoundTrackCue);


  static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuHudWidgetObj(TEXT("/Game/MyStuff/Blueprints/Widgets/BP_W_FloatingGameMenu"));
  if (MainMenuHudWidgetObj.Succeeded()) {
    UE_LOG(LogTemp, Warning, TEXT("ARB_A_MainMenu:ARB_A_MainMenu.Found UserWidget Floating GameMenu"));
    MainMenuHudWidget->SetWidgetClass(MainMenuHudWidgetObj.Class);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("MainMenuHudWidgetObj widget could not initialize on ARB_A_MainMenu"));
  }


}


// Called when the game starts or when spawned
void ARB_A_MainMenu::BeginPlay()
{
  Super::BeginPlay();

  //AudioPlayer = NewObject<URB_UO_AudioPlayer>();
  AudioPlayer->BeginPlay(MainSoundTrackCue);
  //MainMenuHudWidget->SetTwoSided(false);
  //GEngine->FixedFrameRate = 240;
  //GEngine->bUseFixedFrameRate = false;

  UUserWidget* T = MainMenuHudWidget->GetWidget();
  BTNMusic = Cast<UButton>(T->GetWidgetFromName(FName(TEXT("btn_music"))));
  BTNFps120 = Cast<UButton>(T->GetWidgetFromName(FName(TEXT("btn_fps_120"))));
  TBMusic = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("tb_music"))));
  TBFps120 = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("tb_fps_120"))));

  FString Content = TEXT("Hover over a button to get help text\n");
  //Content += TEXT("Map: ");
  TB->SetText(FText::FromString(Content));

  UpdateFloatingTextHud(10);


  //AudioPlayer = NewObject<URB_UO_AudioPlayer>();

}


void ARB_A_MainMenu::UpdateFloatingTextHud(float DeltaTime)
{

  FString Content = TEXT("FPS 120");


  BTNFps120->OnHovered.AddDynamic(this, &ARB_A_MainMenu::OnHoveredBTNFps120);
  BTNFps120->OnClicked.AddDynamic(this, &ARB_A_MainMenu::OnClickedBTNFps120);


  BTNMusic->OnHovered.AddDynamic(this, &ARB_A_MainMenu::OnHoveredBTNMusic);
  BTNMusic->OnClicked.AddDynamic(this, &ARB_A_MainMenu::OnClickedBTNMusic);

  //TBFps120->SetText(FText::FromString(Content));

}


void ARB_A_MainMenu::OnHoveredBTNMusic()
{

  //GEngine->FixedFrameRate = 240;
  //GEngine->bUseFixedFrameRate = true;
  //GEngine->SetMaxFPS(240);
  //AudioPlayer->PauseMusic();

  FString Content = TEXT("Right click to pause or unpause music\n");
  //Content += TEXT("Map: ");
  TB->SetText(FText::FromString(Content));

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_A_MainMenu OnHoveredBTNMusic Triggered"), true, false, FColor::Green, 20.0f);
}


void ARB_A_MainMenu::OnClickedBTNMusic()
{
  //AudioPlayer->StopMusic();
  AudioPlayer->PauseMusic();
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_A_MainMenu OnClickedBTNMusic Triggered"), true, false, FColor::Green, 20.0f);
}



void ARB_A_MainMenu::OnHoveredBTNFps120()
{
  FString Content = TEXT("Right click to set fps to 120\n");
  //Content += TEXT("Map: ");
  TB->SetText(FText::FromString(Content));
  //GEngine->FixedFrameRate = 240;
  //GEngine->bUseFixedFrameRate = true;
  //GEngine->SetMaxFPS(240);

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_A_MainMenu OnHoveredBTNFps120 Triggered"), true, false, FColor::Green, 20.0f);
}


void ARB_A_MainMenu::OnClickedBTNFps120()
{

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_A_MainMenu OnClickedBTNFps120 Triggered"), true, false, FColor::Green, 20.0f);
}