#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
    MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
    {
        matrix = XMMatrixIdentity();
    }

    void Set(Matrix value)
    {
        matrix = XMMatrixTranspose(value);
    }

private:
    Matrix matrix;
};

class ViewBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix view;
        Matrix invView;
    };

public:
    ViewBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.view = XMMatrixIdentity();
        data.invView = XMMatrixIdentity();
    }

    void Set(Matrix view, Matrix invView)
    {
        data.view = XMMatrixTranspose(view);
        data.invView = XMMatrixTranspose(invView);
    }

private:
    Data data;
};

class MaterialBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float4 diffuse = { 1, 1, 1, 1 };        
        Float4 specular = { 1, 1, 1, 1 };
        Float4 ambient = { 1, 1, 1, 1 };
        Float4 emissive = { 0, 0, 0, 1 };

        float shininess = 24.0f;
        int hasNormalMap = 0;

        float padding[2];
    };


public:
    MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data* GetData() { return &data; }

private:
    Data data;
};

class FloatValueBuffer : public ConstBuffer
{
public:
    FloatValueBuffer() : ConstBuffer(values, sizeof(float) * 4)
    {
    }

    float* Get() { return values; }

private:
    float values[4] = {};
};

class LightBuffer : public ConstBuffer
{
public:
    struct Light
    {
        Float4 color = { 1, 1, 1, 1 };

        Float3 direction = { 0, -1, 1 };
        float attentionIntensity = 1.0f;

        Float3 position = { 0, 5, -5 };
        float range = 10.0f;

        float inner = 55.0f;
        float outer = 70.0f;
        int type = 0;
        int isActive = 1;
    };

    struct Data
    {
        Light lights[MAX_LIGHT];

        Float4 ambientColor = { 0.1f, 0.1f, 0.1f, 1.0f };        
        Float4 ambientCeil = { 0.1f, 0.1f, 0.1f, 1.0f };

        int lightCount = 1;
        float padding[3];
    };

public:
    LightBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data& Get() { return data; }

private:
    Data data;
};

class BrushBuffer : public ConstBuffer
{
private:
    struct Data
    {
        int type = 0;
        Float3 pickingPos = {};

        float range = 5.0f;
        Float3 color = { 0, 1, 0 };
    };

public:
    BrushBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data& Get() { return data; }

private:
    Data data;
};

class RayBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Float3 pos;
        UINT triangleSize;

        Float3 dir;
        float padding;
    };

public:
    RayBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data& Get() { return data; }

private:
    Data data;
};

class WaterBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Float4 color = { 1, 1, 1, 1 };

        float waveTime = 0.0f;
        float waveSpeed = 0.1f;
        float waveScale = 0.1f;
        float waveShininess = 24.0f;

        float fresnel = 0.5f;
        float padding[3];
    };
public:
    WaterBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data& Get() { return data; }

private:
    Data data;
};

class SpriteBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Float2 maxFrame;
        Float2 curFrame;
    };
public:
    SpriteBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data& Get() { return data; }

private:
    Data data;
};

class WeatherBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float3 velocity = { 0, -1, 0 };
        float distance = 100.0f;

        Float4 color = { 1, 1, 1, 1 };

        Float3 origin = {};
        float time = 0.0f;

        Float3 size = { 50, 50, 50 };
        float turbulence = 0.1f;
    };

public:
    WeatherBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    };

    Data* GetData() { return &data; }

private:
    Data data;
};