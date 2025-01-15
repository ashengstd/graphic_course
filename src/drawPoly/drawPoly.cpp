#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include <gl/gl.h>
#include <vector>
#include <windows.h>

using namespace std;

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_MODELVIEW 0x1700
#define GL_POINTS 0x0000
#define maxHt 500
#define maxWd 500
#define GL_LINES 0x0001

const int LINE_NUM = 5;
// 存储顶点的顺序：x1, y1, x2, y2
int vertices[4 * LINE_NUM] = {200, 100, 400, 120, 400, 120, 480, 300, 480, 300,
                              300, 400, 300, 400, 120, 300, 120, 300, 200, 100};

// 设置像素
void setPixel(int x, int y) {
  glPointSize(2.0f);
  glBegin(GL_POINTS);
  glVertex2i(x, y);
  glEnd();
  // glFlush();
}

// 绘制直线：使用DDA算法
void drawLine(int x1, int y1, int x2, int y2) {
  int dx = x2 - x1;
  int dy = y2 - y1;

  int steps = max(abs(dx), abs(dy));
  float xIncrement = dx / (float)steps;
  float yIncrement = dy / (float)steps;

  float x = x1;
  float y = y1;

  for (int i = 0; i <= steps; i++) {
    setPixel(round(x), round(y));
    x += xIncrement;
    y += yIncrement;
  }
}

// 边表结构
struct Edge {
  int yMax;
  float xMin;
  float slopeInverse;
};

// 扫描线填充算法
void ScanlineFill() {
  vector<Edge> edgeTable[maxHt];
  int numVertices = LINE_NUM;

  // 构建边表
  for (int i = 0; i < numVertices; i++) {
    int x1 = vertices[4 * i];
    int y1 = vertices[4 * i + 1];
    int x2 = vertices[4 * i + 2];
    int y2 = vertices[4 * i + 3];

    if (y1 == y2)
      continue; // 跳过水平边

    if (y1 > y2) {
      swap(x1, x2);
      swap(y1, y2);
    }

    Edge edge;
    edge.yMax = y2;
    edge.xMin = x1;
    edge.slopeInverse = (float)(x2 - x1) / (y2 - y1);
    edgeTable[y1].push_back(edge);
  }

  vector<Edge> activeEdgeTable;

  // 扫描线填充
  for (int y = 0; y < maxHt; y++) {
    // 更新AET
    for (auto it = activeEdgeTable.begin(); it != activeEdgeTable.end();) {
      if (it->yMax == y) {
        it = activeEdgeTable.erase(it);
      } else {
        it->xMin += it->slopeInverse;
        ++it;
      }
    }

    // 将新的边加入AET
    activeEdgeTable.insert(activeEdgeTable.end(), edgeTable[y].begin(),
                           edgeTable[y].end());

    // 按xMin排序
    sort(activeEdgeTable.begin(), activeEdgeTable.end(),
         [](Edge a, Edge b) { return a.xMin < b.xMin; });

    // 填充像素
    for (size_t i = 0; i < activeEdgeTable.size(); i += 2) {
      if (i + 1 >= activeEdgeTable.size())
        break;

      int xStart = ceil(activeEdgeTable[i].xMin);
      int xEnd = floor(activeEdgeTable[i + 1].xMin);

      // 跳过边框像素
      for (int x = xStart; x <= xEnd; x++) {
        // 边框检查：防止覆盖边框
        bool isOnBorder = (x == round(activeEdgeTable[i].xMin)) ||
                          (x == round(activeEdgeTable[i + 1].xMin));
        if (!isOnBorder) {
          setPixel(x, y);
        }
      }
    }
  }
}

// 初始化GLUT环境
void myInit(void) {
  glClearColor(0, 0, 0, 0); // 背景色
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, maxHt, 0, maxWd);
  glClear(GL_COLOR_BUFFER_BIT);
}

// 绘制多边形
void drawPoly() {
  int x1, y1, x2, y2;
  for (int i = 0; i < LINE_NUM; i++) {
    x1 = vertices[4 * i];
    y1 = vertices[4 * i + 1];
    x2 = vertices[4 * i + 2];
    y2 = vertices[4 * i + 3];
    drawLine(x1, y1, x2, y2);
  }
}

// 显示函数
void Draw(void) {
  glColor3f(0.2f, 0.7f, 0.6f); // 多边形边框颜色
  drawPoly();
  glColor3f(1.0f, 1.0f, 1.0f); // 填充颜色
  ScanlineFill();
  glFlush();
}

// 填充入口
void fill() {
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(maxHt, maxWd);
  glutInitWindowPosition(100, 150);
  glutCreateWindow("Polygon Scanline Fill");
  myInit();
  glutDisplayFunc(Draw);
  glutMainLoop();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  fill();
}
