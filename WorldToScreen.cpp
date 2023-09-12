struct CamewaDescwipsion
{
    Vector3 Location;
    Vector3 Rotation;
    float FieldOfView;
};
 
CamewaDescwipsion GetViewPoint()
{
    CamewaDescwipsion ViewPoint;
    __int64 result; // rax
    __int64 v7; // rcx
 
    result = read<__int64>(Base + 0xE2CED18);
    v7 = Base + 0xE2CECD8;
    if (result)
        v7 = result;
 
    __int64 EncryptedData[7];
    ReadProcessMemory(Handle, (LPCVOID)v7, (LPVOID)EncryptedData, 56, (SIZE_T*)0);
    EncryptedData[0] ^= 0x4CF1F15DFE2D977Fi64; 
    EncryptedData[1] ^= 0x4CF1F15DFE2D977Fi64; 
    EncryptedData[2] ^= 0x4EF1F15DFE2D977Fi64; 
    EncryptedData[3] ^= 0xAF29A7F813EFC5Ci64; 
    EncryptedData[4] ^= 0xAF29A7F813EFC5Ci64; 
    EncryptedData[5] ^= 0x4EF29A7E813EFC5Di64; 
    EncryptedData[6] ^= 0x4E1772384C14291Fi64; 
 
    ViewPoint.Location = { *(double*)(&EncryptedData[1]),*(double*)(&EncryptedData[0]), *(double*)(&EncryptedData[2]) };
    ViewPoint.Rotation = { *(double*)(&EncryptedData[4]),*(double*)(&EncryptedData[3]), *(double*)(&EncryptedData[5]) };
    ViewPoint.FieldOfView = *(float*)(&EncryptedData[6]);
    
    return ViewPoint;
}
 

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	       CamewaDescwipsion ViewPoint = GetViewPoint();

		D3DMATRIX tempMatrix = Matrix(ViewPoint.Rotation);
		
		Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		Vector3 vDelta = WorldLocation - ViewPoint.Location;
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;

		return Vector3((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(ViewPoint.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(ViewPoint.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z,0);
}
