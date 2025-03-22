#include "Framework.h"

unordered_map<wstring, Shader*> Shader::shaders;

VertexShader* Shader::AddVS(wstring file)
{    
    wstring key = file + L"VS";

    if (shaders.count(key) > 0)
        return (VertexShader*)shaders[key];

    wstring path = L"Resources/Shaders/" + file;

    shaders[key] = new VertexShader(path);
    shaders[key]->file = file;

    return (VertexShader*)shaders[key];
}

PixelShader* Shader::AddPS(wstring file)
{
    wstring key = file + L"PS";

    if (shaders.count(key) > 0)
        return (PixelShader*)shaders[key];

    wstring path = L"Resources/Shaders/" + file;

    shaders[key] = new PixelShader(path);
    shaders[key]->file = file;

    return (PixelShader*)shaders[key];
}

ComputeShader* Shader::AddCS(wstring file)
{
    wstring key = file + L"CS";

    if (shaders.count(key) > 0)
        return (ComputeShader*)shaders[key];

    wstring path = L"Resources/Shaders/" + file;

    shaders[key] = new ComputeShader(path);
    shaders[key]->file = file;

    return (ComputeShader*)shaders[key];
}

GeometryShader* Shader::AddGS(wstring file)
{
    wstring key = file + L"GS";

    if (shaders.count(key) > 0)
        return (GeometryShader*)shaders[key];

    wstring path = L"Resources/Shaders/" + file;

    shaders[key] = new GeometryShader(path);
    shaders[key]->file = file;

    return (GeometryShader*)shaders[key];
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : shaders)
        delete shader.second;
}
