#include "plugin.hpp"

struct ArduinoTest : Module {

  ArduinoTest() {
    config(0, 0, 0, 0);
  }

  void process(const ProcessArgs& args) override {}
};

struct ArduinoTestWidget : ModuleWidget {
  ArduinoTestWidget(ArduinoTest* module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ArduinoTest.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
  }
};

Model* modelArduinoTest = createModel<ArduinoTest, ArduinoTestWidget>("ArduinoTest");
