#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASpartaCharacter::ASpartaCharacter()
{
		PrimaryActorTick.bCanEverTick = false;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent? ???
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA? ???? ?? ?? ?? ?? Controller? ASpartaPlayerController? ???
        if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move ?? ?? "?? ??? ?? ??" Move() ??
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::Move
                );
            }
            
            if (PlayerController->JumpAction)
            {
                // IA_Jump ?? ?? "?? ??? ?? ??" StartJump() ??
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::StartJump
                );
                
                // IA_Jump ?? ??? "?? ? ??" StopJump() ??
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &ASpartaCharacter::StopJump
                );
            }
            
            if (PlayerController->LookAction)
            {
                // IA_Look ?? ???? "??? ?" Look() ??
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::Look
                );
            }
            
            if (PlayerController->SprintAction)
            {
                // IA_Sprint ?? ?? "??? ?? ??" StartSprint() ??
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered, 
                    this, 
                    &ASpartaCharacter::StartSprint
                );
                // IA_Sprint ?? ??? "?? ? ??" StopSprint() ??
                EnhancedInput->BindAction(
                    PlayerController->SprintAction, 
                    ETriggerEvent::Completed, 
                    this, 
                    &ASpartaCharacter::StopSprint
                );
            }    
        }
    }
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
    // ????? ??? ?? ??? ??
    if (!Controller) return;

    // Value? Axis2D? ??? IA_Move? ??? (WASD)? ?? ??
    // ?) (X=1, Y=0) ? ?? / (X=-1, Y=0) ? ?? / (X=0, Y=1) ? ??? / (X=0, Y=-1) ? ??
    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // ???? ???? ??(??)?? X? ??
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        // ???? ??? ???? Y? ??
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
    // Jump ??? Character? ?? ??
    if (value.Get<bool>())
    {
        Jump();
    }
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
    // StopJumping ??? Character? ?? ??
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
    // ???? X, Y ???? 2D ??? ???
    FVector2D LookInput = value.Get<FVector2D>();

    // X? ?? ?? (Yaw), Y? ?? ?? (Pitch)
    // ?? ??
    AddControllerYawInput(LookInput.X);
    // ?? ??
    AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
    // Shift ?? ?? ?? ? ??? ????? ??
    // ???? ??? ??
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
    // Shift ?? ? ?? ? ??? ??
    // ??? ??? ??
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}