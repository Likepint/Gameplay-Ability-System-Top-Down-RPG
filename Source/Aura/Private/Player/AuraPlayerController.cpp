#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// check(): AuraContext 포인터 유효성 검사
	// - 기능: nullptr일 경우 에디터 강제 종료로 빠른 에러 확인
	check(AuraContext);

	// 로컬 플레이어의 Enhanced Input Subsystem 인스턴스 획득
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(subsystem);
	// 서브시스템에 AuraContext 추가하여 입력 활성화, 우선순위 0
	subsystem->AddMappingContext(AuraContext, 0);

	// 마우스 커서 표시 활성화
	bShowMouseCursor = true;
	// 기본 마우스 커서 모양 설정
	DefaultMouseCursor = EMouseCursor::Default;

	// 입력 모드 '게임 & UI'로 설정
	FInputModeGameAndUI inputModeData;
	// - 마우스 뷰포트 잠금 해제 (창 밖으로 이동 가능)
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// - 마우스 캡처 시 커서 숨김 비활성화
	inputModeData.SetHideCursorDuringCapture(false);
	// 설정된 입력 모드 적용
	SetInputMode(inputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 기본 InputComponent를 EnhancedInputComponent로 형 변환 (실패 시 강제 종료)
	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 입력값에서 2D 벡터(X, Y) 추출 (예: WASD, 게임패드 스틱)
	const FVector2D inputAxisVector = InputActionValue.Get<FVector2d>();
	
	// 현재 컨트롤러(카메라)의 회전 값 획득
	const FRotator rotation = GetControlRotation();
	// 캐릭터 수직 이동 방지를 위해 Yaw(좌우 회전) 값만 사용
	const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);
	
	// Yaw 회전 기준 월드 공간의 '앞쪽' 방향 벡터 계산
	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	// Yaw 회전 기준 월드 공간의 '오른쪽' 방향 벡터 계산
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	// 현재 조종 중인 폰(Pawn) 확인 및 획득
	if (APawn* controlledPawn = GetPawn<APawn>())
	{
		// 폰의 이동 컴포넌트에 이동 요청 (앞/뒤)
		controlledPawn->AddMovementInput(forwardDirection, inputAxisVector.Y);
		// 폰의 이동 컴포넌트에 이동 요청 (좌/우)		
		controlledPawn->AddMovementInput(rightDirection, inputAxisVector.X);
	}
}
