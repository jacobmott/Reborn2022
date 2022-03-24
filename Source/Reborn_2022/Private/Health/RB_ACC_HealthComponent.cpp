// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/RB_ACC_HealthComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Characters/RB_CC_MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

// Sets default values for this component's properties
URB_ACC_HealthComponent::URB_ACC_HealthComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = false;

  DefaultHealth = 100;
  Health = DefaultHealth;
  SetIsReplicated(true);

  // ...
}


// Called when the game starts
void URB_ACC_HealthComponent::BeginPlay()
{
  Super::BeginPlay();

  AActor* Owner = GetOwner();
  if (Owner) {
    Owner->OnTakeAnyDamage.AddDynamic(this, &URB_ACC_HealthComponent::HandleTakeAnyDamage);
  }

  SetIsReplicated(true);
  
}


void URB_ACC_HealthComponent::OnRep_Health()
{
  AActor* Owner = GetOwner();

  ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(Owner);
  if (!MyCharacter) { return; }
  AController* MyCharacterController = MyCharacter->GetController();
  //If this is a valid character class and we control it.. then update our hud(health)
  if (MyCharacterController && MyCharacterController->IsLocalPlayerController()) {
    MyCharacter->UpdateHud();
  }
  MyCharacter->UpdateFloatingHealthHud();


  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ACC_HealthComponent: OnRep_Health: ")+ Owner->GetName(), true, false, FColor::Green, 30.0f);
  //APawn* tempPawn = Cast<APawn>(Owner);
  //if (tempPawn->IsLocallyControlled()) {
  //if (Owner->GetLocalRole() == ROLE_SimulatedProxy) {
  //APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  //AActor* PCActor = Cast<AActor>(PC);
  //if ( Owner == GetWorld()->GetFirstPlayerController()->GetOwner() ) {
  //if ( Owner == PCActor) {
  //ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(Owner);

}

void URB_ACC_HealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
  FString TheFloatStr = FString::SanitizeFloat(Damage);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ACC_HealthComponent: HandleTakeAnyDamage: Damage: ")+ TheFloatStr, true, false, FColor::Red, 5.0f);
  //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("HandleTakeAnyDamage! here1 "));
  if (Damage < 0) {
    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("HandleTakeAnyDamage! Damage is less than 0 "));
    return;
  }

  Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
  AActor* CurrentOwnerThatTookDmg = GetOwner();
  ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(CurrentOwnerThatTookDmg);
  APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

  //If this is not a MyCharacter class or its not controlled by the server (since we know we are in a server call here)
  //Then dont proceed to call update hud on this mycharacter since its not the current controlled player for this instance/server
  //On the server we always have a controller for each character, thats why We dont technically have to check if GetController returns, but we do anyways
  AController* MyCharacterController = MyCharacter->GetController();
  if (!MyCharacter){ return; }
  MyCharacter->UpdateFloatingHealthHud();
  if (!MyCharacterController) { return; }
  if (!MyCharacterController->IsLocalPlayerController()) {
    //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ACC_HealthComponent: HandleTakeAnyDamage: DamagedActor == CurrentOwnerThatTookDmg? Yes!"), true, false, FColor::Blue, 30.0f);
    return;
  }
  MyCharacter->UpdateHud();
  //if (GetOwner()->HasAuthority() && GetOwner()->GetRemoteRole() != ROLE_SimulatedProxy){
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ACC_HealthComponent: HandleTakeAnyDamage: Damage: ") + DamagedActor->GetName() + " : " + GetOwner()->GetName(), true, false, FColor::Blue, 30.0f);

  //If we are the server, since we wont get the rep notify, we need to trigger the UI update for the server(when we are running in listen client/server, where
  //there server is a client as well, since in that case the server has a UI of the health bar
  //But we only update the UI when the actor that took damage is the actor/playercontroller for the server
  //GetOwner()->HasAuthority() && DONT NEED THIS, Since this willl only ever be called on the server
  //ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(CurrentOwnerThatTookDmg);
  //APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  //ARB_CC_MyCharacter* ThisCharacter = Cast<ARB_CC_MyCharacter>(PC);
  //AActor* PCActor = Cast<AActor>(PC);
  //PC->GetPawn();
  //FString CurrentPlayerName = PCActor->GetName();
  //FString DamagedPlayerName = MyCharacter->GetName();
  //FString ThisCharacterName = ThisCharacter->GetName();
  //uint32 CurrentPlayerID = PC->GetPlayerState<APlayerState>()->GetUniqueID();
  //AActor* CurrentPlayer = PC->GetOwner();
  //FString Name = CurrentPlayer->GetName();
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ACC_HealthComponent: HandleTakeAnyDamage:"), true, false, FColor::Blue, 30.0f);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("  -Owner MyCharacter->GetName(): ") + DamagedPlayerName, true, false, FColor::Blue, 30.0f);
  //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("  -PlayerController: ") + ThisCharacterName, true, false, FColor::Blue, 30.0f);
  //if (CurrentOwnerThatTookDmg == PCActor) {
  //ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(CurrentOwnerThatTookDmg);
  //if (MyCharacter && MyCharacter->GetController()) {
    //ARB_CC_MyCharacter* MyCharacter = Cast<ARB_CC_MyCharacter>(CurrentOwnerThatTookDmg);
  //}
  //}


}


float URB_ACC_HealthComponent::GetHealth()
{
  return Health;
}


float URB_ACC_HealthComponent::GetDefaultHealth()
{
  return DefaultHealth;
}

void URB_ACC_HealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(URB_ACC_HealthComponent, Health);
}

// Called every frame
//void URB_ACC_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

