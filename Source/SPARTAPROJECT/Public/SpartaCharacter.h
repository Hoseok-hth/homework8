#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
// Enhanced Input에서 액션 값을 받을 때 사용하는 구조체
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

	// 현재 체력을 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetHealth() const;
	// 체력을 회복시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	// 현재 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;


	// 이동 속도 관련 프로퍼티들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도
	
	// 입력 바인딩을 처리할 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		
	// IA_Move와 IA_Jump 등을 처리할 함수 원형
	// Enhanced Input에서 액션 값은 FInputActionValue로 전달됩니다.
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	
	// 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

	// 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
	// 또는 AActor의 TakeDamage()를 오버라이드
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
};