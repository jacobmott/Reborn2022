// Fill out your copyright notice in the Description page of Project Settings.


#include "JunkyardAndNotes.h"

JunkyardAndNotes::JunkyardAndNotes()
{
  //SetReplicates(false);
  //bReplicates = false;
}

JunkyardAndNotes::~JunkyardAndNotes()
{
}



//class UStaticMeshComponent;
//class URB_SessionSubsystem;
//class UWidgetComponent;
//class UCapsuleComponent;


//UENUM()
//enum SESSION_ACTOR_TYPE
//{
//  Host       UMETA(DisplayName = "Host"),
//  Client     UMETA(DisplayName = "Client"),
//};

//UCLASS()
//class REBORN_2022_API ARB_AC_Session : public AActor
//{
//  GENERATED_BODY()

//public:
  // Sets default values for this actor's properties
//  ARB_AC_Session();

 // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
//    UStaticMeshComponent* StaticMeshComp;

  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
  //TEnumAsByte<SESSION_ACTOR_TYPE> SessionActorType;

  //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collisions")
  //  UCapsuleComponent* CapsuleComponent;


  //UFUNCTION()
  //  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
  //    const FHitResult& SweepResult);

  //UPROPERTY(EditAnywhere, Category = "Portal")
  //TSubclassOf<AActor> PortalActorToSpawn;

  //Floating widget
  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextHud")
  //  UWidgetComponent* TextHudWidget;

  //Floating widget
  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionActorType")
  //UWidgetComponent* SessionActorTypeWidget;

//protected:


/* Activate client searching */
//SESSION_ACTOR_TYPE value = SessionActorType.GetValue();
//if (value == SESSION_ACTOR_TYPE::Client) {
//SessionListWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Started Timesrs"), true, false, FColor::Red, 10.0f);
//RB_SessionSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ARB_AC_SessionClient::OnFindSessionsComplete);
//RB_SessionSubsystem->OnJoinGameSessionCompleteEvent.AddUObject(this, &ARB_AC_SessionClient::OnJoinGameSessionComplete);
//GetWorld()->GetTimerManager().SetTimer(SearchSessionsTimerHandle, this, &ARB_AC_SessionClient::RefreshSessionsList, 2.0f, false, 0.0f);
//GetWorld()->GetTimerManager().SetTimer(UpdateCurrentSessionTimerHandle, this, &ARB_AC_SessionClient::RefreshAndRotateCurrentSession, 4.0f, true, 4.0f);
//}



//ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(OtherActor);
//APlayerController* MyCharacterController = Cast<APlayerController>(MyCharacter->GetController());
//APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

//if (SessionActorType.GetValue() == SESSION_ACTOR_TYPE::Host) {
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Host/Server Session created"), true, false, FColor::Green, 20.0f);
//  RB_SessionSubsystem->CreateSession(5, false);
//}
//if (SessionActorType.GetValue() == SESSION_ACTOR_TYPE::Client) {
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT(" Client join game session"), true, false, FColor::Green, 10.0f);
  //RB_SessionSubsystem->JoinGameSession(CurrentSelectedSession);
//}

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
