// Copyright Vortex Games, Inc. All Rights Reserved.


#include "Audio/RB_UO_AudioPlayer.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

URB_UO_AudioPlayer::URB_UO_AudioPlayer() {

  //http://bendemott.blogspot.com/2016/10/unreal-4-playing-sound-from-c-with.html
  // Load our Sound Cue for the propeller sound we created in the editor... note your path may be different depending
      // on where you store the asset on disk.
  //static ConstructorHelpers::FObjectFinder<USoundCue> propellerCue(TEXT("'/Game/airplane-engine.airplane-engine'"));

  //AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
  //AudioComponent->SetupAttachment(RootComponent);
  //static ConstructorHelpers::FClassFinder<UUserWidget> TextHudWidgetObj(TEXT("/Game/MyStuff/Blueprints/Widgets/BP_W_FloatingText"));
  //if (TextHudWidgetObj.Succeeded()) {
  //  UE_LOG(LogTemp, Warning, TEXT("ARB_AC_Session:ARB_AC_Session.Found UserWidget Floating text"));
  //  TextHudWidget->SetWidgetClass(TextHudWidgetObj.Class);
  //}
  //else
  //{
  //  UE_LOG(LogTemp, Warning, TEXT("TextHudWidgetObj widget could not initialize on ARB_AC_Session"));
  //}
  //MainSoundTrackCue = CreateDefaultSubobject<USoundCue>("MainSoundTrackCue");
  AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
  //AudioComponent = NewObject<UAudioComponent>();

  AudioComponent->bAutoActivate = false;
}

void URB_UO_AudioPlayer::BeginPlay(USoundCue* MainSoundTrackCueParam)
{
  MainSoundTrackCue = MainSoundTrackCueParam;
  AudioComponent->SetSound(MainSoundTrackCueParam);
  UE_LOG(LogTemp, Warning, TEXT("THIS is my test function called in audio player"));

  // Note because the Cue Asset is set to loop the sound,
  // once we start playing the sound, it will play 
  // continuously...

  // You can fade the sound in... 
  float startTime = 2.f;
  float volume = 1.0f;
  float fadeTime = 1.0f;
  AudioComponent->FadeIn(fadeTime, volume, startTime);

  // Or you can start playing the sound immediately.
  AudioComponent->Play();

  AudioComponent->OnAudioFinished.AddDynamic(this, &URB_UO_AudioPlayer::OnAudioFinishedOrStoped);


  // Modulate the pitch of the sound cue by passing in the RPM of the aircraft propeller
  // Notice we reference the parameter we set in the Cue Object here: 'pitch'.
  //float propRpm = propeller->lastEngineRpm * propeller->reductionRatio;
  //propellerAudioComponent->SetFloatParameter(FName("pitch"), propRpm);


}

void URB_UO_AudioPlayer::PauseMusic()
{
  IsPaused = !IsPaused;
  AudioComponent->SetPaused(IsPaused);
}

void URB_UO_AudioPlayer::StopMusic()
{
  DidClickStop = true;
  AudioComponent->Stop();
}


void URB_UO_AudioPlayer::OnAudioFinishedOrStoped()
{
  if (DidClickStop){ 
    DidClickStop = false;
    return; 
  }
  AudioComponent->Play();
  UE_LOG(LogTemp, Warning, TEXT("OnAudioFinishedOrStoped callback/bind was called"));
}
