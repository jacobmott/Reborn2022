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
  //RootComponent = StaticMeshComp;
  StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARB_AC_Session::OnOverlapBegin);

  //SetReplicates(false);
  bReplicates = false;


  SessionActorTypeWidget = CreateDefaultSubobject<UWidgetComponent>("SessionActorTypeWidget");
  SessionActorTypeWidget->SetupAttachment(RootComponent);
  SESSION_ACTOR_TYPE value = SessionActorType.GetValue();
  //if (value == SESSION_ACTOR_TYPE::Client) {
    SessionListWidget = CreateDefaultSubobject<UWidgetComponent>("SessionListWidget");
    SessionListWidget->SetupAttachment(RootComponent);
  //}
  static ConstructorHelpers::FClassFinder<UUserWidget> SessionListWidgetObj(TEXT("/Game/MyStuff/Blueprints/Widgets/BP_W_FloatingText"));
  if (SessionListWidgetObj.Succeeded()) {
    UE_LOG(LogTemp, Warning, TEXT("ARB_AC_Session:ARB_AC_Session.Found UserWidget Floating text"));
    SessionActorTypeWidget->SetWidgetClass(SessionListWidgetObj.Class);
    //if (value == SESSION_ACTOR_TYPE::Client) {
      SessionListWidget->SetWidgetClass(SessionListWidgetObj.Class);
    //}
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("SessionListWidgetObj widget could not initialize on ARB_AC_Session"));
  }

}


void ARB_AC_Session::UpdateFloatingTextTypeHud()
{
  // MyHealthWidget
   //UUserWidget* NewWidget = NewObject<UUserWidget>(Outer, UserWidgetClass, InstanceName, RF_Transactional);
  UUserWidget* T = SessionActorTypeWidget->GetWidget();
  UTextBlock* TB = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("TextBlock_0"))));
  //TB->SetAutoWrapText(true);
  FString Value = UEnum::GetValueAsName(SessionActorType.GetValue()).ToString();
  TB->SetText(FText::FromString(Value));
}


void ARB_AC_Session::UpdateFloatingTextHud()
{
  // MyHealthWidget
   //UUserWidget* NewWidget = NewObject<UUserWidget>(Outer, UserWidgetClass, InstanceName, RF_Transactional);
  UUserWidget* T = SessionListWidget->GetWidget();
  UTextBlock* TB = Cast<UTextBlock>(T->GetWidgetFromName(FName(TEXT("TextBlock_0"))));
  TB->SetText( FText::FromString(CurrentSelectedSession.GetSessionIdStr()) );
}

// Called when the game starts or when spawned
void ARB_AC_Session::BeginPlay()
{
	Super::BeginPlay();

  CurrentSelectedSessionId = NoSessionYet;

  SessionActorTypeWidget->SetTwoSided(false);
  //SessionActorTypeWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
  UpdateFloatingTextTypeHud();



  UGameInstance* TGameInstance = GetGameInstance();
  RB_SessionSubsystem = TGameInstance->GetSubsystem<URB_SessionSubsystem>();
  if (!RB_SessionSubsystem) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnOverlapBegin. Could not find SessionSubsystem"), true, false, FColor::Red, 10.0f);
    return;
  }




  /* Activate client searching */
  SESSION_ACTOR_TYPE value = SessionActorType.GetValue();
  if (value == SESSION_ACTOR_TYPE::Client) {
    SessionListWidget->SetTwoSided(false);
    //SessionListWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    UpdateFloatingTextHud();
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Started Timesrs"), true, false, FColor::Red, 10.0f);
    RB_SessionSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ARB_AC_Session::OnFindSessionsComplete);
    RB_SessionSubsystem->OnJoinGameSessionCompleteEvent.AddUObject(this, &ARB_AC_Session::OnJoinGameSessionComplete);
    GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_Session::RefreshSessionsList, 2.0f, false, 0.0f);
    GetWorld()->GetTimerManager().SetTimer(UpdateCurrentSessionTimerHandle, this, &ARB_AC_Session::RefreshAndRotateCurrentSession, 4.0f, true, 4.0f);
  }
  if (value == SESSION_ACTOR_TYPE::Host) {
    RB_SessionSubsystem->OnCreateSessionCompleteEvent.AddDynamic(this, &ARB_AC_Session::OnCreateSessionComplete);
    RB_SessionSubsystem->OnStartSessionCompleteEvent.AddDynamic(this, &ARB_AC_Session::OnStartSessionComplete);
    SessionListWidget->SetHiddenInGame(true);
  }


}


void ARB_AC_Session::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnFindSessionsComplete Top"), true, false, FColor::Red, 10.0f);
  if (!Successful) {
    return;
  }
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnFindSessionsComplete Succsss"), true, false, FColor::Red, 10.0f);

  SessionsArray = SessionResults;
  //RefreshCurrentSession();
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("NUM IN SessoinResults ARRAY:? " + FString::FromInt(SessionResults.Num())), true, false, FColor::Blue, 10.0f);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("NUM IN ARRAY:? "+ FString::FromInt(SessionsArray.Num())), true, false, FColor::Blue, 10.0f);
  //for (auto& SessionResult : SessionResults) {
  //  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Somethings in here? " ), true, false, FColor::Blue, 10.0f);
  //}

  GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_Session::RefreshSessionsList, 2.0f, false, 2.0f);
  //GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_Session::RefreshSessionsList, 8.0f, false, 8.0f);

  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnFindSessionsComplete: End: ") + FString::FromInt(SessionResults.Num()), true, false, FColor::Red, 10.0f);

}


void ARB_AC_Session::OnCreateSessionComplete(bool Successful)
{
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnCreateSessionComplete, Called"), true, false, FColor::Red, 10.0f);
  if (!Successful) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnCreateSessionComplete, Not successful"), true, false, FColor::Red, 10.0f);
    return;
  }
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnCreateSessionComplete, Calling StartSession"), true, false, FColor::Black, 10.0f);
  RB_SessionSubsystem->StartSession();
}

void ARB_AC_Session::RefreshSessionsList()
{

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("RefreshSessionsList"), true, false, FColor::Black, 10.0f);
  RB_SessionSubsystem->FindSessions(10, false);
  //ElapsedTime += 2.0f;

}

void ARB_AC_Session::RefreshAndRotateCurrentSession()
{


  if (SessionsArray.Num() <= 0) {
    return;
  }

  //if (ElapsedTime >= 6.0f && !CurrentSelectedSessionId.Equals(NoSessionYet)) {
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

void ARB_AC_Session::OnStartSessionComplete(bool Successful)
{
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnStartSessionComplete, Called"), true, false, FColor::Red, 10.0f);
  if (!Successful) {
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnStartSessionComplete, Not successful"), true, false, FColor::Red, 10.0f);
    return;
  }
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnStartSessionComplete, Calling StartSession"), true, false, FColor::Black, 10.0f);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnStartSessionComplete, right before GetWorld()->ServerTravel call"), true, false, FColor::Yellow, 30.0f);
  if (GetWorld()->ServerTravel("Reborn_2022?listen", true, false)) {
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("-ARB_AC_Session:OnStartSessionComplete"), true, false, FColor::Yellow, 30.0f);
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("  --GetWorld()->ServerTravel Success!!!"), true, false, FColor::Yellow, 30.0f);
  }
}

void ARB_AC_Session::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnOverlapBegin HEREHERHEHREHREHEHREHEH"), true, false, FColor::Red, 10.0f);
  if (!OtherActor) {
    return;
  }
 

  ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(OtherActor);
  APlayerController* MyCharacterController = Cast<APlayerController>(MyCharacter->GetController());

  APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

  //if (ForServer && (PC == MyCharacterController)) {
  if (SessionActorType.GetValue() == SESSION_ACTOR_TYPE::Host) {
    //GetWorldTimerManager().ClearTimer(SearchSessionsTimerHandle);
    //GetWorldTimerManager().ClearTimer(UpdateCurrentSessionTimerHandle);
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Host/Server Session created"), true, false, FColor::Green, 20.0f);
    RB_SessionSubsystem->CreateSession(5, false);
  }
  //else if (ForClient && PC->IsLocalPlayerController() && (PC == MyCharacterController)) {
  if (SessionActorType.GetValue() == SESSION_ACTOR_TYPE::Client) {

    //if (Initial){
    //  FTimerManager& Manager = GetWorld()->GetTimerManager();
    //  Manager.SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_Session::RefreshSessionsList, 8.0f, false, 8.0f);
    //  Manager.SetTimer(UpdateCurrentSessionTimerHandle, this, &ARB_AC_Session::RefreshCurrentSession, 6.0f, false, 6.0f);
    //  Initial = false;
    //}
    //else{
      UKismetSystemLibrary::PrintString(GetWorld(), TEXT(" Client join game session"), true, false, FColor::Green, 10.0f);
      RB_SessionSubsystem->JoinGameSession(CurrentSelectedSession);
    //}
  }

  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnOverlapBegin, Got SessionSubsystem"), true, false, FColor::Red, 10.0f);
  //CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted)
  //RB_SessionSubsystem->OnCreateSessionCompleteEvent.AddDynamic(this, &ARB_AC_Session::OnCreateSessionComplete);


  ///UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_SessionSubsystem:OnStartSessionCompleted, right before GetWorld()->ServerTravel call"), true, false, FColor::Yellow, 30.0f);
  ///if (GetWorld()->ServerTravel("Reborn_2022?listen", true, false)) {
  ///  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("-URB_SessionSubsystem:OnStartSessionCompleted"), true, false, FColor::Yellow, 30.0f);
  ///  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("  --GetWorld()->ServerTravel Success!!!"), true, false, FColor::Yellow, 30.0f);
  ///}


  //RB_SessionSubsystem->FindSessions(20, true);

  //if (FocusedActor) {
  //  IInteractInterface* Interface = Cast<IInteractInterface>(OtherActor);
  //  if (Interface) {
  //    Interface->Execute_OnInteract(OtherActor, this);
  //  }
  //}
}

// Called every frame
//void ARB_AC_Session::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

