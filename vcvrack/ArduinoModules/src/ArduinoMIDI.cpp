#include "plugin.hpp"

struct MidiOutput : dsp::MidiGenerator<PORT_MAX_CHANNELS>, midi::Output {
	void onMessage(midi::Message message) override {
		midi::Output::sendMessage(message);
	}

	void reset() {
		Output::reset();
		MidiGenerator::reset();
	}
};

struct ArduinoMIDI : Module {

  midi::InputQueue midiInput;
  MidiOutput midiOutput;

  ArduinoMIDI() {
    config(0, 0, 0, 0);
  }

  void process(const ProcessArgs& args) override {}
};

struct ArduinoMIDIWidget : ModuleWidget {
  ArduinoMIDIWidget(ArduinoMIDI* module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ArduinoMIDI.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    MidiWidget* midiInputWidget = createWidget<MidiWidget>(mm2px(Vec(3.41891, 14.8373)));
		midiInputWidget->box.size = mm2px(Vec(33.840, 28));
		midiInputWidget->setMidiPort(module ? &module->midiInput : NULL);
		addChild(midiInputWidget);

		MidiWidget* midiOutputWidget = createWidget<MidiWidget>(mm2px(Vec(3.41891, 44.8373)));
		midiOutputWidget->box.size = mm2px(Vec(33.840, 28));
		midiOutputWidget->setMidiPort(module ? &module->midiOutput : NULL);
		addChild(midiOutputWidget);
  }
};

Model* modelArduinoMIDI = createModel<ArduinoMIDI, ArduinoMIDIWidget>("ArduinoMIDI");
