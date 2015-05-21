#include "ColorPicker.h"
#include "../gfx/MaterialBank.h"
#include "imgui\imgui.h"

ColorPicker::ColorPicker(){}
ColorPicker::~ColorPicker(){}

void ColorPicker::Init()
{
	m_pickerProgram = gfx::g_ShaderBank.LoadShaderProgram("shader/ColorPicker/ColorPickerHSVH.glsl");
	m_sliderProgram = gfx::g_ShaderBank.LoadShaderProgram("shader/ColorPicker/ColorPickerSlideHSVH.glsl");

	m_picker.SetTexture("asset/ColorPicker.png");
	m_picker.SetPos(glm::vec2(0, 0));
	m_pickerIarea.SetPos(glm::vec2(0, 0));
	m_pickerIarea.SetSize(glm::vec2(256, 256));

	m_pickerMarker.SetTexture("asset/ColorPickerMarker.png");
	m_sliderMarker.SetTexture("asset/ColorPickerSliderMarker.png");
	
	m_slide.SetTexture("asset/ColorPickerSlide.png");
	m_slide.SetPos(glm::vec2(288, 0));
	m_sliderIarea.SetPos(glm::vec2(288, 0));
	m_sliderIarea.SetSize(glm::vec2(32, 256));

	m_show = true;
	m_hue = 0;

	CalculateSliderMarkerPos(glm::vec2(0, 0));
	CalculateMarkerPos(glm::vec2(0, 0));
	CalculateColor(glm::vec2(0, 0));

	GeneratePicker();
	GenerateSlider();
}

void ColorPicker::Update()
{
	if (m_show)
	{
		m_pickerIarea.Update();
		glm::vec2 clickPos;
		if (m_pickerIarea.IsClicked(clickPos))
		{
			CalculateColor(clickPos);
			CalculateMarkerPos(clickPos);
		}

		m_sliderIarea.Update();
		if (m_sliderIarea.IsClicked(clickPos))
		{
			CalculateSliderMarkerPos(clickPos);
			m_hue = (1.0f-clickPos.y) * 360;
			CalculateColor(clickPos);
			GeneratePicker();
		}
	}
}

void ColorPicker::Render(gfx::RenderQueue* rq)
{
	if (m_show)
	{
		rq->Enqueue(m_picker);
		rq->Enqueue(m_slide);
		rq->Enqueue(m_pickerMarker);
		rq->Enqueue(m_sliderMarker);
	}
}

void ColorPicker::GeneratePicker()
{
	int size = 256;
	gfx::Texture* texture = gfx::g_MaterialBank.GetTexture(m_picker.GetTexture());
	texture->Resize(size, size);

	gfx::ShaderProgram* prog = gfx::g_ShaderBank.GetProgramFromHandle(m_pickerProgram);
	prog->Apply();
	prog->SetUniformFloat("hue", m_hue);

	glBindImageTexture(0, texture->GetHandle(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	GLint x = size / 32;
	glDispatchCompute(x, x, 1);
	glUseProgram(0);
}

void ColorPicker::GenerateSlider()
{
	int size = 256;
	gfx::Texture* texture = gfx::g_MaterialBank.GetTexture(m_slide.GetTexture());
	texture->Resize(32, size);

	gfx::ShaderProgram* prog = gfx::g_ShaderBank.GetProgramFromHandle(m_sliderProgram);
	prog->Apply();
	glBindImageTexture(0, texture->GetHandle(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	GLint x = size / 32;
	glDispatchCompute(1, x, 1);
	glUseProgram(0);
}

void ColorPicker::CalculateColor(glm::vec2 uv)
{
	float h = m_hue;
	float s = uv.x;
	float v = uv.y;
	float c = v*s;
	float x = c*(1.0f - glm::abs(glm::mod(h / 60.0f, 2.0f) - 1.0f));
	float m = v - c;


	m_color = glm::vec4(m, m, m, 1.0f);
	if (h < 60)
	{
		m_color.x = c + m;
		m_color.y = x + m;
	}
	else if (h < 120)
	{
		m_color.y = c + m;
		m_color.x = x + m;
	}
	else if (h < 180)
	{
		m_color.y = c + m;
		m_color.z = x + m;
	}
	else if (h < 240)
	{
		m_color.z = c + m;
		m_color.y = x + m;
	}
	else if (h < 300)
	{
		m_color.z = c + m;
		m_color.x = x + m;
	}
	else if (h < 360)
	{
		m_color.x = c + m;
		m_color.z = x + m;
	}
}

void ColorPicker::CalculateMarkerPos(glm::vec2 uv)
{
	glm::vec2 pickerPos = m_pickerIarea.GetPos();
	glm::vec2 pickerSize = m_pickerIarea.GetSize();
	glm::vec2 markerSize = glm::vec2(16,16);

	glm::vec2 position = pickerPos + pickerSize*uv - markerSize*0.5f;
	m_pickerMarker.SetPos(position);
}

void ColorPicker::CalculateSliderMarkerPos(glm::vec2 uv)
{
	glm::vec2 sliderPos = m_sliderIarea.GetPos();
	glm::vec2 sliderSize = m_sliderIarea.GetSize();
	glm::vec2 markerSize = glm::vec2(16, 16);

	glm::vec2 position;
	position.x = sliderPos.x + sliderSize.x;
	position.y = sliderPos.y + sliderSize.y*uv.y - markerSize.y*0.5f;
	m_sliderMarker.SetPos(position);
}


