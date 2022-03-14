
float GetCameraComponentFOV(__int64 PlayerCameraManager)
{
	float FieldOfViewReturn = 90.f; // DefaultFOV
	if (PlayerCameraManager) {
		__int64 AnimCameraActor = read<__int64>(PlayerCameraManager + 0x29b8);

		__int64 CameraComponent = read<__int64>(AnimCameraActor + 0x228);

		FieldOfViewReturn = read<float>(CameraComponent + 0x1f0); // CameraComponent->FieldOfView
	}

	return (float)FieldOfViewReturn;
}

// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Engine/Private/PlayerCameraManager.cpp#L835
// PlayerCameraManager = PlayerController + 0x2C8
namespace APlayerCameraManager
{
	float APlayerCameraManager::GetFOVAngle()
	{
		float LockedFOV = read<float>(PlayerCameraManager + 0x23c);
		return (LockedFOV > 0.f) ? LockedFOV : GetCameraComponentFOV(PlayerCameraManager);
	}

	void APlayerCameraManager::SetFOV(float NewFOV) 
	{
		write<float>(PlayerCameraManager + 0x23c, NewFOV); // Detected ^^
	}

	void APlayerCameraManager::UnlockFOV()
	{
		write<float>(PlayerCameraManager + 0x23c, 0.f);
	}
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);
	Vector3 Camera;

	auto chain69 = read<uintptr_t>(LocalPlayer + 0xa8);
	uint64_t chain699 = read<uintptr_t>(chain69 + 8);
	Camera.x = read<float>(chain699 + 0x8F8);
	Camera.y = read<float>(RootComponent + 0x12C);

	float test = asin(Camera.x);
	float degrees = test * (180.0 / M_PI);
	Camera.x = degrees;

	if (Camera.y < 0)
		Camera.y = 360 + Camera.y;

	D3DMATRIX tempMatrix = Matrix(Camera);
	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	uint64_t chain = read<uint64_t>(LocalPlayer + 0x70);
	uint64_t chain1 = read<uint64_t>(chain + 0x98);
	uint64_t chain2 = read<uint64_t>(chain1 + 0x140);
	Vector3 vDelta = WorldLocation - read<Vector3>(chain2 + 0x10); 
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float FovAngle = APlayerCameraManager::GetFOVAngle();
	float ScreenCenterX = Width / 2.0f; //  GetWindowRect(FortniteWindow, &GameRect); Width = GameRect.right - GameRect.left; 
	float ScreenCenterY = Height / 2.0f; // GetWindowRect(FortniteWindow, &GameRect); Height = GameRect.bottom - GameRect.top;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}
