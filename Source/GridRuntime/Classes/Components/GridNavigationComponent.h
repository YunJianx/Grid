#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "Components/GridNavigationAgent.h"
#include "GridNavigationComponent.generated.h"

class UGrid;

UENUM(BlueprintType)
enum class EGridNavMode : uint8
{
	GridBased,
	Free,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridNavEventSignature, UGridNavigationComponent*, Component);

UCLASS( ClassGroup=(Grid), Blueprintable, meta=(BlueprintSpawnableComponent) )
class GRIDRUNTIME_API UGridNavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGridNavigationComponent();
	virtual ~UGridNavigationComponent();

	virtual void BeginPlay() override;

	/**
	*	@note make sure your character's "Max Acceleration" large enough for smooth moving
	*/
	UFUNCTION(BlueprintCallable, Category = "GridNavigationComponent")
	virtual bool RequestMove(UGrid* DestGrid);

	UFUNCTION(BlueprintCallable, Category = "GridNavigationComponent")
	virtual bool IsMoving();

	UPROPERTY(BlueprintAssignable, Category = "GridNavigationComponent")
	FGridNavEventSignature OnArrivalNewGrid;

	UPROPERTY(BlueprintAssignable, Category = "GridNavigationComponent")
	FGridNavEventSignature OnArrivalGoal;

	/** if you implement a new GridNavigationAgent, you should add that class to this Array */
	UPROPERTY(EditDefaultsOnly, Category = "Grid")
	TArray<TSubclassOf<UGridNavigationAgent> > AgentClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GridNavigationComponent")
	EGridNavMode NavMode;

protected:
	bool MoveToNext();
	virtual bool MoveToNextGrid();
	virtual bool MoveToNextPoint();

	UGridNavigationAgent* FindAgent(UGrid* Start, UGrid* Goal);

	UFUNCTION()
	virtual void OnMoveCompleted(APawn* Pawn, bool Succ);

protected:
	UPROPERTY()
	APawn* OwnerPawn;

	UPROPERTY()
	AAIController* OwnerController;

	int FollowingPathIndex;

	bool bIsMoving;

	UPROPERTY()
	TArray<UGrid*> CurrentFollowingPath;

	UPROPERTY()
	TArray<UGridNavigationAgent*> Agents;
};
