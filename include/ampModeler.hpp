#ifndef AMP_MODELER_HPP
#define AMP_MODELER_HPP

#include "../../Gimmel/include/utility.hpp"
#include "../RTNeural/modules/rt-nam/rt-nam.hpp"

// Add NAM compatibility to giml
namespace giml {
  template<typename T, typename Layer1, typename Layer2, unsigned samplesPerBuffer>
  class AmpModeler : public Effect<T>, public wavenet::RTWavenet<1, samplesPerBuffer, Layer1, Layer2> {
  public:
    // Add default constructor
    AmpModeler() {
      // Initialize any necessary members here
      this->enabled = false;
    }

    // Destructor
    ~AmpModeler() {}

    // Copy constructor
    AmpModeler(const AmpModeler<T, Layer1, Layer2, samplesPerBuffer>& other) : 
    Effect<T>(other), wavenet::RTWavenet<1, samplesPerBuffer, Layer1, Layer2>(other) {}

    // Copy assignment operator
    AmpModeler<T, Layer1, Layer2, samplesPerBuffer>& operator=(const AmpModeler<T, Layer1, Layer2, samplesPerBuffer>& other) {
      Effect<T>::operator=(other);
      wavenet::RTWavenet<1, samplesPerBuffer, Layer1, Layer2>::operator=(other);
      return *this;
    }

    /**
     * @brief Loads the amp model from a vector of weights
     * 
     * @param weights Vector of float weights for the neural network model
     */
    void loadModel(std::vector<float> weights) {
      this->model.load_weights(weights);
      this->model.prepare(samplesPerBuffer); // Prepare for single sample processing
      this->model.prewarm();
    }
    
    /**
     * @brief Process a single sample through the amp model
     * 
     * @param input Input sample
     * @return T Processed sample
     */
    T processSample(const T& input) override {
      if (!this->enabled) { return input; }
      return this->model.forward(input);;
    }

    void processBuffer(const float* input, float* output, size_t size) {
      if (!this->enabled) { return; }
      this->model.forward(input, output, size);
    }    

  };
} // namespace giml

#endif // AMP_MODELER_HPP