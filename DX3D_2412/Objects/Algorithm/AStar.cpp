#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
    heap = new Heap();
}

AStar::~AStar()
{
    for (Node* node : nodes)
        delete node;

    delete heap;
}

void AStar::Update()
{
    if (KEY->Down(VK_RBUTTON))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);

        for (Node* node : nodes)
        {
            if (node->IsRayCollision(ray, nullptr))
            {
                node->SetState(Node::OBSTACLE);
                break;
            }
        }
    }
}

void AStar::Render()
{
    for (Node* node : nodes)
        node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
    Vector2 size = terrain->GetSize();

    interval.x = size.x / width;
    interval.y = size.y / height;

    nodes.reserve((width + 1) * (height + 1));
    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
            pos.y = terrain->GetHeight(pos);

            nodes.push_back(new Node(pos, nodes.size()));            
            nodes.back()->UpdateWorld();

            if (pos.y > 10.0f)
            {
                nodes.back()->SetState(Node::OBSTACLE);
                //AddObstacle(nodes.back());
            }
        }
    }

    SetEdge();
}

int AStar::FindCloseNode(Vector3 pos)
{
    float minDist = FLT_MAX;

    int index = -1;

    for (UINT i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->state == Node::OBSTACLE)
            continue;

        float distance = Vector3::Distance(pos, nodes[i]->Center());

        if (minDist > distance)
        {
            minDist = distance;
            index = i;
        }
    }

    return index;
}

void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path)
{
    Reset();
    path.clear();

    //1. 시작노드 초기화하고 오픈노드에 추가
    float G = 0;
    float H = GetDiagonalManhattanDistance(start, end);

    nodes[start]->f = G + H;
    nodes[start]->g = G;
    nodes[start]->h = H;
    nodes[start]->via = start;
    nodes[start]->state = Node::OPEN;

    //openNodes.push_back(start);
    heap->Insert(nodes[start]);

    while (nodes[end]->state != Node::CLOSED)
    {
        //if (openNodes.empty())
        if (heap->Empty())
            return;

        //2. 오픈노드 중에서 효율이 가장 좋은 노드 찾기
        int curIndex = GetMinNode();
        //3. 찾은 노드와 연결된 노드의 정보 갱신후 오픈노드에 추가하고
        //확장이 끝난 노드는 닫기
        Extend(curIndex, end);
        nodes[curIndex]->state = Node::CLOSED;
    }

    //5. BackTracking
    int curIndex = end;

    while (curIndex != start)
    {
        nodes[curIndex]->state = Node::USING;
        path.push_back(nodes[curIndex]->Center());
        curIndex = nodes[curIndex]->via;
    }
}

void AStar::Reset()
{
    for (Node* node : nodes)
    {
        if (node->state != Node::OBSTACLE)
            node->state = Node::NONE;
    }

    //openNodes.clear();
    heap->Clear();
}

float AStar::GetManhattanDistance(int start, int end)
{
    Vector3 startPos = nodes[start]->Center();
    Vector3 endPos = nodes[end]->Center();

    Vector3 temp = endPos - startPos;

    return abs(temp.x) + abs(temp.z);
}

float AStar::GetDiagonalManhattanDistance(int start, int end)
{
    Vector3 startPos = nodes[start]->Center();
    Vector3 endPos = nodes[end]->Center();

    Vector3 temp = endPos - startPos;

    float x = abs(temp.x);
    float z = abs(temp.z);

    float maxSize = max(x, z);
    float minSize = min(x, z);

    return (maxSize - minSize) + sqrt(minSize * minSize * 2);
}

void AStar::Extend(int center, int end)
{
    for (Node::Edge* edge : nodes[center]->edges)
    {
        int index = edge->index;

        if (nodes[index]->state == Node::CLOSED)
            continue;
        if (nodes[index]->state == Node::OBSTACLE)
            continue;

        float G = nodes[center]->g + edge->cost;
        float H = GetDiagonalManhattanDistance(index, end);
        float F = G + H;

        if (nodes[index]->state == Node::OPEN)
        {
            if (F < nodes[index]->f)
            {
                nodes[index]->g = G;
                nodes[index]->f = F;
                nodes[index]->via = center;
            }
        }
        else if (nodes[index]->state == Node::NONE)
        {
            nodes[index]->g = G;
            nodes[index]->h = H;
            nodes[index]->f = F;
            nodes[index]->via = center;
            nodes[index]->state = Node::OPEN;

            //openNodes.push_back(index);
            heap->Insert(nodes[index]);
        }
    }
}

int AStar::GetMinNode()
{
    //float minCost = FLT_MAX;
    //int index = -1;
    //
    //for (UINT i = 0; i < openNodes.size(); i++)
    //{
    //    float cost = nodes[openNodes[i]]->f;
    //
    //    if (cost < minCost)
    //    {
    //        minCost = cost;
    //        index = i;
    //    }
    //}
    //int result = openNodes[index];
    //openNodes.erase(openNodes.begin() + index);

    return heap->DeleteRoot()->index;    
}

void AStar::SetEdge()
{
    UINT width = this->width + 1;

    for (UINT i = 0; i < nodes.size(); i++)
    {
        if (i % width != width - 1)
        {
            nodes[i]->AddEdge(nodes[i + 1]);
            nodes[i + 1]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i + width]);
            nodes[i + width]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - width && i % width != width - 1)
        {
            nodes[i]->AddEdge(nodes[i + width + 1]);
            nodes[i + width + 1]->AddEdge(nodes[i]);
        }
        
        if (i < nodes.size() - width && i % width != 0)
        {
            nodes[i]->AddEdge(nodes[i + width - 1]);
            nodes[i + width - 1]->AddEdge(nodes[i]);
        }
    }
}
