struct CamewaDescwipsion
{
    Vector3 Location;
    Vector3 Rotation;
    float FieldOfView;
    char Useless[0x18];
};
CamewaDescwipsion GetViewPoint()
{
    char v1; // r8
    CamewaDescwipsion ViewPoint = Read<CamewaDescwipsion>(base_address + 0xe9ad420);
    BYTE* v2 = (BYTE*)&ViewPoint;
    int i; // edx
    __int64 result; // rax
 
    v1 = 0x40;
    for (i = 0; i < 0x40; ++i)
    {
        *v2 ^= v1;
        result = (unsigned int)(i + 0x17);
        v1 += i + 0x17;
        v2++;
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
