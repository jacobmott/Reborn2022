// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RB_AC_Session.h"
#include "Components/StaticMeshComponent.h"

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

  StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
  StaticMeshComp->SetupAttachment(RootComponent);
  StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARB_AC_Session::OnOverlapBegin);

  SetReplicates(false);




  SessionActorTypeWidget = CreateDefaultSubobject<UWidgetComponent>("SessionActorTypeWidget");
  SESSION_ACTOR_TYPE value = SessionActorType.GetValue();
  if (value == SESSION_ACTOR_TYPE::Client) {
    SessionListWidget = CreateDefaultSubobject<UWidgetComponent>("SessionListWidget");
  }
  static ConstructorHelpers::FClassFinder<UUserWidget> SessionListWidgetObj(TEXT("/Game/MyStuff/Blueprints/Widgets/BP_W_FloatingText"));
  if (SessionListWidgetObj.Succeeded()) {
    SessionActorTypeWidget->SetWidgetClass(SessionListWidgetObj.Class);
    if (value == SESSION_ACTOR_TYPE::Client) {
      SessionListWidget->SetWidgetClass(SessionListWidgetObj.Class);
    }
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

  SessionActorTypeWidget->SetTwoSided(false);
  SessionActorTypeWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
  UpdateFloatingTextTypeHud();

  /* Activate client searching */
  SESSION_ACTOR_TYPE value = SessionActorType.GetValue();
  if (value == SESSION_ACTOR_TYPE::Client){
    SessionListWidget->SetTwoSided(false);
    SessionListWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    UpdateFloatingTextHud();
    GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_Session::RefreshSessionsList, 6.0f, true);
  }

  UGameInstance* TGameInstance = GetGameInstance();
  RB_SessionSubsystem = TGameInstance->GetSubsystem<URB_SessionSubsystem>();
  if (!RB_SessionSubsystem) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnOverlapBegin. Could not find SessionSubsystem"), true, false, FColor::Red, 10.0f);
    return;
  }


  RB_SessionSubsystem->OnCreateSessionCompleteEvent.AddDynamic(this, &ARB_AC_Session::OnCreateSessionComplete);
  RB_SessionSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ARB_AC_Session::OnFindSessionsComplete);
  RB_SessionSubsystem->OnStartSessionCompleteEvent.AddDynamic(this, &ARB_AC_Session::OnStartSessionComplete);
  RB_SessionSubsystem->OnJoinGameSessionCompleteEvent.AddUObject(this, &ARB_AC_Session::OnJoinGameSessionComplete);
}


void ARB_AC_Session::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnFindSessionsComplete Top"), true, false, FColor::Red, 10.0f);
  if (!Successful) {
    return;
  }
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnFindSessionsComplete Succsss"), true, false, FColor::Red, 10.0f);
  FString NoSessionYet = TEXT("NULLNULLNULL");
  CurrentSelectedSessionId = NoSessionYet;
  for (auto& SessionResult : SessionResults) {

    FString SessionId = SessionResult.Session.GetSessionIdStr();
    Sessions.Add(SessionId, SessionResult);

    CurrentSelectedSessionId = SessionId;
  }
  //if (ElapsedTime >= 6.0f && !CurrentSelectedSessionId.Equals(NoSessionYet)) {
  if (!CurrentSelectedSessionId.Equals(NoSessionYet) ) {
    ElapsedTime = 0.0f;
    CurrentSelectedSession = Sessions[CurrentSelectedSessionId];
    UpdateFloatingTextHud();
  }

  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnFindSessionsComplete: End: ") + FString::FromInt(SessionResults.Num()), true, false, FColor::Red, 10.0f);

}


void ARB_AC_Session::OnCreateSessionComplete(bool Successful)
{
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnCreateSessionComplete, Called"), true, false, FColor::Red, 10.0f);
  if (!Successful) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnCreateSessionComplete, Not successful"), true, false, FColor::Red, 10.0f);
    return;
  }
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnCreateSessionComplete, Calling StartSession"), true, false, FColor::Black, 10.0f);
  RB_SessionSubsystem->StartSession();
}

void ARB_AC_Session::RefreshSessionsList()
{
  RB_SessionSubsystem->FindSessions(10, false);
  //ElapsedTime += 2.0f;
}

void ARB_AC_Session::OnJoinGameSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
  RB_SessionSubsystem->TryTravelToSession(CurrentSelectedSession);
}

void ARB_AC_Session::OnStartSessionComplete(bool Successful)
{
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnStartSessionComplete, Called"), true, false, FColor::Red, 10.0f);
  if (!Successful) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnStartSessionComplete, Not successful"), true, false, FColor::Red, 10.0f);
    return;
  }
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnStartSessionComplete, Calling StartSession"), true, false, FColor::Black, 10.0f);
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnStartSessionComplete, right before GetWorld()->ServerTravel call"), true, false, FColor::Yellow, 30.0f);
  if (GetWorld()->ServerTravel("Reborn_2022?listen", true, false)) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("-ARB_AC_Session:OnStartSessionComplete"), true, false, FColor::Yellow, 30.0f);
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("  --GetWorld()->ServerTravel Success!!!"), true, false, FColor::Yellow, 30.0f);
  }
}

void ARB_AC_Session::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnOverlapBegin"), true, false, FColor::Red, 10.0f);
  if (!OtherActor) {
    return;
  }
 

  ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(OtherActor);
  APlayerController* MyCharacterController = Cast<APlayerController>(MyCharacter->GetController());

  APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

  //if (ForServer && (PC == MyCharacterController)) {
  if (SessionActorType.GetValue() == SESSION_ACTOR_TYPE::Host) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnOverlapBegin, Host/Server Session created"), true, false, FColor::Green, 20.0f);
    RB_SessionSubsystem->CreateSession(5, false);
  }
  //else if (ForClient && PC->IsLocalPlayerController() && (PC == MyCharacterController)) {
  if (SessionActorType.GetValue() == SESSION_ACTOR_TYPE::Client) {
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_AC_Session:OnOverlapBegin, Client"), true, false, FColor::Green, 10.0f);
    RB_SessionSubsystem->JoinGameSession(CurrentSelectedSession);
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

