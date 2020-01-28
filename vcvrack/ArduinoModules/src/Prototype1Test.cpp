#include "plugin.hpp"

#define MIDI_CC_BINARY 14
#define MIDI_CC_POTMETER 15

/**
 * 
 */
struct MidiOutput : dsp::MidiGenerator<PORT_MAX_CHANNELS>, midi::Output {

	void onMessage(midi::Message message) override {
		midi::Output::sendMessage(message);
	}

	void reset() {
		Output::reset();
		MidiGenerator::reset();
	}
};

/**
 * Data needed have an input trigger a light and send MIDI CCs.
 */
struct input {
	int controllerOff;
	int controllerOn;
	int inputId;
	int lightId;
	dsp::SchmittTrigger triggerOff;
	dsp::SchmittTrigger triggerOn;
};

/**
 * 
 */
struct Prototype1Test : Module {

	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		INPUT_0,
		INPUT_1,
		INPUT_2,
		INPUT_3,
		INPUT_4,
		INPUT_5,
		INPUT_6,
		INPUT_7,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		LED_0,
		LED_1,
		LED_2,
		LED_3,
		LED_4,
		LED_5,
		LED_6,
		LED_7,
		NUM_LIGHTS
	};

  midi::InputQueue midiInput;
  MidiOutput midiOutput;
	struct input ins[NUM_INPUTS];

  Prototype1Test() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		onReset();

		for (int i = 0; i < NUM_INPUTS; i++) {
			struct input in;
			in.controllerOff = 16 + (i * 2);
			in.controllerOn = 17 + (i * 2);
			in.inputId = i;
			in.lightId = i;
			ins[i] = in;
		}
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

		// process inputs
		for (int i = 0; i < NUM_INPUTS; i++) {
			if (inputs[i].isConnected()) {

				// if the input value of 1 triggers the schmittrigger to flip HIGH
				if (ins[i].triggerOn.process(inputs[i].value)) {
					lights[i].setBrightness(1.f);
					midiOutput.setCc(ins[i].controllerOn, MIDI_CC_BINARY);
				}

				// if the input value of 0 triggers the schmittrigger to flip HIGH
				if (ins[i].triggerOff.process(1 - inputs[i].value)) {
					lights[i].setBrightness(0);
					midiOutput.setCc(ins[i].controllerOff, MIDI_CC_BINARY);
				}
			}
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
		lights[LED_0].setBrightness(1.f);

		midiOutput.setVelocity(0, 1);
		midiOutput.setNoteGate(60, false, 1);
	}

	void releaseNote(uint8_t note) {
		lights[LED_0].setBrightness(0.f);

		midiOutput.setVelocity(100, 1);
		midiOutput.setNoteGate(60, true, 1);
	}

	/**
	 * Save module state to JSON.
	 */
	json_t* dataToJson() override {
		json_t* rootJSON = json_object();
		json_object_set_new(rootJSON, "midiInput", midiInput.toJson());
		json_object_set_new(rootJSON, "midiOutput", midiOutput.toJson());
		return rootJSON;
	}

	/**
	 * Load module state from JSON.
	 */
	void dataFromJson(json_t* rootJSON) override {
		json_t* midiInputJSON = json_object_get(rootJSON, "midiInput");
		if (midiInputJSON) {
			midiInput.fromJson(midiInputJSON);
		}
		json_t* midiOutputJSON = json_object_get(rootJSON, "midiOutput");
		if (midiOutputJSON) {
			midiOutput.fromJson(midiOutputJSON);
		}
	}
};

struct Prototype1TestWidget : ModuleWidget {
  Prototype1TestWidget(Prototype1Test* module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Prototype1Test.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    MidiWidget* midiInputWidget = createWidget<MidiWidget>(mm2px(Vec(3.41891, 16)));
		midiInputWidget->box.size = mm2px(Vec(33.840, 28));
		midiInputWidget->setMidiPort(module ? &module->midiInput : NULL);
		addChild(midiInputWidget);

		MidiWidget* midiOutputWidget = createWidget<MidiWidget>(mm2px(Vec(3.41891, 46)));
		midiOutputWidget->box.size = mm2px(Vec(33.840, 28));
		midiOutputWidget->setMidiPort(module ? &module->midiOutput : NULL);
		addChild(midiOutputWidget);

		float center = 20.25;
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center - 9, 80)), module, Prototype1Test::LED_0));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center - 3, 80)), module, Prototype1Test::LED_1));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center + 3, 80)), module, Prototype1Test::LED_2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center + 9, 80)), module, Prototype1Test::LED_3));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center - 9, 86)), module, Prototype1Test::LED_4));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center - 3, 86)), module, Prototype1Test::LED_5));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center + 3, 86)), module, Prototype1Test::LED_6));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center + 9, 86)), module, Prototype1Test::LED_7));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center - 13.5, 95)), module, Prototype1Test::INPUT_0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center - 4.5, 95)), module, Prototype1Test::INPUT_1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center + 4.5, 95)), module, Prototype1Test::INPUT_2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center + 13.5, 95)), module, Prototype1Test::INPUT_3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center - 13.5, 105)), module, Prototype1Test::INPUT_4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center - 4.5, 105)), module, Prototype1Test::INPUT_5));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center + 4.5, 105)), module, Prototype1Test::INPUT_6));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center + 13.5, 105)), module, Prototype1Test::INPUT_7));
  }
};

Model* modelPrototype1Test = createModel<Prototype1Test, Prototype1TestWidget>("Prototype1Test");
