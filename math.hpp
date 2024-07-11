#pragma once

namespace Math
{
#define M_PI_F DirectX::XM_PI

#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)

#define ANGLE2SHORT(x) ((int)((x) * (65536 / 360)) & 0xFFFF)
#define SHORT2ANGLE(x) ((x) * (360 / 65536))

#define PITCH 0 // up / down
#define YAW 1   // left / right
#define ROLL 2  // fall over

#define DotProduct(x, y) (x[0] * y[0] + x[1] * y[1] + x[2] * y[2])
#define VectorSubtract(a, b, c)                                                                                        \
    {                                                                                                                  \
        c[0] = a[0] - b[0];                                                                                            \
        c[1] = a[1] - b[1];                                                                                            \
        c[2] = a[2] - b[2];                                                                                            \
    }
#define VectorAdd(a, b, c)                                                                                             \
    {                                                                                                                  \
        c[0] = a[0] + b[0];                                                                                            \
        c[1] = a[1] + b[1];                                                                                            \
        c[2] = a[2] + b[2];                                                                                            \
    }
#define VectorCopy(a, b)                                                                                               \
    {                                                                                                                  \
        b[0] = a[0];                                                                                                   \
        b[1] = a[1];                                                                                                   \
        b[2] = a[2];                                                                                                   \
    }
#define VectorScale(a, b, c)                                                                                           \
    {                                                                                                                  \
        c[0] = b * a[0];                                                                                               \
        c[1] = b * a[1];                                                                                               \
        c[2] = b * a[2];                                                                                               \
    }
#define VectorClear(x)                                                                                                 \
    {                                                                                                                  \
        x[0] = x[1] = x[2] = 0;                                                                                        \
    }
#define VectorNegate(x)                                                                                                \
    {                                                                                                                  \
        x[0] = -x[0];                                                                                                  \
        x[1] = -x[1];                                                                                                  \
        x[2] = -x[2];                                                                                                  \
    }
#define VectorSet(v, a, b, c)                                                                                          \
    {                                                                                                                  \
        v[0] = a;                                                                                                      \
        v[1] = b;                                                                                                      \
        v[2] = c;                                                                                                      \
    }

inline void VectorMA(const vec3_t &v1, float scale, const vec3_t &v2, vec3_t &vecOut)
{
    vecOut[0] = v1[0] + scale * v2[0];
    vecOut[1] = v1[1] + scale * v2[1];
    vecOut[2] = v1[2] + scale * v2[2];
}

inline float GetDistance3D(const vec3_t &v1, const vec3_t &v2)
{
    float dx = v2[0] - v1[0];
    float dy = v2[1] - v1[1];
    float dz = v2[2] - v1[2];

    return sqrtf(dx * dx + dy * dy + dz * dz);
}

inline vec_t VectorNormalize(vec3_t v)
{
    float length, ilength;

    length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length)
    {
        ilength = 1 / length;
        v[0] *= ilength;
        v[1] *= ilength;
        v[2] *= ilength;
    }
    return length;
}

inline void AngleNormalize(vec3_t &angle)
{
    while (angle[0] > 89.0f)
    {
        angle[0] -= 180.f;
    }

    while (angle[0] < -89.0f)
    {
        angle[0] += 180.f;
    }

    while (angle[1] > 180.f)
    {
        angle[1] -= 360.f;
    }

    while (angle[1] < -180.f)
    {
        angle[1] += 360.f;
    }

    angle[2] = 0.f;
}

inline float Length2D(const vec3_t &v)
{
    return (v[0] * v[0] + v[1] * v[1]);
}

inline void SinCos(float radians, float *sine, float *cosine)
{
    *sine = sinf(radians);
    *cosine = cosf(radians);
}

inline void Rotate2D(vec3_t &v, const float &f)
{
    float _x, _y;

    float s, c;
    SinCos(DEG2RAD(f), &s, &c);

    _x = v[0];
    _y = v[1];

    v[0] = (_x * c) - (_y * s);
    v[1] = (_x * s) + (_y * c);
}

inline void AngleVectors(const vec3_t &angles, vec3_t &forward, vec3_t &right, vec3_t &up)
{
    float angle;
    static float sr, sp, sy, cr, cp, cy;

    angle = angles[YAW] * (M_PI_F * 2 / 360);
    sy = sinf(angle);
    cy = cosf(angle);
    angle = angles[PITCH] * (M_PI_F * 2 / 360);
    sp = sinf(angle);
    cp = cosf(angle);
    angle = angles[ROLL] * (M_PI_F * 2 / 360);
    sr = sinf(angle);
    cr = cosf(angle);

    if (forward)
    {
        forward[0] = cp * cy;
        forward[1] = cp * sy;
        forward[2] = -sp;
    }
    if (right)
    {
        right[0] = -sr * sp * cy + cr * sy;
        right[1] = -sr * sp * sy - cr * cy;
        right[2] = -sr * cp;
    }
    if (up)
    {
        up[0] = cr * sp * cy + sr * sy;
        up[1] = cr * sp * sy - sr * cy;
        up[2] = cr * cp;
    }
}

inline void VectorAngles(const vec3_t &forward, vec3_t &angles)
{
    float tmp, yaw, pitch;

    if (forward[1] == 0 && forward[0] == 0)
    {
        yaw = 0;
        if (forward[2] > 0)
            pitch = 270;
        else
            pitch = 90;
    }
    else
    {
        yaw = (atan2f(forward[1], forward[0]) * 180 / M_PI_F);
        if (yaw < 0)
            yaw += 360;

        tmp = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);
        pitch = (atan2f(-forward[2], tmp) * 180 / M_PI_F);
        if (pitch < 0)
            pitch += 360;
    }

    angles[0] = yaw;
    angles[1] = pitch;
    angles[2] = 0;
}

inline void MakeVector(const vec3_t &angles, vec3_t &vecOut)
{
    const float pitch = DEG2RAD(angles[0]);
    const float yaw = DEG2RAD(angles[1]);

    vecOut[0] = -cosf(pitch) * -cosf(yaw);
    vecOut[1] = sinf(yaw) * cosf(pitch);
    vecOut[2] = -sinf(pitch);
}

inline float GetFov(const vec3_t &viewAngle, const vec3_t &aimAngle)
{
    vec3_t delta = {};
    VectorSubtract(aimAngle, viewAngle, delta);
    AngleNormalize(delta);
    return hypotf(delta[0], delta[1]);
}

inline void AnglesToAxis(const vec3_t &angles, const vec3_t &origin, vec3_t axis[3])
{
    vec3_t right = {};
    AngleVectors(angles, axis[0], right, axis[2]);
    VectorSubtract(origin, right, axis[1]);
}

inline void VectorToViewangles(const vec3_t &vecTarget, const vec3_t &origin, const vec3_t &viewAngles, vec3_t &vecOut)
{
    vec3_t vecEntity{};
    vec3_t vecAngle{};
    vec3_t axis[3]{};

    VectorSubtract(vecTarget, origin, vecEntity);

    VectorAngles(vecEntity, vecAngle);

    vecAngle[0] = -vecAngle[0];
    if (vecAngle[1] > 180.0f)
        vecAngle[1] -= 360.0f;
    else if (vecAngle[1] < -180.0f)
        vecAngle[1] += 360.0f;
    if (vecAngle[0] > 180.0f)
        vecAngle[0] -= 360.0f;
    else if (vecAngle[0] < -180.0f)
        vecAngle[0] += 360.0f;

    AnglesToAxis(vecAngle, origin, axis);

    vecAngle[1] -= viewAngles[1];

    if (vecAngle[1] > 180.0f)
        vecAngle[1] -= 360.0f;
    else if (vecAngle[1] < -180.0f)
        vecAngle[1] += 360.0f;
    vecAngle[0] -= viewAngles[0];
    if (vecAngle[0] > 180.0f)
        vecAngle[0] -= 360.0f;
    else if (vecAngle[0] < -180.0f)
        vecAngle[0] -= 360.0f;

    vecOut[0] = vecAngle[1];
    vecOut[1] = vecAngle[2];
}

inline void CalcAngle(const vec3_t &src, const vec3_t &dst, vec3_t &angles)
{
    vec3_t delta = {};
    VectorSubtract(src, dst, delta);

    Math::VectorAngles(delta, angles);
}

inline void PlayerViewPoint(const vec3_t &angle, const vec3_t &headPos, vec3_t &endPos)
{
    vec3_t forward, right, up;
    AngleVectors(angle, forward, right, up);

    VectorCopy(headPos, endPos);

    endPos[0] += (forward[0] * 100.f);
    endPos[1] += (forward[1] * 100.f);
    endPos[2] += (forward[2] * 100.f);
}

inline bool WorldToScreen(const vec3_t &world, vec2_t &vecOut)
{
    vec3_t pos{};
    VectorSubtract(world, refdef->ViewOrigin, pos);

    vec3_t transform = {};
    transform[0] = DotProduct(pos, refdef->ViewAxis[1]);
    transform[1] = DotProduct(pos, refdef->ViewAxis[2]);
    transform[2] = DotProduct(pos, refdef->ViewAxis[0]);

    if (transform[2] < 0.01f)
    {
        return false;
    }

    vecOut[0] = static_cast<float>(refdef->width / 2) * (1.f - (transform[0] / refdef->fov[0] / transform[2]));
    vecOut[1] = static_cast<float>(refdef->height / 2) * (1.f - (transform[1] / refdef->fov[1] / transform[2]));

    return true;
}

inline bool WorldToScreen(const vec3_t &world, vec3_t &vecOut)
{
    vec2_t tmpVec{};
    bool result = WorldToScreen(world, tmpVec);
    if (result)
    {
        vecOut[0] = tmpVec[0];
        vecOut[1] = tmpVec[1];
        vecOut[2] = 0.f;
    }
    return result;
}
} // namespace Math