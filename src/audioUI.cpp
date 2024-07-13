// Joel A. Jaffe 2024-07-09
// User Interface App for Audio Applications

/*
TODO:
- Add docblocks
- Add oscilloscope class
*/

#include "../include/graphicsTemplate.hpp"

// functions
Vec2f stripZ(Vec3f coords) {
  return Vec2f(coords[0], coords[1]);
}

void snapTo(Mesh &m, Vec2f xy) {
  Vec3f center = m.getCenter();
  m.translate(-center[0], -center[1]);
  m.translate(xy[0], xy[1]);
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
    this->content.copy(m);
    m.unitize();
    m.scale(std::min(width, height) / 2 - padding);
    m.translate(center[0], center[1]);
  }

  void translate(Vec2f xy) {
    this->frame.translate(xy[0], xy[1]);
    this->content.translate(xy[0], xy[1]);
  }

  void snap(Vec2f xy) {
    snapTo(this->frame, xy);
    snapTo(this->content, xy);
  }

  void scale(float s) {
    this->frame.scale(s);
    this->content.scale(s);
  }

  void unit() {
    this->frame.unitize();
    this->content.unitize();
  }

  void draw(Graphics &g) {
    g.draw(frame);
    g.draw(content);
  }

  void copy(Element &e) {
    e.frame.copy(e.frame);
    e.frame.copy(e.frame);
  }

  inline virtual void query(Vec2f xy) {
    if (this->inside(xy, this->frame)) {
        this->frame.primitive(Mesh::TRIANGLE_FAN);
      } else {this->frame.primitive(Mesh::LINE_LOOP);}
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

struct Container {
  // member variables
  Vec2f center = Vec2f(0, 0.85);
  float width = 2.f, height = 0.25f, padding = 0.05f;
  std::vector<Element> elements;

  void addElement(Element& e) {
    //Element elem; 
    //elem.seed();
    //elem.copy(e);
    elements.push_back(e); // append element
    float eWidth = width/elements.size();
    for (int i = 0; i < elements.size(); i++) {
      Vec2f target = center + Vec2f(-width/2 + eWidth*(i+0.5), 0);
      elements[i].unit();
      elements[i].scale(eWidth/width);
      elements[i].snap(target);
    }
  } 

  void draw(Graphics &g){
    for (int i = 0; i < elements.size(); i++) {
      elements[i].draw(g);
    }
  }

  void query(Vec2f xy) {
    for (int i = 0; i < elements.size(); i++) {
      elements[i].query(xy);
    }
  }

};

struct MenuBar {
  // member variables
  Vec2f anchor = Vec2f(0, 1);
  float width = 2.f, height = 0.25f, padding = 0.05f;
  std::vector<Mesh> containers;

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
  //MenuBar menu;
  Scope scope;
  Element body;
  Container test;
  
  void onCreate() override {
    //menu.seed(6);
    scope.seed();
    body.center = Vec2f(0, 0);
    body.height = 1.65;
    body.width = 1.95;
    body.seed();
    body.addContent(scope.mesh);

    for (int i = 0; i < 6; i++) {
      test.addElement(body);
    }
    //test.seed(); 
  }

  void onAnimate(double dt) override {}

  bool onMouseMove(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    // menu.query(pos);
    test.query(pos);
    return true;
  } 

  void onDraw(Graphics& g) override {
    graphicsTemplate<T>::onDraw(g); // call base class onDraw() 
    //menu.draw(g);
    //scope.draw(g);
    body.draw(g);
    //test.draw(g);
  }
};

int main() {
  audioUI<float> app;
  app.start();
  return 0;
}
