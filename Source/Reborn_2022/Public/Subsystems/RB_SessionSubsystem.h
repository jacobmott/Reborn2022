
#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSessionSettings.h"
#include "RB_SessionSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnDestroySessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FCSOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);


/**
 * 
 */
UCLASS()
class REBORN_2022_API URB_SessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
  URB_SessionSubsystem();

  void CreateSession(int32 NumPublicConnections, bool IsLANMatch, FString MapName, FString SessionName);
  void UpdateSession();
  void StartSession(FName SessionName);
  void EndSession(FName SessionName);
  void DestroySession(FName SessionName);
  void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
  void JoinGameSession(FString MapName, FName SessionName, const FOnlineSessionSearchResult& SessionResult);
  bool TryTravelToSession(const FOnlineSessionSearchResult& SessionResult);
  FName GetCurrentSession();
  FString GetCurrentMap();
  FOnlineSessionSearchResult GetCurrentOnlineSessionSearchResult();

  FCSOnCreateSessionComplete OnCreateSessionCompleteEvent;
  FCSOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
  FCSOnStartSessionComplete OnStartSessionCompleteEvent;
  FCSOnEndSessionComplete OnEndSessionCompleteEvent;
  FCSOnDestroySessionComplete OnDestroySessionCompleteEvent;
  FCSOnFindSessionsComplete OnFindSessionsCompleteEvent;
  FCSOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

protected:
  void OnCreateSessionCompleted(FName SessionName, bool Successful);
  void OnUpdateSessionCompleted(FName SessionName, bool Successful);
  void OnStartSessionCompleted(FName SessionName, bool Successful);
  void OnEndSessionCompleted(FName SessionName, bool Successful);
  void OnDestroySessionCompleted(FName SessionName, bool Successful);
  void OnFindSessionsCompleted(bool Successful);
  void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
  bool TryTravelToCurrentSession();
  bool TryTravelToLocalMap(FString MapName);
  FName CurrentSessionName;
  FString CurrentMapName;
  FOnlineSessionSearchResult CurrentOnlineSessionSearchResult;

private:
  FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
  FDelegateHandle CreateSessionCompleteDelegateHandle;
  TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

  FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
  FDelegateHandle UpdateSessionCompleteDelegateHandle;

  FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
  FDelegateHandle StartSessionCompleteDelegateHandle;

  FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
  FDelegateHandle EndSessionCompleteDelegateHandle;

  FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
  FDelegateHandle DestroySessionCompleteDelegateHandle;

  FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
  FDelegateHandle FindSessionsCompleteDelegateHandle;
  TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

  FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
  FDelegateHandle JoinSessionCompleteDelegateHandle;


};
