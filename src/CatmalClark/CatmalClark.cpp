#define GL_SILENCE_DEPRECATION
#define _CRT_SECURE_NO_WARNINGS

#include <GL/freeglut.h>

#include <assert.h>
#include <map>
#include <set>
#include <vector>

using namespace std;
#define PI 3.1415926536
const int maxn = 500;
const int RGB = 255;

// 顶点结构
typedef struct Vertex {
  float x, y, z;
} Vertex;

// 面结构
typedef struct Face {
  int num;
  int order[4];
} Face;

// 半边结构
typedef struct HalfEdge {
  int origin;
  struct HalfEdge *next;     // 指向的同面下一边
  struct HalfEdge *opposite; // 相邻异面对边
  int IncFace;               // 所在面编号
} HalfEdge;

// 每次的初始数据
vector<Vertex> vertex;
vector<Face> face;
vector<HalfEdge *> edge;
int e_num = 0;
int n_node = 0;
int n_face = 0;
int n_edge = 0;
int width = 800;
int height = 800;

void init_data() {
  // 初始化正方体数据
  // 点数据
  n_node = 8;
  n_edge = 12;
  n_face = 6;
  Vertex v;
  v.x = -1;
  v.y = -1;
  v.z = -1;
  vertex.push_back(v);
  v.x = 1;
  v.y = -1;
  v.z = -1;
  vertex.push_back(v);
  v.x = 1;
  v.y = 1;
  v.z = -1;
  vertex.push_back(v);
  v.x = -1;
  v.y = 1;
  v.z = -1;
  vertex.push_back(v);
  v.x = -1;
  v.y = 1;
  v.z = 1;
  vertex.push_back(v);
  v.x = -1;
  v.y = -1;
  v.z = 1;
  vertex.push_back(v);
  v.x = 1;
  v.y = -1;
  v.z = 1;
  vertex.push_back(v);
  v.x = 1;
  v.y = 1;
  v.z = 1;
  vertex.push_back(v);
  // 面数据
  Face f;
  f.num = 4;
  f.order[0] = 0;
  f.order[1] = 1;
  f.order[2] = 2;
  f.order[3] = 3;
  face.push_back(f);
  f.num = 4;
  f.order[0] = 2;
  f.order[1] = 7;
  f.order[2] = 4;
  f.order[3] = 3;
  face.push_back(f);
  f.num = 4;
  f.order[0] = 5;
  f.order[1] = 4;
  f.order[2] = 7;
  f.order[3] = 6;
  face.push_back(f);
  f.num = 4;
  f.order[0] = 0;
  f.order[1] = 5;
  f.order[2] = 6;
  f.order[3] = 1;
  face.push_back(f);
  f.num = 4;
  f.order[0] = 1;
  f.order[1] = 6;
  f.order[2] = 7;
  f.order[3] = 2;
  face.push_back(f);
  f.num = 4;
  f.order[0] = 0;
  f.order[1] = 3;
  f.order[2] = 4;
  f.order[3] = 5;
  face.push_back(f);
}

// 半边数据生成存入edge函数
void initEdge() {
  // 初始化记录矩阵map，记录顶点i->j半边的存在性以及对应edge[]中的序号
  int map[maxn][maxn] = {0};
  for (int i = 0; i < n_node; i++) {
    for (int j = 0; j < n_node; j++) {
      map[i][j] = -1;
    }
  }
  int e_num = 0;
  // 读取每个面的数据，初始化此面上的四条半边
  for (int i = 0; i < n_face; i++) {
    HalfEdge *edge1 = new HalfEdge();
    HalfEdge *edge2 = new HalfEdge();
    HalfEdge *edge3 = new HalfEdge();
    HalfEdge *edge4 = new HalfEdge();
    // 四条半边的origin分别为face的四个顶点
    edge1->origin = face[i].order[0];
    edge2->origin = face[i].order[1];
    edge3->origin = face[i].order[2];
    edge4->origin = face[i].order[3];
    // 按顺序彼此相连
    edge1->next = edge2;
    edge2->next = edge3;
    edge3->next = edge4;
    edge4->next = edge1;
    // 生成对边
    HalfEdge *tmpe = new HalfEdge();
    // 如果0->1的对边1->0已创建,则互相初始化为对边
    if (map[face[i].order[1]][face[i].order[0]] != -1) {
      tmpe = edge[map[face[i].order[1]][face[i].order[0]]];
      edge1->opposite = tmpe;
      tmpe->opposite = edge1;
      printf("OPT-1: %d->%d\n", face[i].order[0], face[i].order[1]);
    }
    // 如果不存在，opposite先设为空，将edge1更新到map中（对应存储此边在edge中编号）
    else {
      edge1->opposite = NULL;
      map[face[i].order[0]][face[i].order[1]] = e_num;
      printf("OPT-2: %d->%d\n", face[i].order[0], face[i].order[1]);
    }
    e_num++;
    if (map[face[i].order[2]][face[i].order[1]] != -1) {
      tmpe = edge[map[face[i].order[2]][face[i].order[1]]];
      edge2->opposite = tmpe;
      tmpe->opposite = edge2;
      printf("OPT-1: %d->%d\n", face[i].order[1], face[i].order[2]);
    } else {
      edge2->opposite = NULL;
      map[face[i].order[1]][face[i].order[2]] = e_num;
      printf("OPT-2: %d->%d\n", face[i].order[1], face[i].order[2]);
    }
    e_num++;
    if (map[face[i].order[3]][face[i].order[2]] != -1) {
      tmpe = edge[map[face[i].order[3]][face[i].order[2]]];
      edge3->opposite = tmpe;
      tmpe->opposite = edge3;
      printf("OPT-1: %d->%d\n", face[i].order[2], face[i].order[3]);
    } else {
      edge3->opposite = NULL;
      map[face[i].order[2]][face[i].order[3]] = e_num;
      printf("OPT-2: %d->%d\n", face[i].order[2], face[i].order[3]);
    }
    e_num++;
    if (map[face[i].order[0]][face[i].order[3]] != -1) {
      tmpe = edge[map[face[i].order[0]][face[i].order[3]]];
      edge4->opposite = tmpe;
      tmpe->opposite = edge4;
      printf("OPT-1: %d->%d\n", face[i].order[3], face[i].order[0]);
    } else {
      edge4->opposite = NULL;
      map[face[i].order[3]][face[i].order[0]] = e_num;
      printf("OPT-2: %d->%d\n", face[i].order[3], face[i].order[0]);
    }
    e_num++;
    // 每条边的面数据记录为i
    edge1->IncFace = i;
    edge2->IncFace = i;
    edge3->IncFace = i;
    edge4->IncFace = i;
    // 每条边都存入edge
    edge.push_back(edge1);
    edge.push_back(edge2);
    edge.push_back(edge3);
    edge.push_back(edge4);
  }
  // 及时更新n_edge
  n_edge = edge.size();
  printf("First-Done!\n");
}

// 找到从该定点出发的第一条半边
HalfEdge *locate(int v) {
  for (int k = 0; k < n_edge; k++) {
    if (edge[k]->origin == v)
      return edge[k];
  }
  return NULL;
}
// 找到某个面上从顶点处出发的半边
HalfEdge *locate(int v, int fn) {
  for (int k = 0; k < n_edge; k++) {
    if (edge[k]->origin == v && edge[k]->IncFace == fn)
      return edge[k];
  }
  return NULL;
}

void display() {
  glClearColor((float)255 / RGB, (float)255 / RGB, (float)255 / RGB, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓存
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -5.0f);

  gluLookAt(0.01, 0.01, 0.02, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  // 设置观察的位置和方向

  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_LINES);
  glVertex3f(100, 0, 0);
  glVertex3f(-100, 0, 0);
  glVertex3f(0, 0, 100);
  glVertex3f(0, 0, -100);
  glVertex3f(0, 100, 0);
  glVertex3f(0, -100, 0);
  glEnd();

  glBegin(GL_QUADS);
  for (int i = 0; i < n_face; i++) {
    // int i = 1;
    glVertex3f(vertex[face[i].order[0]].x, vertex[face[i].order[0]].y,
               vertex[face[i].order[0]].z);
    glVertex3f(vertex[face[i].order[1]].x, vertex[face[i].order[1]].y,
               vertex[face[i].order[1]].z);
    glVertex3f(vertex[face[i].order[2]].x, vertex[face[i].order[2]].y,
               vertex[face[i].order[2]].z);
    glVertex3f(vertex[face[i].order[3]].x, vertex[face[i].order[3]].y,
               vertex[face[i].order[3]].z);
  }
  glEnd();

  glPopMatrix();
  glutSwapBuffers();
}
// 细分算法
// void subdivide() {}
// Face point calculation for a face
Vertex calculateFacePoint(int faceIndex) {
  Vertex facePoint = {0.0f, 0.0f, 0.0f};
  for (int i = 0; i < face[faceIndex].num; i++) {
    facePoint.x += vertex[face[faceIndex].order[i]].x;
    facePoint.y += vertex[face[faceIndex].order[i]].y;
    facePoint.z += vertex[face[faceIndex].order[i]].z;
  }
  facePoint.x /= face[faceIndex].num;
  facePoint.y /= face[faceIndex].num;
  facePoint.z /= face[faceIndex].num;
  return facePoint;
}

// Edge point calculation for a half-edge
Vertex calculateEdgePoint(HalfEdge *he) {
  Vertex edgePoint = {0.0f, 0.0f, 0.0f};
  // Get the two vertices of the edge
  Vertex v1 = vertex[he->origin];
  Vertex v2 = vertex[he->next->origin];
  // Get the two face points
  Vertex f1 = calculateFacePoint(he->IncFace);
  Vertex f2 = calculateFacePoint(he->opposite->IncFace);

  edgePoint.x = (v1.x + v2.x + f1.x + f2.x) / 4.0f;
  edgePoint.y = (v1.y + v2.y + f1.y + f2.y) / 4.0f;
  edgePoint.z = (v1.z + v2.z + f1.z + f2.z) / 4.0f;
  return edgePoint;
}

// Vertex point calculation for an original vertex
// 计算更新后的顶点位置
Vertex calculateVertexPoint(int vertexIndex) {
  Vertex vertexPoint = {0.0f, 0.0f, 0.0f};
  HalfEdge *start = locate(vertexIndex);
  if (!start)
    return vertex[vertexIndex];

  HalfEdge *current = start;

  // n = 与该顶点相邻的边数(即度数)
  int n = 0;

  // Q = 相邻面的中心点的平均
  Vertex Q = {0.0f, 0.0f, 0.0f};

  // R = 相邻边的中点的平均
  Vertex R = {0.0f, 0.0f, 0.0f};

  // P = 原始顶点位置
  Vertex P = vertex[vertexIndex];

  do {
    // 累加相邻面的中心点
    Vertex facePoint = calculateFacePoint(current->IncFace);
    Q.x += facePoint.x;
    Q.y += facePoint.y;
    Q.z += facePoint.z;

    // 累加相邻边的中点
    int nextVertex = current->next->origin;
    R.x += (vertex[vertexIndex].x + vertex[nextVertex].x) * 0.5f;
    R.y += (vertex[vertexIndex].y + vertex[nextVertex].y) * 0.5f;
    R.z += (vertex[vertexIndex].z + vertex[nextVertex].z) * 0.5f;

    n++;

    // 移动到下一条边
    current = current->opposite;
    if (!current)
      break;
    current = current->next;
  } while (current != start && current != nullptr);

  if (n < 3)
    return vertex[vertexIndex]; // 处理特殊情况

  // 计算平均值
  Q.x /= n;
  Q.y /= n;
  Q.z /= n;

  R.x /= n;
  R.y /= n;
  R.z /= n;

  // Catmull-Clark顶点更新公式:
  // F = (Q + 2R + (n-3)P) / n
  float n_minus_3 = n - 3;
  vertexPoint.x = (Q.x + 2.0f * R.x + n_minus_3 * P.x) / n;
  vertexPoint.y = (Q.y + 2.0f * R.y + n_minus_3 * P.y) / n;
  vertexPoint.z = (Q.z + 2.0f * R.z + n_minus_3 * P.z) / n;

  return vertexPoint;
}

void subdivide() {
  // 临时存储所有新生成的数据
  vector<Vertex> newVertices = vertex; // 保持原始顶点
  vector<Face> newFaces;

  // 用于存储和查找面点
  vector<int> facePoints(n_face);

  // 用于存储和查找边点
  // key: 用ordered pair确保边的唯一性(较小顶点索引,较大顶点索引)
  // value: 新顶点的索引
  map<pair<int, int>, int> edgePointIndices;

  // 1. 计算所有面点并存储
  for (int i = 0; i < n_face; i++) {
    Vertex facePoint = calculateFacePoint(i);
    newVertices.push_back(facePoint);
    facePoints[i] = newVertices.size() - 1;
  }

  // 2. 计算所有边点并存储
  // 为确保每条边只处理一次，先收集所有唯一边
  set<pair<int, int>> processedEdges;
  for (int i = 0; i < n_face; i++) {
    for (int j = 0; j < face[i].num; j++) {
      int v1 = face[i].order[j];
      int v2 = face[i].order[(j + 1) % face[i].num];

      // 确保较小的顶点索引在前
      pair<int, int> edgeKey = {min(v1, v2), max(v1, v2)};

      // 如果这条边没处理过
      if (processedEdges.find(edgeKey) == processedEdges.end()) {
        processedEdges.insert(edgeKey);

        // 找到这条边对应的半边
        HalfEdge *he = locate(v1, i);
        while (he->next->origin != v2) {
          he = he->next;
        }

        // 计算边点
        Vertex edgePoint = calculateEdgePoint(he);
        newVertices.push_back(edgePoint);
        edgePointIndices[edgeKey] = newVertices.size() - 1;
      }
    }
  }

  // 3. 计算更新后的原始顶点位置
  vector<Vertex> updatedVertices(n_node);
  for (int i = 0; i < n_node; i++) {
    updatedVertices[i] = calculateVertexPoint(i);
  }

  // 更新原始顶点
  for (int i = 0; i < n_node; i++) {
    newVertices[i] = updatedVertices[i];
  }

  // 4. 为每个原始面生成新的四边形面片
  for (int faceIdx = 0; faceIdx < n_face; faceIdx++) {
    Face &oldFace = face[faceIdx];
    int fpIdx = facePoints[faceIdx]; // 当前面的面点索引

    // 为面的每个顶点创建一个新的四边形
    for (int i = 0; i < oldFace.num; i++) {
      Face newFace;
      newFace.num = 4;

      // 当前顶点
      int v0 = oldFace.order[i];
      // 下一个顶点
      int v1 = oldFace.order[(i + 1) % oldFace.num];

      // 获取当前边的边点索引
      pair<int, int> currentEdge = {min(v0, v1), max(v0, v1)};
      int ep1 = edgePointIndices[currentEdge];

      // 获取上一条边的边点索引
      int prev = (i - 1 + oldFace.num) % oldFace.num;
      pair<int, int> prevEdge = {min(oldFace.order[prev], v0),
                                 max(oldFace.order[prev], v0)};
      int ep2 = edgePointIndices[prevEdge];

      // 按照逆时针顺序设置新面片的顶点
      newFace.order[0] = v0;    // 原始顶点
      newFace.order[1] = ep1;   // 当前边的边点
      newFace.order[2] = fpIdx; // 面点
      newFace.order[3] = ep2;   // 前一条边的边点

      newFaces.push_back(newFace);
    }
  }

  // 5. 更新全局数据
  vertex = newVertices;
  face = newFaces;
  n_node = vertex.size();
  n_face = face.size();

  // 6. 重建半边结构
  edge.clear();
  initEdge();
}

// 交互页面函数
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case '1':
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  case '2':
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  case 'd':
    subdivide();
    break;
  }
  glutPostRedisplay();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
  init_data();
  initEdge();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("catmual-clark subdivision");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
