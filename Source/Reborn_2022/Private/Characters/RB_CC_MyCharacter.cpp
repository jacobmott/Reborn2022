// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RB_CC_MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Interact/InteractInterface.h"
#include "MatineeCameraShake.h"
#include "Health/RB_ACC_HealthComponent.h"
#include "Widgets/RB_UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"

#include "PhysXPublic.h"
#include "PhysxUserData.h"
#include "PhysXPublic.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "PhysicsEngine/BodyInstance.h"


#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



static const int32 DEBUG_ALL = 1;
static const int32 DEBUG_OTHER = 2;
static const int32 DEBUG_FIREFORWARD = 3;
static int32 DebugMyCharacter = 0;
FAutoConsoleVariableRef CVARDebugMyCharacter(TEXT("Reborn.DebugMyCharacter"), DebugMyCharacter, TEXT("Draw debug info for MyCharacter"), ECVF_Cheat);

DECLARE_DELEGATE_OneParam(FShakeDelegate, float);

// Sets default values
ARB_CC_MyCharacter::ARB_CC_MyCharacter()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  //PrimaryActorTick.bCanEverTick = false;

  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
  //SpringArmComp->bUsePawnControlRotation = true;
  SpringArmComp->SetupAttachment(RootComponent);

  CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
  CameraComp->SetupAttachment(SpringArmComp);

  StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
  StaticMeshComp->SetupAttachment(RootComponent);
  StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARB_CC_MyCharacter::OnOverlapBegin);

  HealthComponent = CreateDefaultSubobject<URB_ACC_HealthComponent>(TEXT("HealthComp"));

  BaseTurnRate = 45.0f;
  BaseLookUpAtRate = 45.0f;
  TraceDistance = 2000.0f;

  ImpulseForce = 500.0f;


  ApplyRadialForce = true;
  ImpactRadius = 200.0f;
  RadialImpactForce = 2000.0f;
  //NOTE: WOW! this was causing the original issue i was seeing
  UseActorsCenterOfMassInCollisionCalculation = true;

  StartScale = FVector(1, 1, 1);
  TargetScale = FVector(1.3f, 1.3f, 0.8f);
  SquashTimeline;

  MyHealthWidget = CreateDefaultSubobject<UWidgetComponent>("MyHealthWidget");
  MyHealthWidget2 = CreateDefaultSubobject<UWidgetComponent>("MyHealthWidget2");
  MyHealthWidget3 = CreateDefaultSubobject<UWidgetComponent>("MyHealthWidget3");
  MyHealthWidget4 = CreateDefaultSubobject<UWidgetComponent>("MyHealthWidget4");
  static ConstructorHelpers::FClassFinder<UUserWidget> MyHealthWidgetObj(TEXT("/Game/MyStuff/Blueprints/Widgets/BP_W_FloatingHealth"));
  if (MyHealthWidgetObj.Succeeded()) {
    MyHealthWidget->SetWidgetClass(MyHealthWidgetObj.Class);
    MyHealthWidget2->SetWidgetClass(MyHealthWidgetObj.Class);
    MyHealthWidget3->SetWidgetClass(MyHealthWidgetObj.Class);
    MyHealthWidget4->SetWidgetClass(MyHealthWidgetObj.Class);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("MyHealthWidgetObj widget could not initialize on InteractibleActor"));
  }



}

void ARB_CC_MyCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (FocusedActor) {
    IInteractInterface* Interface = Cast<IInteractInterface>(OtherActor);
    if (Interface) {
      Interface->Execute_OnInteract(OtherActor, this);
    }
  }
}


void ARB_CC_MyCharacter::UpdateHud()
{

  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_CC_MyCharacter:UpdateHud HEREHEHEHRERH!1: ")+GetName(), true, false, FColor::Orange, 30.0f);
  if (Hud) {

    float value = HealthComponent->GetHealth() / HealthComponent->GetDefaultHealth();
    FString TheFloatStr = FString::SanitizeFloat(value);
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ARB_CC_MyCharacter:UpdateHud value")+ TheFloatStr, true, false, FColor::Orange, 3.0f);
    Hud->ProgressHealthBar->SetPercent(value);
  }

  

}

void ARB_CC_MyCharacter::UpdateFloatingHealthHud()
{
  // MyHealthWidget
   //UUserWidget* NewWidget = NewObject<UUserWidget>(Outer, UserWidgetClass, InstanceName, RF_Transactional);
  float value = HealthComponent->GetHealth() / HealthComponent->GetDefaultHealth();
  UUserWidget* T = MyHealthWidget->GetWidget();
  UProgressBar* PB = Cast<UProgressBar>(T->GetWidgetFromName( FName(TEXT("ProgressBar_0")) ) ) ;
  PB->SetPercent(value);

  UUserWidget* T2 = MyHealthWidget2->GetWidget();
  UProgressBar* PB2 = Cast<UProgressBar>(T2->GetWidgetFromName(FName(TEXT("ProgressBar_0"))));
  PB2->SetPercent(value);

  UUserWidget* T3 = MyHealthWidget3->GetWidget();
  UProgressBar* PB3 = Cast<UProgressBar>(T3->GetWidgetFromName(FName(TEXT("ProgressBar_0"))));
  PB3->SetPercent(value);

  UUserWidget* T4 = MyHealthWidget4->GetWidget();
  UProgressBar* PB4 = Cast<UProgressBar>(T4->GetWidgetFromName(FName(TEXT("ProgressBar_0"))));
  PB4->SetPercent(value);
  UParticleSystemComponent* PC =  UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RadialExplosionEffect, GetActorLocation(), FRotator::ZeroRotator, (FVector)(1.0f));
}

// Called when the game starts or when spawned
void ARB_CC_MyCharacter::BeginPlay()
{
 	Super::BeginPlay();
  //If this doesnt work from including it in the contstructor then you should add it here and enable/uncomment begin play
  //StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARB_CC_MyCharacter::OnOverlapBegin);
  Hud = CreateWidget<URB_UserWidget>(GetGameInstance(), HudClass, FName(TEXT("Hud")));
  if (Hud)
  {
    //APlayerController* const PlayerController = Cast<APlayerController>(GetController());
    //Hud->SetOwningPlayer(PlayerController);
    //Hud->SetOwningPlayer()
    Hud->AddToViewport();
    float value = HealthComponent->GetHealth() / HealthComponent->GetDefaultHealth();
    Hud->ProgressHealthBar->SetPercent(value);
 
    //GetWorldTimerManager().SetTimer(TimerHandle_Health, this, &ARB_CC_MyCharacter::SetPercent, 2.0f, true, 2.0f);
  }
  MyHealthWidget->SetTwoSided(true);
  MyHealthWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

  MyHealthWidget2->SetTwoSided(true);
  MyHealthWidget2->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

  MyHealthWidget3->SetTwoSided(true);
  MyHealthWidget3->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

  MyHealthWidget4->SetTwoSided(true);
  MyHealthWidget4->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

//void ARB_CC_MyCharacter::SetPercent()
//{
//  Current += 10.0f;
//  float Value = Current / MaxHealth;
//  Hud->ProgressHealthBar->SetPercent(Value);
//  if (Current >= 100.0f) {
//    GetWorldTimerManager().ClearTimer(TimerHandle_Health);
//  }
//}

void ARB_CC_MyCharacter::MoveForward(float Value)
{
  if (Controller && (Value != 0.0f)) {
    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    /** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
    //float Pitch;
    /** Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South. */
    //float Yaw;
    /** Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. */
    //float Roll;
    //FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll);
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    // get forward vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);
  }
}

void ARB_CC_MyCharacter::MoveRight(float Value)
{
  if (Controller && (Value != 0.0f)) {
    // find out which way is right
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    // get right vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // add movement in that direction 
    AddMovementInput(Direction, Value);
  }
}

void ARB_CC_MyCharacter::TurnAtRate(float Value)
{
  //GetWorld()->GetDeltaSeconds() this makes this frame rate independent
  AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARB_CC_MyCharacter::LookUpAtRate(float Value)
{
  AddControllerPitchInput(Value * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
}

void ARB_CC_MyCharacter::InteractPressed()
{


}


void ARB_CC_MyCharacter::TraceForward_Implementation()
{

  struct ForwardTraceHitInformation TraceInfo = GetForwardTraceHitInformation(false);
  if (TraceInfo.Error) {
    return;
  }
  //DrawDebugPoint(GetWorld(), TraceInfo.End, 5.0f, FColor::Red, false, true, -1.0f, 1);
  bool HadHit = TraceInfo.HadHit;
  FHitResult HitResult = TraceInfo.HitResult;

  if (!HadHit) {
    if (FocusedActor) {
      IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
      if (Interface) {
        Interface->Execute_EndFocus(FocusedActor);
      }
    }
    FocusedActor = nullptr;
    return;
  }

  //DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
  AActor* InteractableActorWeHit = HitResult.GetActor();
  if (!InteractableActorWeHit) {
    return;
  }

  if (InteractableActorWeHit == FocusedActor) {
    return;
  }

  //If everything is valid, and we have a previous focusedActor.. 
  //end focus on that focused actor and start focus on our new HitActorInteractable actor
  if (FocusedActor) {
    IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
    if (Interface) {
      Interface->Execute_EndFocus(FocusedActor);
    }
  }
  IInteractInterface* Interface = Cast<IInteractInterface>(InteractableActorWeHit);
  if (Interface) {
    Interface->Execute_StartFocus(InteractableActorWeHit);
  }
  FocusedActor = InteractableActorWeHit;
  

}

void ARB_CC_MyCharacter::CameraShakeDemo(float Scale)
{

  GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShake, Scale);

}

void ARB_CC_MyCharacter::FireForwardClientTrace() {
  if (!HasAuthority()) {
    struct ForwardTraceHitInformation TraceInfo = GetForwardTraceHitInformation(false);
  }
}

void ARB_CC_MyCharacter::FireForward_Implementation(){

  if (!HasAuthority()) {
    //this can never happen really, probably dont need this block, its almost guarrantee/assumed that this function
    //is called by the server
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("I have no authority to fire, so not firing "), true, false, FColor::Orange, 3.0f);
    return; 
  }

  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("I have authority to fire, so firing "), true, false, FColor::Orange, 3.0f);
  FString IntAsString222 = FString::FromInt(DebugMyCharacter);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("DebugMyCharacter: ") + IntAsString222, true, false, FColor::Orange, 3.0f);

  struct ForwardTraceHitInformation TraceInfo = GetForwardTraceHitInformation(false);
  if (TraceInfo.Error) {
    return;
  }
  bool HadHit = TraceInfo.HadHit;
  FHitResult HitResult = TraceInfo.HitResult;

  //Check if we had a hit, return if not
  if (!HadHit) {
    return;
  }

  //https://www.udemy.com/course/unreal-engine-blueprints-the-ultimate-developer-course/learn/lecture/16386624#overview
  if (ApplyRadialForce){

    FString IntAsString2 = FString::FromInt(RadialImpactForce);
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("RadialImpactForce: ") + IntAsString2, true, false, FColor::Orange, 3.0f);
    FString IntAsString3 = FString::FromInt(ImpactRadius);
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ImpactRadius: ") + IntAsString3, true, false, FColor::Orange, 3.0f);
    FCollisionShape SphereCollision = FCollisionShape::MakeSphere(ImpactRadius);
    //Lets apply some radial force, so we can move actors around the actor we just hit
    TArray<FHitResult> HitResultsFromRadialForce;
    //FCollisionObjectQueryParams ObjectQueryParams;
    //ObjectQueryParams.AllDynamicObjects;
    //bool DidSweepHit = GetWorld()->SweepMultiByChannel(HitResultsFromRadialForce, HitResult.Location, HitResult.Location+(FVector(0.001f, 0.001f, 1.0f)),
    //  FQuat::Identity, ECC_WorldStatic, SphereCollision);
    // Be careful with these sweeps, since they actually ahve to "sweep'/move to detect collision, so start and end need some difference so
    //things can actually sweep 
    //https://www.youtube.com/watch?v=Sg1siSsT2-0
    bool DidSweepHit = GetWorld()->SweepMultiByChannel(HitResultsFromRadialForce, HitResult.Location, HitResult.Location + FVector(0.0f, 0.0f, 0.001f),
      FQuat::Identity, ECC_WorldStatic, SphereCollision);
    
    AddClientDrawDebugSphere(GetWorld(), HitResult.Location, ImpactRadius, 35, FColor::Orange, false, 3.0f);
    //DrawDebugSphere(GetWorld(), HitResult.Location, ImpactRadius, 35, FColor::Orange, false, 3.0f);
    int results = HitResultsFromRadialForce.Num();
    FString IntAsString = FString::FromInt(results);
    //UKismetSystemLibrary::PrintString(GetWorld(), IntAsString, true, false, FColor::Orange, 3.0f);
    SpawnExplosion(HitResult.Location, FVector(ImpactRadius, ImpactRadius, ImpactRadius));
    
    //if (DidSweepHit){
      for (auto& Hit : HitResultsFromRadialForce) {
        //Can we move the root component of what we hit? if not return
        if (!Hit.GetActor()->IsRootComponentMovable()) {
          continue;
        }
        //Do we have a static mesh for what we hit? otherwise return
        UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
        //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Sweeped across: ") + Hit.GetActor()->GetName(), true, false, FColor::Orange, 3.0f);
        if (MeshComp) {
          //AddClientDrawDebugSphere(GetWorld(), HitResult.Location, 20.0f, 35, FColor::Red, false, 3.0f);
          if (DebugMyCharacter == DEBUG_FIREFORWARD || DebugMyCharacter == DEBUG_ALL) {
            //DrawDebugSphere(GetWorld(), HitResult.Location, 20.0f, 35, FColor::Red, false, 3.0f);
            //DrawDebugLine(GetWorld(), HitResult.Location, HitResult.Location + (FVector(ImpactRadius, 0.0f, 0.0f)), FColor::Red, false, 3.0f, 0, 10.0f);
            //DrawDebugLine(GetWorld(), HitResult.Location, HitResult.Location + (FVector(0.0f, ImpactRadius, 0.0f)), FColor::Red, false, 3.0f, 0, 10.0f);
            //DrawDebugLine(GetWorld(), HitResult.Location, HitResult.Location + (FVector(0.0f, 0.0f, ImpactRadius)), FColor::Red, false, 3.0f, 0, 10.0f);
            //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Had mesh"), true, false, FColor::Orange, 3.0f);
            ////https://forums.unrealengine.com/t/how-to-find-center-of-mass/285687
            ////https://cpp.hotexamples.com/examples/-/PxRigidBody/addForce/cpp-pxrigidbody-addforce-method-examples.html
            //DrawDebugSphere(GetWorld(), MeshComp->GetBodyInstance()->GetCOMPosition(), 10.0f, 32, FColor::Orange, false, 3.0f);
          }

          //float ActorCOMZPosDiffFromHitOriginZPos = FMath::Abs(MeshComp->GetBodyInstance()->GetCOMPosition().Z - HitResult.Location.Z);
          FVector HitOriginLocation = HitResult.Location;
          FVector diff = MeshComp->GetBodyInstance()->GetCOMPosition() - HitOriginLocation;
          float RealDistanceFromHitOriginToActorCOM = ImpactRadius;
          if (UseActorsCenterOfMassInCollisionCalculation) {
            RealDistanceFromHitOriginToActorCOM = diff.Size();
          }
          FPhysicsActorHandle PAH = MeshComp->GetBodyInstance()->GetPhysicsActorHandle();
          PxRigidBody* Actor = FPhysicsInterface::GetPxRigidBody_AssumesLocked(PAH);
          if (!Actor) {
            continue;
          }
          PxTransform PCOMTransform = Actor->getGlobalPose().transform(Actor->getCMassLocalPose());
          PxVec3 PCOMPos = PCOMTransform.p; // center of mass in world space
          PxVec3 POrigin = U2PVector(HitResult.Location); // origin of radial impulse, in world space
          PxVec3 PDelta = PCOMPos - POrigin; // vector from origin to COM

          float Mag = PDelta.magnitude(); // Distance from COM to origin, in Unreal scale : @todo: do we still need conversion scale?
          FString IntAsString33 = FString::FromInt(Mag);
          //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Mag is: ") + IntAsString33, true, false, FColor::Orange, 3.0f);
          // If COM is outside radius, do nothing.
          if (Mag > ImpactRadius)
          {
            //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WTF!"), true, false, FColor::Orange, 3.0f);
          }

          MeshComp->AddRadialImpulse(HitResult.Location, RealDistanceFromHitOriginToActorCOM, RadialImpactForce, ERadialImpulseFalloff::RIF_Constant, true);
        }
      }
  }


  //Do we have a static mesh for what we hit? otherwise return
  UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(HitResult.GetActor()->GetRootComponent());
  if (!MeshComp) {
    return;
  }


  //Can we move the root component of what we hit? if not return
  if (!HitResult.GetActor()->IsRootComponentMovable()) {
    return;
  }

  //Everything checks out! Move that sucker! Impulse!
  FVector CameraForward = CameraComp->GetForwardVector();
  MeshComp->AddImpulse(CameraForward * ImpulseForce * MeshComp->GetMass());

}

// Called every frame
void ARB_CC_MyCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  TraceForward();
  SquashTimeline.TickTimeline(DeltaTime);

}

// Called to bind functionality to input
void ARB_CC_MyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this, &ARB_CC_MyCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ARB_CC_MyCharacter::MoveRight);


  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
  PlayerInputComponent->BindAxis("TurnRate", this, &ARB_CC_MyCharacter::TurnAtRate);
  PlayerInputComponent->BindAxis("LookUpRate", this, &ARB_CC_MyCharacter::LookUpAtRate);

  PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARB_CC_MyCharacter::InteractPressed);
  PlayerInputComponent->BindAction("SpawnActor", IE_Pressed, this, &ARB_CC_MyCharacter::SpawnActorAtLocation);
  PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARB_CC_MyCharacter::FireForward);
  PlayerInputComponent->BindAction("Fire2", IE_Pressed, this, &ARB_CC_MyCharacter::FireForwardClientTrace);
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

  PlayerInputComponent->BindAction<FShakeDelegate>(TEXT("CameraShake"), IE_Pressed, this, &ARB_CC_MyCharacter::CameraShakeDemo, 1.0f);
  //PlayerInputComponent->BindAction("LMBDown", IE_Pressed, this, &AMainCharacter::LMBDown);


}



ForwardTraceHitInformation ARB_CC_MyCharacter::GetForwardTraceHitInformation(bool DrawDebugTraceLine)
{
  FVector Loc;
  FRotator Rot;
  FHitResult Hit;
  struct ForwardTraceHitInformation Result;


  if (!Controller){ 
    Result.Error = true;
    return Result;
  }


  Controller->GetPlayerViewPoint(Loc, Rot);

  //FVector StartCam = Loc;
  
  //FVector EndCam = StartCam + (Rot.Vector() * 4000);

  //FVector SocketOffset = SpringArmComp->SocketOffset;
  //FString TheFloatStrx = FString::SanitizeFloat(SocketOffset.X);
  //FString TheFloatStry = FString::SanitizeFloat(SocketOffset.Y);
  //FString TheFloatStrz = FString::SanitizeFloat(SocketOffset.Z);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("X: ")+TheFloatStrx+TEXT(" -- ")TEXT("Y: ") + TheFloatStry + TEXT(" -- ")TEXT("Z: ") + TheFloatStrz, true, false, FColor::Orange, 3.0f);
  //FVector Start = Loc;
  //float HalfMeshSize = 50.0f;
  //FVector Start = GetCapsuleComponent()->GetCenterOfMass() + (Rot.Vector() * HalfMeshSize);
  //FVector Start = FVector(Loc.X, Loc.Y, GetActorLocation().Z);
  //FVector End = Start + SocketOffset + (Rot.Vector() * 4000);
  //FVector End = Start + (Rot.Vector() * 4000);

  //Start =  End - (GetActorLocation() + Rot.Vector());
  //Change the start to be the player location with a bit of forward offset

  FVector Start = Loc;
  FVector End = Start + (Rot.Vector() * 6000);
  FCollisionQueryParams TraceParams;
  bool HadHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, TraceParams);
  if (DrawDebugTraceLine){
    DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 1.0f, 0, 1.0f);
  }

  Result.Error = false;
  Result.HadHit = HadHit;
  Result.Start = Start;
  Result.End = End;
  Result.HitResult = Hit;
  Result.Rot = Rot;

  return Result;
}


void ARB_CC_MyCharacter::SpawnActorAtLocation_Implementation()
{

  //This is just here to remind the reader that this shoud be a server RPC(only done/called on the server)
  if (!HasAuthority()) {
    return;
  }

  struct ForwardTraceHitInformation TraceInfo = GetForwardTraceHitInformation(false);
  if (TraceInfo.Error) {
    return;
  }
  bool HadHit = TraceInfo.HadHit;
  FHitResult HitResult = TraceInfo.HitResult;

  if (HadHit) {
    FActorSpawnParameters SpawnParams;
    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, HitResult.Location, TraceInfo.Rot, SpawnParams);
  }
}


//void ARB_CC_MyCharacter::AddClientOnScreenDebugMessage_Implementation(int32 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage)
//{
//  if (DebugMyCharacter == DEBUG_FIREFORWARD || DebugMyCharacter == DEBUG_ALL) {
//    GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, DisplayColor, DebugMessage);
//  }
//}


void ARB_CC_MyCharacter::SpawnExplosion_Implementation(FVector Location, FVector Scale)
{

  UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RadialExplosionEffect, Location, FRotator::ZeroRotator, (FVector)(1.0f));
}

void ARB_CC_MyCharacter::AddClientDrawDebugSphere_Implementation(const UWorld* InWorld, FVector const& Center, float Radius, int32 Segments, FColor const& Color, bool bPersistentLines /*= false*/, float LifeTime /*= -1.f*/)
{
  if (DebugMyCharacter == DEBUG_FIREFORWARD || DebugMyCharacter == DEBUG_ALL) {
    DrawDebugSphere(InWorld, Center, Radius, Segments, Color, bPersistentLines, LifeTime);
  }
}

void ARB_CC_MyCharacter::Landed(const FHitResult& Hit)
{
  
  Super::Landed(Hit);
  if (CurveFloat) {
    FOnTimelineFloat TimeLineProgress;
    TimeLineProgress.BindUFunction(this, FName("SquashProgress"));
    SquashTimeline.AddInterpFloat(CurveFloat, TimeLineProgress); 
    SquashTimeline.SetLooping(false);
    SquashTimeline.PlayFromStart();
  }


}

void ARB_CC_MyCharacter::SquashProgress(float Value)
{

  FString TheFloatStr = FString::SanitizeFloat(Value);
  //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, TEXT("SquashProgress value: ") + TheFloatStr);
  FVector NewScale = FMath::Lerp(StartScale, TargetScale, Value);
  StaticMeshComp->SetWorldScale3D(NewScale);
}

