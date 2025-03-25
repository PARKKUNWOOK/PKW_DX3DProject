#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define MAX_LIGHT 10
#define MAX_BONE 256
#define MAX_FRAME 256
#define MAX_INSTANCE 128

#define CENTER Vector3(SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1, 0.0f)

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define DELTA Timer::Get()->GetElapsedTime()
#define KEY Keyboard::Get()
#define SCENE SceneManager::Get()

#define CAM Environment::Get()->GetMainCamera()
#define DIALOG ImGuiFileDialog::Instance()

#define FOR(n) for(int i = 0 ; i < n ; i++)

#include <windows.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <queue>
#include <list>
#include <functional>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::TriangleTests;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//DirectXTex
#include <DirectXTex/DirectXTex.h>

#pragma comment(lib, "DirectXTex.lib")

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>

#pragma comment(lib, "ImGui.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#pragma comment(lib, "assimp-vc143-mtd.lib")

//FMOD
#include <FMOD/fmod.hpp>

#pragma comment(lib, "fmod_vc.lib")

typedef XMMATRIX Matrix;
typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMVECTOR Vector4;
typedef XMFLOAT4X4 Float4x4;

typedef function<void()> Event;
typedef function<void(void*)> ParamEvent;
typedef function<void(int)> IntParamEvent;

//Framework Head
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Vector2.h"
#include "Framework/Math/GameMath.h"
#include "Framework/Math/Transform.h"

#include "Framework/Utility/Utility.h"
#include "Framework/Utility/Singleton.h"
#include "Framework/Utility/Timer.h"
#include "Framework/Utility/Keyboard.h"
#include "Framework/Utility/BinaryReader.h"
#include "Framework/Utility/BinaryWriter.h"
#include "Framework/Utility/Font.h"
#include "Framework/Utility/Observer.h"
#include "Framework/Utility/tinyxml2.h"
#include "Framework/Utility/Audio.h"

#include "Framework/Device/Device.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/GeometryShader.h"

#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/RawBuffer.h"
#include "Framework/Buffer/StructuredBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"
#include "Framework/Buffer/VertexLayouts.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"
#include "Framework/Render/DepthStencil.h"
#include "Framework/Render/RenderTarget.h"

#include "Framework/State/RasterizerState.h"
#include "Framework/State/SamplerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Environment.h"
#include "Framework/Environment/Reflection.h"
#include "Framework/Environment/Refraction.h"
#include "Framework/Environment/Shadow.h"

#include "Framework/Model/ModelData.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"

//GameObject Head
#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Pyramid.h"
#include "Objects/Basic/Cone.h"
#include "Objects/Basic/Plane.h"
#include "Objects/Basic/Quad.h"

#include "Objects/Collider/Collider.h"
#include "Objects/Collider/SphereCollider.h"
#include "Objects/Collider/BoxCollider.h"
#include "Objects/Collider/CapsuleCollider.h"
#include "Objects/Collider/RectCollider.h"

#include "Objects/Algorithm/Node.h"
#include "Objects/Algorithm/Heap.h"
#include "Objects/Algorithm/AStar.h"

#include "Objects/UI/Button.h"
#include "Objects/UI/ProgressBar.h"
#include "Objects/UI/UIManager.h"

#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/TerrainEditor.h"
#include "Objects/Landscape/Skybox.h"
#include "Objects/Landscape/Water.h"

#include "Objects/Manager/PoolingManager.h"

#include "Objects/Particle/Particle.h"
#include "Objects/Particle/Spark.h"
#include "Objects/Particle/Sprite.h"
#include "Objects/Particle/Rain.h"
#include "Objects/Particle/Snow.h"
#include "Objects/Particle/ParticleSystem.h"

#include "Objects/Test/TestModel.h"

#include "Objects/Character/Ghost.h"

#include "Objects/Shooting/Bullet.h"
#include "Objects/Shooting/BulletManager.h"

#include "Objects/HellfireDescent/Character/Player.h"
#include "Objects/HellfireDescent/Character/PlayerManager.h"
#include "Objects/HellfireDescent/Character/Enemy.h"
#include "Objects/HellfireDescent/Character/AssaultEnemy.h"
#include "Objects/HellfireDescent/Character/ThrowerEnemy.h"
#include "Objects/HellfireDescent/Character/EnemyManager.h"
#include "Objects/HellfireDescent/MapEditor/CubeMapEditor.h"
#include "Objects/HellfireDescent/MapEditor/Portal.h"
#include "Objects/HellfireDescent/MapEditor/MapManager.h"

//Manager Head
#include "Scenes/Scene.h"
#include "Managers/GameManager.h"
#include "Managers/SceneManager.h"

extern HWND hWnd;
extern Vector3 mousePos;