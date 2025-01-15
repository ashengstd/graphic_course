#include <cmath>
#include <gl/freeglut.h>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
  float x, y;
  Point(float x = 0, float y = 0) : x(x), y(y) {}
};

// 控制点
vector<Point> controlPoints = {{100, 100}, {150, 300}, {200, 400}, {300, 350},
                               {400, 300}, {450, 200}, {500, 100}};

// 计算阶乘
int factorial(int n) {
  if (n <= 1)
    return 1;
  return n * factorial(n - 1);
}

// 计算组合数 C(n, i)
int combination(int n, int i) {
  return factorial(n) / (factorial(i) * factorial(n - i));
}

// 输出 Bezier 曲线函数表达式
void printBezierExpression(const vector<Point> &points) {
  int n = points.size() - 1;
  cout << "Bezier Curve Function (n = " << n << "):\n";

  // 输出 x(t) 和 y(t)
  cout << "x(t) = ";
  for (int i = 0; i <= n; i++) {
    float coef = combination(n, i);
    if (i > 0)
      cout << " + ";
    cout << coef << " * (" << points[i].x << ") * (1-t)^" << (n - i) << " * t^"
         << i;
  }
  cout << "\n";

  cout << "y(t) = ";
  for (int i = 0; i <= n; i++) {
    float coef = combination(n, i);
    if (i > 0)
      cout << " + ";
    cout << coef << " * (" << points[i].y << ") * (1-t)^" << (n - i) << " * t^"
         << i;
  }
  cout << "\n";
}

// 计算 Bezier 曲线点
Point bezierPoint(const vector<Point> &points, float t) {
  int n = points.size() - 1;
  Point p(0, 0);
  for (int i = 0; i <= n; i++) {
    float coef = combination(n, i) * pow(1 - t, n - i) * pow(t, i);
    p.x += coef * points[i].x;
    p.y += coef * points[i].y;
  }
  return p;
}

// 绘制 Bezier 曲线
void drawBezier(const vector<Point> &points, int steps) {
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= steps; i++) {
    float t = i / (float)steps;
    Point p = bezierPoint(points, t);
    glVertex2f(p.x, p.y);
  }
  glEnd();
}

// 绘制控制点
void drawControlPoints(const vector<Point> &points) {
  glPointSize(5.0f);
  glBegin(GL_POINTS);
  for (const auto &p : points) {
    glVertex2f(p.x, p.y);
  }
  glEnd();
}

// 显示函数
void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0f, 1.0f, 0.0f); // 控制点颜色
  drawControlPoints(controlPoints);

  // 绘制6次 Bezier 曲线
  glColor3f(0.0f, 0.0f, 1.0f); // 曲线颜色
  drawBezier(controlPoints, 100);

  // 绘制分段3次 Bezier 曲线
  vector<Point> firstSegment(controlPoints.begin(), controlPoints.begin() + 4);
  vector<Point> secondSegment(controlPoints.begin() + 3, controlPoints.end());

  glColor3f(1.0f, 0.0f, 0.0f); // 第一段颜色
  drawBezier(firstSegment, 100);

  glColor3f(1.0f, 1.0f, 0.0f); // 第二段颜色
  drawBezier(secondSegment, 100);

  glFlush();
}

// 初始化GLUT
void myInit() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 600.0, 0.0, 600.0);
}

// 主函数
int main(int argc, char **argv) {
  // 输出 Bezier 曲线表达式
  cout << "6th Degree Bezier Curve:\n";
  printBezierExpression(controlPoints);

  cout << "\nSegmented 3rd Degree Bezier Curves:\n";
  vector<Point> firstSegment(controlPoints.begin(), controlPoints.begin() + 4);
  vector<Point> secondSegment(controlPoints.begin() + 3, controlPoints.end());

  cout << "First Segment:\n";
  printBezierExpression(firstSegment);

  cout << "\nSecond Segment:\n";
  printBezierExpression(secondSegment);

  // 初始化 GLUT 绘制
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Bezier Curve");
  myInit();
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}
