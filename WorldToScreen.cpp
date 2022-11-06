struct Camera
{
	float FieldOfView;
	Vector3 Rotation;
	Vector3 Location;
};

Camera GetCamera()
{
	Camera LocalCamera;
	__int64 v1;
	__int64 v6;
	__int64 v7;
	__int64 v8;

	v1 = read<__int64>(Localplayer + 0xd5);
	__int64 v9 = read<__int64>(v1 + 0xc);

	LocalCamera.FieldOfView = read<double>(v9 + 0x6DF);
	
	LocalCamera.Rotation.x = read<double>(v9 + 0x7D3);
        LocalCamera.Rotation.y = read<double>(v9 + 0x7DD);
 	LocalCamera.Rotation.z = read<double>(v9 + 0x7EC);
	
        LocalCamera.Location = read<Vector3>(read<__int64>(GWorld + 0x100));

	return VirtualCamera;
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	        Camera vCamera = GetCamera();
		vCamera.Rotation.x = (asin(vCamera.Rotation.x)) * (180.0 / M_PI);

		D3DMATRIX tempMatrix = Matrix(vCamera.Rotation);
		
		Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		Vector3 vDelta = WorldLocation - vCamera.Location;
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;

		return Vector3((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z,0);
}
