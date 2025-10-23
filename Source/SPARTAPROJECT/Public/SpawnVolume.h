#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SPARTAPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
    
public:	
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	USceneComponent* Scene;
	// 스폰 영역을 담당할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category="Spawning")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem(); // 리턴 형식을 AActor* 로 변경

	// 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
	
	FVector GetRandomPointInVolume() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FItemSpawnRow* GetRandomItem() const;
};