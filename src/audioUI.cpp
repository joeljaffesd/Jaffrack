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

/**
 * @brief Element class containing meshes for frame and content.
 * @param center orients the position of the element 
 * @param width scales element width
 * @param height scales element height
 * @param padding scales size of content relative to frame with its inverse
 * @param frameColor determines color of frame. Set same as background to hide frame
 * @param contentColor determines color of content
 */
class Element {
private:
  Vec2f center = Vec2f(0, 0);
  float width = 1.f, height = 1.f, padding = 0;
  Mesh frame, content;
  Color frameColor = HSV(0, 0, 1), contentColor = HSV(0, 0, 1);

public:
  // Element() = delete;
  Element(Vec2f center = Vec2f(0,0), float width = 1.f, float height = 1.f, 
          float padding = 0.25, Color frameColor = HSV(0, 0, 1), Color contentColor = HSV(0, 0, 1)) : 
            center(center), width(width), height(height), padding(padding), 
            frameColor(frameColor), contentColor(contentColor) {
              makeFrame(frame, width, height, center[0], center[1], frameColor);
              frame.primitive(Mesh::LINE_LOOP);
  }

  //Copy constructor
  Element(const Element& e) {
    this->center = e.center;
    this->width = e.width;
    this->height = e.height;
    this->padding = e.padding;
    this->frameColor = e.frameColor;
    this->contentColor = e.contentColor;
    this->frame.copy(e.frame);
    this->content.copy(e.content);
  }

  /**
   * @brief copies a mesh to content 
   * @param m mesh to be copied
   */
  void addContent(Mesh& m) {
    this->content.copy(m); // copies input mesh... could this be done better?
    content.unitize(); // unitize for scaling
    content.scale((std::sqrt(width*width + height*height) * (1 - padding)) / std::sqrt(8)); // scale
    content.translate(center[0], center[1]); // snap to center of frame
  }

  /**
   * @brief updates center and snaps meshes to it
   * @param m mesh to be copied
   */
  void snap(Vec2f xy) {
    center = xy;
    snapTo(frame, xy);
    snapTo(content, xy);
  }

  /**
   * @brief scales meshes by a factor
   * @param s scaling factor
   */
  void scale(float s) {
    width = width * s;
    height = height * s;
    this->frame.scale(s);
    this->content.scale(s);
  }

  /**
   * @brief unitizes meshes
   */
  void unit() {
    this->frame.unitize();
    this->content.unitize();
    this->content.scale((1 - this->padding));
  }

  void color(Color frameColor = HSV(0, 0, 1), Color contentColor = HSV(0, 0, 1)) {
    for (int i = 0; i < frame.colors().size(); i++) {frame.colors()[i] = frameColor;}
    for (int i = 0; i < content.colors().size(); i++) {content.colors()[i] = frameColor;}
  }

  /**
   * @brief draws meshes
   * @param g graphics context
   */
  void draw(Graphics &g) {
    g.draw(frame);
    g.draw(content);
  }

  /**
   * @brief overridable query function. 
   * Default checks if xy is inside frame and color fills if so
   */
  inline virtual void query(Vec2f xy) {
    if (this->inside(xy, this->frame)) {this->frame.primitive(Mesh::TRIANGLE_FAN);} 
    else {this->frame.primitive(Mesh::LINE_LOOP);}
  }

  /**
   * @brief function that checks if xy is inside a mesh
   * @param xy 
   * @param m mesh
   */
  bool inside(Vec2f xy, Mesh& m) {
    bool state = false; // outside by default
    int checks = 0;
    Vec2f min, max;
    getBounds2d(m, min, max); // find bounds
    for (int i = 0; i < 2; i++) { // loop over dims
      if (xy[i] > min[i] && xy[i] < max[i]) {checks++;} // increment checks if in bounds
    }
    if (checks == 2) {state = true;} // inside
    return state;
  }
};

/**
 * @brief Container class that holds elements and organizes them horizontally
 * TODO: add enums for organizing vertically, grid, etc
 */
class Container {
private: // member variables
  Vec2f center = Vec2f(0, 0);
  float width = 1.f, height = 1.f, padding = 0.f;
  std::vector<Element> elements;

public: // member functions
  // Container() = delete;
  Container(Vec2f center = Vec2f(0.f, 0.85f), float width = 2.f, float height = 0.25f, float padding = 0.15) : 
            center(center), width(width), height(height), padding(padding) {}

  /**
   * @brief Append elements and organize accordingly 
   * @param e element to append
   */
  void addElement(Element& e) {
    Element elem(e);
    elements.push_back(elem); // append element
    float eWidth = width/elements.size(); // calculate widths- this orients container horizontally
    float colorStep = 1.f / elements.size();
    for (int i = 0; i < elements.size(); i++) { // for each element...
      Vec2f target = center + Vec2f(-width/2 + eWidth*(i+0.5), 0); // calculate center
      elements[i].unit(); // unitize 
      elements[i].scale((eWidth/width) * (1 - padding)); // scale 
      elements[i].snap(target); // move to target
      elements[i].color (HSV(i * colorStep, 1, 1));
    }
  } 

  /**
   * @brief Draw function 
   * @param g graphics content
   */
  void draw(Graphics &g){ 
    for (int i = 0; i < elements.size(); i++) {
      elements[i].draw(g);
    }
  }

  /**
   * @brief calls the query of each element
   * @param xy coordinate to query
   */
  void query(Vec2f xy) {
    for (int i = 0; i < elements.size(); i++) {
      elements[i].query(xy);
    }
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
                  std::sin(unit * i * 2 * M_2PI)); // y
      mesh.color(HSV(0,0,1)); // color
    }
    //mesh.scale((width - padding)/width, (height + std::abs(anchor[1]) - 2*padding) / (2));
    mesh.scale(0.7);
    mesh.translate(0, -0.15f);
    //mesh.translate(anchor[0], -0.15f);
  }

  void draw(Graphics &g){g.draw(mesh);}
};

// app class
template <typename T>
struct audioUI : graphicsTemplate<T> {
  Scope scope;
  Element button;
  Container menu;
  
  void onCreate() override {
    scope.seed();
    button.addContent(scope.mesh);

    for (int i = 0; i < 6; i++) {
      menu.addElement(button);
    }
  }

  void onAnimate(double dt) override {}

  bool onMouseMove(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    menu.query(pos);
    return true;
  } 

  void onDraw(Graphics& g) override {
    graphicsTemplate<T>::onDraw(g); // call base class onDraw() 
    //button.draw(g);
    menu.draw(g); // draw menu
  }
};

int main() {
  audioUI<float> app;
  app.start();
  return 0;
}
