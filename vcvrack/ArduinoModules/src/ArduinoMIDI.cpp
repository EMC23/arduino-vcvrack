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

	enum LightIds {
		NOTE_IN_LIGHT,
		NUM_LIGHTS
	};

  midi::InputQueue midiInput;
  MidiOutput midiOutput;

  ArduinoMIDI() {
    config(0, 0, 0, NUM_LIGHTS);
		onReset();
  }

	void onReset() override {
		midiInput.reset();
		midiOutput.reset();
	}

  void process(const ProcessArgs& args) override {
		midi::Message msg;
		while (midiInput.shift(&msg)) {
			processMessage(msg);
		}
	}

	void processMessage(midi::Message msg) {
		switch (msg.getStatus()) {
			// note off
			case 0x8: {
				releaseNote(msg.getNote());
			} break;
			// note on
			case 0x9: {
				if (msg.getValue() > 0) {
					pressNote(msg.getNote(), msg.getValue());
				}
				else {
					// Many stupid keyboards send a "note on" command with 0 velocity to mean "note release"
					releaseNote(msg.getNote());
				}
			} break;
			default: break;
		}
	}

	void pressNote(uint8_t note, uint8_t vel) {
		lights[NOTE_IN_LIGHT].setBrightness(1.f);

		midiOutput.setVelocity(0, 1);
		midiOutput.setNoteGate(60, false, 1);
	}

	void releaseNote(uint8_t note) {
		lights[NOTE_IN_LIGHT].setBrightness(0.f);

		midiOutput.setVelocity(100, 1);
		midiOutput.setNoteGate(60, true, 1);
	}
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

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(10, 80)), module, ArduinoMIDI::NOTE_IN_LIGHT));
  }
};

Model* modelArduinoMIDI = createModel<ArduinoMIDI, ArduinoMIDIWidget>("ArduinoMIDI");
