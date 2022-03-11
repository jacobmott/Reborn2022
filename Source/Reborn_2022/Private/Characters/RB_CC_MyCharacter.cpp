// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RB_CC_MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Interact/InteractInterface.h"

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

  BaseTurnRate = 45.0f;
  BaseLookUpAtRate = 45.0f;
  TraceDistance = 2000.0f;
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

// Called when the game starts or when spawned
//void ARB_CC_MyCharacter::BeginPlay()
//{
// 	Super::BeginPlay();
//  //If this doesnt work from including it in the contstructor then you should add it here and enable/uncomment begin play
//  StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARB_CC_MyCharacter::OnOverlapBegin);
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

  TraceForward();
  if (FocusedActor) {
    IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
    if (Interface) {
      Interface->Execute_OnInteract(FocusedActor, this);
    }
  }
}

void ARB_CC_MyCharacter::TraceForward_Implementation()
{
  FVector Loc;
  FRotator Rot;
  FHitResult Hit;

  GetController()->GetPlayerViewPoint(Loc, Rot);

  FVector Start = Loc;
  FVector End = Start + (Rot.Vector() * 2000);

  FCollisionQueryParams TraceParams;
  bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
  //DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f, 0, 1.0f);
 

  if (bHit) {
    //DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
    AActor* Interactable = Hit.GetActor();  
    if (Interactable) {
      if (Interactable != FocusedActor) {
        if (FocusedActor) {
          IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
          if (Interface) {
            Interface->Execute_EndFocus(FocusedActor);
          }
        }
        IInteractInterface* Interface = Cast<IInteractInterface>(Interactable);
        if (Interface) {
          Interface->Execute_StartFocus(Interactable);
        }
        FocusedActor = Interactable;
      }

    }
  }
  else {
    if (FocusedActor) {
      IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
      if (Interface) {
        Interface->Execute_EndFocus(FocusedActor);
      }
    }
    FocusedActor = nullptr;
  }
}

// Called every frame
void ARB_CC_MyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  TraceForward();

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
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

  //PlayerInputComponent->BindAction("LMBDown", IE_Pressed, this, &AMainCharacter::LMBDown);


}

