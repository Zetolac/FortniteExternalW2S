struct Camera
{
	float FieldOfView;
	Vector3 Rotation;
	Vector3 Location;
};
struct FQuat
{
    double X;
    double Y;
    double Z;
    double W;
};

struct FRotator
{
    double Pitch;
    double Yaw;
    double Roll;
};

FRotator* Rotator(FQuat* Current, struct FRotator* retstr)
{
    float Z; // xmm4_4
    float Y; // xmm0_4
    float W; // xmm2_4
    float v7; // xmm5_4
    float v8; // xmm7_4
    float v9; // xmm8_4
    float v10; // xmm0_4
    float v11; // xmm1_4
    float v12; // xmm7_4
    float X; // xmm0_4
    float v14; // xmm0_4
    float v15; // xmm0_4
    float v16; // xmm0_4
    float v17; // xmm1_4
    float v18; // xmm5_4
    float v19; // xmm2_4
    float v20; // xmm4_4
    float v21; // xmm0_4
    float v22; // xmm0_4
    float v23; // xmm2_4
    float v24; // xmm4_4

    Z = Current->Z;
    Y = Current->Y;
    W = Current->W;
    v7 = (float)(Current->X * Z) - (float)(Y * W);
    v8 = (float)((float)(Y * Current->X) + (float)(Y * Current->X)) + (float)((float)(W * Z) + (float)(W * Z));
    v9 = 1.0 - (float)((float)((float)(Y * Y) + (float)(Y * Y)) + (float)((float)(Z * Z) + (float)(Z * Z)));
    if (v7 >= -0.49999949)
    {
        if (v7 <= 0.49999949)
        {
            v18 = v7 + v7;
            v19 = v18;
            v20 = sqrtf(fmaxf(1.0 - v19, 0.0))
                * (float)((float)((float)((float)((float)((float)((float)((float)((float)((float)((float)((float)((float)(0.0066700899 - (float)(v19 * 0.0012624911)) * v19)
                    - 0.017088126)
                    * v19)
                    + 0.03089188)
                    * v19)
                    - 0.050174303)
                    * v19)
                    + 0.088978991)
                    * v19)
                    - 0.2145988)
                    * v19)
                    + 1.5707963);
            if (v18 < 0.0)
                v21 = v20 - 1.5707963;
            else
                v21 = 1.5707963 - v20;
            retstr->Pitch = v21 * 57.295776;
            v22 = atan2(v8, v9);
            v23 = Current->X * Current->X;
            v24 = Current->X * Current->W;
            retstr->Yaw = v22 * 57.295776;
            v15 = atan2(
                (float)((float)(Current->Y * Current->Z) * -2.0) - (float)(v24 + v24),
                1.0 - (float)((float)(v23 + v23) + (float)((float)(Current->Y * Current->Y) + (float)(Current->Y * Current->Y))))
                * 57.295776;
            goto LABEL_13;
        }
        retstr->Pitch = 90.0;
        v16 = atan2(v8, v9);
        v17 = Current->W;
        retstr->Yaw = v16 * 57.295776;
        v14 = retstr->Yaw - (float)((float)(atan2(Current->X, v17) * 2.0) * 57.295776);
    }
    else
    {
        retstr->Pitch = -90.0;
        v10 = atan2(v8, v9);
        v11 = Current->W;
        v12 = v10 * 57.295776;
        X = Current->X;
        retstr->Yaw = v12;
        //v14 = COERCE_FLOAT(LODWORD(v12) ^ _xmm) - (float)((float)(FGenericPlatformMath::Atan2(X, v11) * 2.0) * 57.295776);
    }
LABEL_13:
    retstr->Roll = 0;
    return retstr;
}

Camera GetCamera(__int64 a1)
{
	Camera LocalCamera;
	__int64 v1;
	__int64 v6;
	__int64 v7;
	__int64 v8;

	v1 = read<__int64>(Localplayer + 0xd0);
	__int64 v9 = read<__int64>(v1 + 8);

	LocalCamera.FieldOfView = read<float>(PlayerController + 0x38C) * 90.f);
	LocalCamera.Rotation.x = read<double>(v9 + 0x9C0);
	if(!read<uintptr_t>(LocalPawn + 0x23f8))
        LocalCamera.Rotation.y = read<double>(a1 + 0x148);
        else
	{
               __int64 ViewportClient = read<__int64>(Localplayer + 0x78);
    __int64 AudioHandle = read<__int64>(ViewportClient + 0xB8);
    FQuat ViewRotation = read<FQuat>(AudioHandle + 0x1E0);
    FRotator Buffer;
    Rotator(&ViewRotation, &Buffer);
LocalCamera.Rotation.y = Buffer.Yaw;
	}

        LocalCamera.Location = read<Vector3>(read<__int64>(GWorld + 0x100));

	return VirtualCamera;
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	        Camera vCamera = GetCamera(RootComponent);
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
