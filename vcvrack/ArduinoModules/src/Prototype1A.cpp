#include "plugin.hpp"

#define MIDI_CC_BINARY 14
#define MIDI_CC_POTMETER 15
#define MIDI_CC_STATUS 0xb
#define PUSH_BUTTON_OFF 8
#define PUSH_BUTTON_ON 9
#define ENCODER_BUTTON_OFF 10
#define ENCODER_BUTTON_ON 11
#define ENCODER_OUTPUT_A 12
#define ENCODER_OUTPUT_B 13

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
struct Prototype1A : Module {

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
		OUTPUT_POTMETER,
		OUTPUT_POTMETER_INVERSE,
		OUTPUT_PUSHBUTTON,
		OUTPUT_ENCODER,
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
	float encoderValue = 0;
	float noteVoltage = 1.0f / 12.0f;

  Prototype1A() {
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
			processMIDIInputMessage(msg);
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
	
	void processMIDIInputMessage(midi::Message msg) {
		switch (msg.getStatus()) {
			case MIDI_CC_STATUS:
				processMIDIInputCC(msg.bytes, msg.size);
				break;
			default:
				break;
		}
	}

	void processMIDIInputCC(uint8_t* bytes, uint8_t size) {
		if (size == 3) {
			switch (bytes[1]) {
				case MIDI_CC_BINARY: {
					switch (bytes[2]) {
						case PUSH_BUTTON_OFF:
							outputs[OUTPUT_PUSHBUTTON].value = 0;
							break;
						case PUSH_BUTTON_ON:
							outputs[OUTPUT_PUSHBUTTON].value = 5;
							break;
						case ENCODER_BUTTON_OFF:
							break;
						case ENCODER_BUTTON_ON:
							break;
						case ENCODER_OUTPUT_A: {
							encoderValue -= noteVoltage;
							outputs[OUTPUT_ENCODER].value = encoderValue;
							break;
						}
						case ENCODER_OUTPUT_B: {
							encoderValue += noteVoltage;
							outputs[OUTPUT_ENCODER].value = encoderValue;
							break;
						}
						default:
							break;
					}
					break;
				}
				case MIDI_CC_POTMETER: {
					double potmeterValue = (bytes[2] / 127.f) * 5.f;
					outputs[OUTPUT_POTMETER].value = potmeterValue;
					outputs[OUTPUT_POTMETER_INVERSE].value = 5 - potmeterValue;
					break;
				}
				default:
					break;
			}
		}
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

struct Prototype1AWidget : ModuleWidget {
  Prototype1AWidget(Prototype1A* module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Prototype1A.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    MidiWidget* midiInputWidget = createWidget<MidiWidget>(mm2px(Vec(3.41891, 16)));
		midiInputWidget->box.size = mm2px(Vec(33.840, 28));
		midiInputWidget->setMidiPort(module ? &module->midiInput : NULL);
		addChild(midiInputWidget);

		MidiWidget* midiOutputWidget = createWidget<MidiWidget>(mm2px(Vec(3.41891, 45)));
		midiOutputWidget->box.size = mm2px(Vec(33.840, 28));
		midiOutputWidget->setMidiPort(module ? &module->midiOutput : NULL);
		addChild(midiOutputWidget);

		float center = 20.25;
		float y = 78;
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center - 9, y)), module, Prototype1A::LED_0));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center - 3, y)), module, Prototype1A::LED_1));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center + 3, y)), module, Prototype1A::LED_2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center + 9, y)), module, Prototype1A::LED_3));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center - 9, y + 6)), module, Prototype1A::LED_4));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center - 3, y + 6)), module, Prototype1A::LED_5));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center + 3, y + 6)), module, Prototype1A::LED_6));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(center + 9, y + 6)), module, Prototype1A::LED_7));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center - 13.5, y + 14)), module, Prototype1A::INPUT_0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center - 4.5, y + 14)), module, Prototype1A::INPUT_1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center + 4.5, y + 14)), module, Prototype1A::INPUT_2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center + 13.5, y + 14)), module, Prototype1A::INPUT_3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center - 13.5, y + 24)), module, Prototype1A::INPUT_4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center - 4.5, y + 24)), module, Prototype1A::INPUT_5));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center + 4.5, y + 24)), module, Prototype1A::INPUT_6));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(center + 13.5, y + 24)), module, Prototype1A::INPUT_7));
		
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(center - 13.5, y + 36)), module, Prototype1A::OUTPUT_POTMETER));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(center - 4.5, y + 36)), module, Prototype1A::OUTPUT_POTMETER_INVERSE));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(center + 4.5, y + 36)), module, Prototype1A::OUTPUT_PUSHBUTTON));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(center + 13.5, y + 36)), module, Prototype1A::OUTPUT_ENCODER));
  }
};

Model* modelPrototype1A = createModel<Prototype1A, Prototype1AWidget>("Prototype1A");
