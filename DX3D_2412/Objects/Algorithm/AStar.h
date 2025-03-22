#pragma once

class AStar
{
public:
    AStar(UINT width = 20, UINT height = 20);
    ~AStar();

    void Update();
    void Render();

    void SetNode(class Terrain* terrain);

    int FindCloseNode(Vector3 pos);    

    void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);    

private:
    void Reset();

    float GetManhattanDistance(int start, int end);
    float GetDiagonalManhattanDistance(int start, int end);

    void Extend(int center, int end);
    int GetMinNode();

    void SetEdge();

private:
    UINT width, height;
    Vector2 interval;

    vector<Node*> nodes;
    //vector<int> openNodes;
    Heap* heap;
    //vector<Collider*> obstacles;
};