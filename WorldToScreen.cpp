Vector3 ProjectionWorldToScreen(Vector3 WorldLocation, Vector3 camrot) //
{

	auto one = read<uintptr_t>(DriverHandle, processID, Localplayer + 0xa8); // =
	uint64_t two = read<uintptr_t>(DriverHandle, processID, one + 8);

        Vector3 Camera;
	Camera.x = read<float>(DriverHandle, processID, two + 0x8F8);
	Camera.y = read<float>(DriverHandle, processID, Rootcomp + 0x12C);  

	float testing = asin(Camera.x);
	float degrees = testing * (180.0 / M_PI);
	Camera.x = degrees;

	if (Camera.y < 0)
		Camera.y = 360 + Camera.y;

	D3DMATRIX tempMatrix = Matrix(Camera);
	
        Vector3 vAxisX, vAxisY, vAxisZ;
	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	uint64_t three = read<uint64_t>(DriverHandle, processID, Localplayer + 0x70);
	uint64_t four = read<uint64_t>(DriverHandle, processID, three + 0x98);
	uint64_t five = read<uint64_t>(DriverHandle, processID, four + 0x140);

	Vector3 vDelta = WorldLocation - read<Vector3>(DriverHandle, processID, five + 0x10);
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float zoom = read<float>(DriverHandle, processID, two + 0x580);

	FovAngle = 80.0f / (zoom / 1.19f);
	float ScreenCenterX = Width / 2.0f;
	float ScreenCenterY = Height / 2.0f;

        Vector3 Screenlocation = Vector3(0, 0, 0);
	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	CameraEXT = Camera;

	return Screenlocation;
}  // 
