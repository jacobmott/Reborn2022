#include "Subsystems/RB_SessionSubsystem.h"

#include "OnlineSubsystemUtils.h"
#include "Kismet/KismetSystemLibrary.h"

URB_SessionSubsystem::URB_SessionSubsystem()
  : CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted))
  , UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted))
  , StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted))
  , EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnEndSessionCompleted))
  , DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted))
  , FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted))
  , JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted))
{
}

void URB_SessionSubsystem::CreateSession(int32 NumPublicConnections, bool IsLANMatch, FString MapName)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {

    OnCreateSessionCompleteEvent.Broadcast(false);
    return;
  }
  LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
  LastSessionSettings->NumPrivateConnections = 0;
  LastSessionSettings->NumPublicConnections = NumPublicConnections;
  LastSessionSettings->bAllowInvites = true;
  LastSessionSettings->bAllowJoinInProgress = true;
  LastSessionSettings->bAllowJoinViaPresence = true;
  LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
  LastSessionSettings->bIsDedicated = false;
  LastSessionSettings->bUsesPresence = true;
  LastSessionSettings->bIsLANMatch = IsLANMatch;
  LastSessionSettings->bShouldAdvertise = true;

  LastSessionSettings->Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService);

  CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
  const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
  if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
  {
    sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

    OnCreateSessionCompleteEvent.Broadcast(false);
  }

}

void URB_SessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (sessionInterface)
  {
    sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
  }

  OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void URB_SessionSubsystem::UpdateSession()
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {
    OnUpdateSessionCompleteEvent.Broadcast(false);
    return;
  }

  TSharedPtr<FOnlineSessionSettings> updatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
  updatedSessionSettings->Set(SETTING_MAPNAME, FString("Updated Level Name"), EOnlineDataAdvertisementType::ViaOnlineService);

  UpdateSessionCompleteDelegateHandle =
    sessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

  if (!sessionInterface->UpdateSession(NAME_GameSession, *updatedSessionSettings))
  {
    sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);

    OnUpdateSessionCompleteEvent.Broadcast(false);
  }
  else
  {
    LastSessionSettings = updatedSessionSettings;
  }
}

void URB_SessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (sessionInterface)
  {
    sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
  }

  OnUpdateSessionCompleteEvent.Broadcast(Successful);
}

void URB_SessionSubsystem::StartSession()
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {
    OnStartSessionCompleteEvent.Broadcast(false);
    return;
  }

  StartSessionCompleteDelegateHandle =
    sessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

  if (!sessionInterface->StartSession(NAME_GameSession))
  {
    sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

    OnStartSessionCompleteEvent.Broadcast(false);
  } 

  float thiskkll = 1.0f;
}

void URB_SessionSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (sessionInterface)
  {
    sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
  }

  OnStartSessionCompleteEvent.Broadcast(Successful);
}

void URB_SessionSubsystem::EndSession()
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {
    OnEndSessionCompleteEvent.Broadcast(false);
    return;
  }

  EndSessionCompleteDelegateHandle =
    sessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);

  if (!sessionInterface->EndSession(NAME_GameSession))
  {
    sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);

    OnEndSessionCompleteEvent.Broadcast(false);
  }
}

void URB_SessionSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (sessionInterface)
  {
    sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
  }

  OnEndSessionCompleteEvent.Broadcast(Successful);
}

void URB_SessionSubsystem::DestroySession()
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {
    OnDestroySessionCompleteEvent.Broadcast(false);
    return;
  }

  DestroySessionCompleteDelegateHandle =
    sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

  if (!sessionInterface->DestroySession(NAME_GameSession))
  {
    sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

    OnDestroySessionCompleteEvent.Broadcast(false);
  }
}

void URB_SessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (sessionInterface)
  {
    sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
  }

  OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void URB_SessionSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {
    OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
    return;
  }

  FindSessionsCompleteDelegateHandle =
    sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

  LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
  LastSessionSearch->MaxSearchResults = MaxSearchResults;
  LastSessionSearch->bIsLanQuery = IsLANQuery;
  LastSessionSearch->PingBucketSize = 4;
  LastSessionSearch->TimeoutInSeconds = 20;
  LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

  const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
  if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
  {
    sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
    OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
  }
}

void URB_SessionSubsystem::OnFindSessionsCompleted(bool Successful)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (sessionInterface)
  {
    sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
  }

  if (LastSessionSearch->SearchResults.Num() <= 0)
  {
    OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
    return;
  }
  
  OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults, Successful);
}

void URB_SessionSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SessionResult)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {
    OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
    return;
  }

  JoinSessionCompleteDelegateHandle =
    sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

  const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
  if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
  {
    sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

    OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
  }
}

void URB_SessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (sessionInterface)
  {
    sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
  }
  
  OnJoinGameSessionCompleteEvent.Broadcast(Result);
}

bool URB_SessionSubsystem::TryTravelToSession(const FOnlineSessionSearchResult& SessionResult)
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {
    return false;
  }

  FString connectString;
  if (!sessionInterface->GetResolvedConnectString(SessionResult, NAME_GamePort, connectString))
  {
    return false;
  }

  APlayerController* playerController = GetWorld()->GetFirstPlayerController();
  playerController->ClientTravel(connectString, TRAVEL_Absolute);
  return true;
}

bool URB_SessionSubsystem::TryTravelToCurrentSession()
{
  const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
  if (!sessionInterface.IsValid())
  {
    return false;
  }

  FString connectString;
  if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
  {
    return false;
  }

  APlayerController* playerController = GetWorld()->GetFirstPlayerController();
  playerController->ClientTravel(connectString, TRAVEL_Absolute);
  return true;
}

