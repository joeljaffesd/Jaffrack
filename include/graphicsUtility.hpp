#ifndef GRAPHICS_UTILITY_HPP
#define GRAPHICS_UTILITY_HPP

#include "../Gimmel/include/utility.hpp"
#include "al/app/al_App.hpp"
using namespace al;

auto primType(double phase) {
  int type = static_cast<unsigned int>(phase);
  switch (type){
    case 0: return Mesh::POINTS; break;
    case 1: return Mesh::LINES; break;
    case 2: return Mesh::LINE_STRIP; break;
    case 3: return Mesh::LINE_LOOP; break;
    case 4: return Mesh::TRIANGLES; break;
    case 5: return Mesh::TRIANGLE_STRIP; break;
    case 6: return Mesh::TRIANGLE_FAN; break;
    case 7: return Mesh::LINES_ADJACENCY; break;
    case 8: return Mesh::LINE_STRIP_ADJACENCY; break;
    case 9: return Mesh::TRIANGLES_ADJACENCY; break;
    case 10: return Mesh::TRIANGLE_STRIP_ADJACENCY; break;
    default: std::cout << "Unknown Primitive" << std::endl; 
    return Mesh::POINTS; break;
  }
}

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
protected:
  Vec2f center = Vec2f(0, 0);
  float width = 1.f, height = 1.f, padding = 0;
  Mesh frame; 
  std::vector<Mesh*> content;
  Color frameColor = HSV(0, 0, 1), contentColor = HSV(0, 0, 1);

public:
  Element() = delete;
  Element(Vec2f center = Vec2f(0,0), 
          float width = 2.f, 
          float height = 2.f, 
          float padding = 0, 
          Color frameColor = HSV(0, 0, 1), 
          Color contentColor = HSV(0, 0, 1)) : 
          center(center), width(width), height(height), padding(padding), frameColor(frameColor), contentColor(contentColor) 
  {
    makeFrame(frame, width, height, center[0], center[1], frameColor);
    frame.primitive(Mesh::LINE_LOOP);
  }

  // Copy constructor - deep copy content meshes
  Element(const Element& e) {
    this->center = e.center;
    this->width = e.width;
    this->height = e.height;
    this->padding = e.padding;
    this->frameColor = e.frameColor;
    this->contentColor = e.contentColor;
    this->frame.copy(e.frame);
    
    // Deep copy content meshes
    for (const auto* meshPtr : e.content) {
      if (meshPtr) {
        al::Mesh* newMesh = new al::Mesh();
        newMesh->copy(*meshPtr);
        this->content.push_back(newMesh);
      }
    }
  }

  // Destructor - clean up dynamically allocated meshes
  virtual ~Element() {
    for (auto* meshPtr : content) {
      delete meshPtr;
    }
    content.clear();
  }

  // Copy assignment operator - proper deep copy
  Element& operator=(const Element& e) {
    if (this != &e) {
      // Clean up existing content
      for (auto* meshPtr : content) {
        delete meshPtr;
      }
      content.clear();
      
      // Copy all members
      center = e.center;
      width = e.width;
      height = e.height;
      padding = e.padding;
      frameColor = e.frameColor;
      contentColor = e.contentColor;
      frame.copy(e.frame);
      
      // Deep copy content meshes
      for (const auto* meshPtr : e.content) {
        if (meshPtr) {
          al::Mesh* newMesh = new al::Mesh();
          newMesh->copy(*meshPtr);
          content.push_back(newMesh);
        }
      }
    }
    return *this;
  }

  /**
   * @brief copies a mesh to content 
   * @param m mesh to be copied
   */
  void addContent(Mesh& m) {
    al::Mesh* meshPtr = new al::Mesh();
    meshPtr->copy(m);
    meshPtr->unitize(); // unitize for scaling
    meshPtr->scale((std::sqrt(width*width + height*height) * (1 - padding)) / std::sqrt(8)); // scale
    meshPtr->translate(center[0], center[1]); // snap to center of frame
    
    // Set colors to the element's content color
    for (int i = 0; i < meshPtr->colors().size(); i++) {
      meshPtr->colors()[i] = contentColor;
    }
    
    this->content.emplace_back(meshPtr); // store pointer
  }

  /**
   * @brief updates center and snaps meshes to it
   */
  void snap(Vec2f xy) {
    center = xy;
    snapTo(frame, xy);
    for (auto& mesh : content) {
      snapTo(*mesh, xy);
    }
  }

  /**
   * @brief scales meshes by a factor
   * @param s scaling factor
   */
  void scale(float s) {
    width = width * s;
    height = height * s;
    this->frame.scale(s);
    for (auto& mesh : content) {
      mesh->scale(s);
    }
  }

  /**
   * @brief unitizes meshes
   */
  void unit() {
    this->frame.unitize();
    for (auto& mesh : content) {
      mesh->unitize();
      mesh->scale((1 - this->padding));
    }
  }

  /**
   * @brief colors meshes 
   * @param frameColor
   * @param contentColor
   */
  void color(Color frameColor = HSV(0, 0, 1), Color contentColor = HSV(0, 0, 1)) {
    for (int i = 0; i < frame.colors().size(); i++) {frame.colors()[i] = frameColor;}
    for (auto& mesh : content) {
      for (int i = 0; i < mesh->colors().size(); i++) {mesh->colors()[i] = frameColor;}
    }
  }

  /**
   * @brief draws meshes
   * @param g `al::Graphics` object
   */
  virtual void draw(Graphics& g) {
    g.draw(frame);
    for (auto& mesh : content) {
      g.draw(*mesh);
    }
  }

  /**
   * @brief overridable query function. 
   * Default checks if xy is inside frame and color fills if so
   */
  inline virtual bool query(Vec2f xy) {
    if (this->inside(xy, this->frame)) {
      this->frame.primitive(Mesh::TRIANGLE_FAN);
      return true;
    } 
    this->frame.primitive(Mesh::LINE_LOOP);
    return false;
  }

  /**
   * @brief overridable query function. 
   * Default checks if xy is inside frame and color fills if so
   */
  inline virtual void deselect() {
    this->frame.primitive(Mesh::LINE_LOOP);
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

    if (checks == 2) {
      state = true; // inside
    } 

    return state;
  }
};

/**
 * @brief Container class that holds elements and organizes them horizontally
 * @brief constructor
 * @param center of type `al::Vec2f`
 * @param width 
 * @param height
 * @param padding (0-1)
 */
class Container {
private: // member variables
  Vec2f center = Vec2f(0, 0);
  float width = 1.f, height = 1.f, padding = 0.f;
  std::vector<std::unique_ptr<Element>> elements;

public: // member functions
  Container() = delete;
  Container(Vec2f center = Vec2f(0.f, 0.f), 
            float width = 2.f, 
            float height = 2.f, 
            float padding = 0.f) : 
            center(center), width(width), height(height), padding(padding) {}

  /**
   * @brief Append elements and organize accordingly 
   * @param e element to append (takes ownership)
   */
  void addElement(std::unique_ptr<Element> e) {
    elements.push_back(std::move(e)); // append element
    float eWidth = width/elements.size(); // calculate widths- this orients container horizontally
    float colorStep = 1.f / elements.size();
    for (size_t i = 0; i < elements.size(); ++i) { // for each element...
      Vec2f target = center + Vec2f(-width/2 + eWidth*(i+0.5f), 0); // calculate center
      elements[i]->unit(); // unitize 
      elements[i]->scale((eWidth/width) * (1 - padding)); // scale 
      elements[i]->snap(target); // move to target
      elements[i]->color (HSV(i * colorStep, 1, 1));
    }
  } 

  /**
   * @brief Draw function 
   * @param g graphics content
   */
  void draw(Graphics& g){ 
    for (auto& elem : elements) {
      elem->draw(g);
    }
  }

  /**
   * @brief calls the query of each element
   * @param xy coordinate to query
   */
  void query(Vec2f xy) {
    for (auto& elem : elements) {
      elem->query(xy);
    }
  }

  /**
   * @brief returns reference to elements vector
   */
  std::vector<std::unique_ptr<Element>>& getElements() {
    return elements;
  }

};

/** 
 * @brief Custom oscilloscope class based on Mahdi & I's work for Gimmel-Allolib-Tests.
 * This variation allows for vertical offset & scaling 
 */
class Oscilloscope : public al::Mesh {
public:
	Oscilloscope() = delete;
  /**
   * @brief constructor that requires sampleRate and yCoord.
   * @param sampleRate
   * @param yCoord
   */
	Oscilloscope(int sampleRate, float yCoord) : bufferSize(sampleRate), yCoord(yCoord) {
		this->buffer.allocate(sampleRate);
		this->primitive(al::Mesh::LINE_STRIP);
		for (int i = 0; i < bufferSize; i++) {
			this->vertex((i / static_cast<float>(this->bufferSize)) * 2.f - 1.f, yCoord);
			this->color(al::RGB(1.f));
		}
	}

  /**
   * @brief write sample data to the buffer
   * @param sample
   */
	void writeSample(float sample) {this->buffer.writeSample(sample);}

  /**
   * @brief update function that configures the mesh's vertices based on 
   * the state of the audio buffer
   */
	void update() {
		for (int i = 0; i < this->bufferSize; i++) {
			this->vertices()[i][1] = ((yCoord + this->buffer.readSample((size_t)(this->bufferSize - i))) * (1 - std::fabs(this->yCoord)));
		}
	}

  /**
   * @brief function for setting the color of the mesh
   * @param c color of `al::Color` type
   */
	void setColor(Color c) {
		for (int i = 0; i < this->bufferSize; i++) {this->colors()[i] = c;}
	}

  float yCoord;

private:
	int bufferSize;
	giml::CircularBuffer<float> buffer;
};

struct SineEmblem {
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
    mesh.scale(0.7);
    mesh.translate(0, -0.15f);
  }

  void draw(Graphics& g) {
    g.draw(mesh);
  }
};

#endif // GRAPHICS_UTILITY_HPP