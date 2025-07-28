#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// - bOrientRotationToMovement가 true이면, 캐릭터는 AddMovementInput으로 지정된 방향을 바라보게 됨
	// - 기능: 캐릭터가 이동하는 방향으로 자동으로 회전하도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// - FRotator(Pitch, Yaw, Roll) 순서이며, Yaw(좌우) 축을 기준으로 초당 400도의 속도로 회전
	// - 기능: 캐릭터의 회전 속도 설정
	GetCharacterMovement()-> RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	// - 주로 탑다운이나 쿼터뷰 게임에서 캐릭터가 공중으로 뜨거나 땅 아래로 꺼지는 것을 방지
	// - 기능: 캐릭터의 움직임을 특정 평면(기본값: XY 평면)으로 제한
	GetCharacterMovement()->bConstrainToPlane = true;
	// - bConstrainToPlane이 true일 때 함께 사용
	// - 기능: 게임 시작 시 캐릭터를 즉시 제한 평면에 맞춤(Snap)
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// [폰(Pawn)의 컨트롤러 회전 값 사용 여부 설정]
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
