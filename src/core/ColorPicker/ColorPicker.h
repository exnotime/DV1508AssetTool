#pragma once
#include "../gfx/ShaderBank.h"
#include "../gfx/Sprite.h"
#include "../gfx/RenderQueue.h"
#include "InteractiveArea\InterActiveArea.h"

class ColorPicker
{
public:
	ColorPicker();
	~ColorPicker();

	void Init(glm::vec2 position, float scale = 1.0f, bool show = true);
	void Update();
	void Draw(gfx::RenderQueue* rq);
	
	void TogglePicker();
	bool IsActive();
	//glm::vec4 GetColor()const;
	static glm::vec4 m_color;

private:
	void CalculateColor(glm::vec2 uv);
	void CalculateMarkerPos(glm::vec2 uv);
	void CalculateSliderMarkerPos(glm::vec2 uv);
	void GeneratePicker();
	void GenerateSlider();

	gfx::ShaderProgramHandle m_pickerProgram = 0;
	gfx::ShaderProgramHandle m_sliderProgram = 0;
	gfx::Sprite	m_picker;
	gfx::Sprite	m_slide;
	gfx::Sprite	m_pickerMarker;
	gfx::Sprite m_sliderMarker;
	gfx::Sprite m_background;

	InteractiveArea m_pickerIarea;
	bool m_pickerActive;

	InteractiveArea m_sliderIarea;
	bool m_sliderActive;

	bool m_show;
	float m_hue;
	float m_scale;
	glm::vec2 m_lastClick;

};

