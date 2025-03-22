#pragma once

class TerrainEditor : public GameObject<VertexUVNormalAlpha>
{
private:
    typedef VertexUVNormalAlpha VertexType;

    const UINT MAX_SIZE = 256;
    const UINT MAX_HEIGHT = 20.0f;

    enum EditType
    {
        HEIGHT, ALPHA
    };

    struct InputDesc
    {
        Float3 v0, v1, v2;
    };

    struct OutputDesc
    {
        int picked;
        float distance;
    };

public:
    TerrainEditor();
    ~TerrainEditor();

    void Update();
    void Render();
    void GUIRender();

    void Picking();
    bool ComputePicking(Vector3& pos);

private:
    void MakeMesh();    
    void MakeComputeData();

    void AdjustHeight();
    void AdjustAlpha();

    void SaveHeightMap();
    void LoadHeightMap();

    void SaveAlphaMap();
    void LoadAlphaMap();

    void Resize();
    void UpdateHeight();

private:
    UINT width, height, triangleSize;
    Vector3 pickingPos;
    float adjustValue = 20.0f;
    string projectPath;
    EditType editType = HEIGHT;
    int selectMap = 0;

    Texture* heightMap;
    Texture* secondMap;
    Texture* thirdMap;

    BrushBuffer* brushBuffer;

    //Compute
    RayBuffer* rayBuffer;
    StructuredBuffer* structuredBuffer;

    vector<InputDesc> inputs;
    vector<OutputDesc> outputs;  

    ComputeShader* computeShader;
};