// Joel A. Jaffe 2024-07-09
// User Interface App for Audio Applications

#include "../include/graphicsTemplate.hpp"

// functions
Vec2f stripZ(Vec3f coords) {
  return Vec2f(coords[0], coords[1]);
}

void getBounds2d(Mesh &m, Vec2f& min, Vec2f& max) {
  Vec3f minDummy, maxDummy;
  m.getBounds(minDummy, maxDummy);
  min = stripZ(minDummy);
  max = stripZ(maxDummy);
}

Vec2f mouseNormCords(float mouseX, float mouseY, float windowWidth, float windowHeight) {
  Vec2f coords = Vec2f(mouseX / windowWidth, mouseY / windowHeight);
  coords *= 2;
  coords[0] -= 1;
  coords[1] *= -1;
  coords[1] += 1;
  return coords;
}

void makeRect(Mesh &mesh, Vec2f leftCorn, float width, float height, Color color) {
  for (int i = 0; i < 4; i++) {mesh.color(color);} // color vertices
  mesh.vertex(leftCorn); // top left
  mesh.vertex(leftCorn + Vec2f(width, 0)); // top right
  mesh.vertex(leftCorn + Vec2f(width, -height)); // bottom right
  mesh.vertex(leftCorn + Vec2f(0, -height)); // bottom left
}

void makeFrame(Mesh &m, float w, float h, float cx, float cy, Color color = HSV(0, 0, 1)) {
  for (int i = 0; i < 4; i++) {m.color(color);} // color vertices
  float l = cx - w * 0.5;
  float r = cx + w * 0.5;
  float b = cy - h * 0.5;
  float t = cy + h * 0.5;
  m.vertex(l, b); // bottom left
  m.vertex(r, b); // bottom right
  m.vertex(r, t); // top right
  m.vertex(l, t); // top left
}

// classes 
struct Icon : public Mesh {
  // member variables

  // methods
};

struct Element {
  Vec2f center = Vec2f(0, 0);
  float width = 1.f, height = 1.f, padding = 0.1f;
  //float radius = std::min(width, height) / 2;
  Mesh frame;
  Mesh content;
  Color frameColor = HSV(0, 0, 1);
  Color contentColor = HSV(0, 0, 1);

  void seed() {
    makeFrame(frame, width, height, 
              center[0], center[1], frameColor);
    frame.primitive(Mesh::LINE_LOOP);
  }

  void addContent(Mesh& m) {
    // scale content to unit space, center and draw in frame
    m.unitize();
    m.scale(std::min(width, height) / 2 - padding);
    m.translate(center[0], center[1]);
  }

  void draw(Graphics &g) {
    g.draw(frame);
    g.draw(content);
  }

  bool inside(Vec2f coords, Mesh& m) {
    bool state = false; // outside by default
    int checks = 0;
    Vec2f min, max;
    getBounds2d(m, min, max); // find bounds
    for (int i = 0; i < 2; i++) { // loop over dims
      if (coords[i] > min[i] && coords[i] < max[i]) {
        checks++; // increment checks if in bounds
      }
    }
    if (checks == 2) {state = true;} // inside
    return state;
  }
};

struct Scope {
  // member variables
  Vec2f anchor = Vec2f(0, 0.7);
  float width = 2.f, height = 1.f, padding = 0.1f;
  float scale = (width - padding) * (height - padding) / (width * height);

  Mesh mesh;
  int sampleRate = 48000;

  // methods
  void seed() {
    float unit = 1.f / sampleRate;
    mesh.primitive(Mesh::LINE_STRIP); // primitive 
    for (int i = 0; i < sampleRate; i++) {
      mesh.vertex(-1 + unit * i * width, // x
                  std::sin(unit * i * M_2PI)); // y
      mesh.color(HSV(0,0,1)); // color
    }
    //mesh.scale((width - padding)/width, (height + std::abs(anchor[1]) - 2*padding) / (2));
    mesh.scale(0.7);
    mesh.translate(0, -0.15f);
    //mesh.translate(anchor[0], -0.15f);
  }

  void draw(Graphics &g){g.draw(mesh);}
};

struct MenuBar {
  // member variables
  Vec2f anchor = Vec2f(0, 1);
  float width = 2.f, height = 0.25f, padding = 0.05f;
  Vec2f bLeftCorn = Vec2f(anchor[0] - width/2, anchor[1] - height);
  Vec2f tRightCorn = Vec2f(anchor[0] + width/2, anchor[1]);
  std::vector<Mesh> containers;
  std::vector<Icon> icons;

  // methods
  void seed(int numElements) {
    float containerWidth = width/numElements;
    for (int i = 0; i < numElements; i++) {
      Mesh mesh;
      mesh.primitive(Mesh::LINE_LOOP);
      // draw a box for each 
      Vec2f center = anchor;
      center[0] = -width/2 + containerWidth*(i+0.5);
      center[1] -= height/2;
      makeFrame(mesh, // pass in a mesh
               containerWidth - padding, height - padding, 
               center[0], center[1], // set w,h
               HSV(i * 1.f/numElements, 1, 1)); // color
      containers.push_back(mesh); // append to list
    }
  }

  void draw(Graphics &g){
    for (int i = 0; i < containers.size(); i++) {
      g.draw(containers[i]);
    }
  }

  void setPrims(Mesh::Primitive prim) {
    for (int i = 0; i < containers.size(); i++) {
      containers[i].primitive(prim);
    }
  }

  void query(Vec2f coords) {
    for (int i = 0; i < containers.size(); i++) {
      if (this->inside(coords, containers[i])) {
        containers[i].primitive(Mesh::TRIANGLE_FAN);
      } else {containers[i].primitive(Mesh::LINE_LOOP);}
    }
  }

  bool inside(Vec2f coords, Mesh& m) {
    bool state = false; // outside by default
    int checks = 0;
    Vec2f min, max;
    getBounds2d(m, min, max); // find bounds
    for (int i = 0; i < 2; i++) { // loop over dims
      if (coords[i] > min[i] && coords[i] < max[i]) {
        checks++; // increment checks if in bounds
      }
    }
    if (checks == 2) {state = true;} // inside
    return state;
  }

};

// app class
template <typename T>
struct audioUI : graphicsTemplate<T> {
  MenuBar menu;
  Scope scope;
  Element test;
  
  void onCreate() override {
    menu.seed(6);
    scope.seed();
    menu.inside(Vec2f(0,0), menu.containers[0]);
    test.center = Vec2f(0, -0.1);
    test.height = 1.65;
    test.width = 1.95;
    test.seed();
    test.addContent(scope.mesh);
  }
  void onAnimate(double dt) override {}

  bool onMouseMove(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    menu.query(pos);
    return true;
  } 

  void onDraw(Graphics& g) override {
    graphicsTemplate<T>::onDraw(g); // call base class onDraw() 
    menu.draw(g);
    scope.draw(g);
    test.draw(g);
  }
};

int main() {
  audioUI<float> app;
  app.start();
  return 0;
}
