#include "ColorPicker.h"
#include "../gfx/MaterialBank.h"
#include "imgui\imgui.h"

glm::vec4 ColorPicker::m_color;

ColorPicker::ColorPicker(){}
ColorPicker::~ColorPicker(){}

void ColorPicker::Init(glm::vec2 position, float scale, bool show)
{
	m_scale = scale;

	// Shaders
	m_pickerProgram = gfx::g_ShaderBank.LoadShaderProgram("shader/ColorPicker/ColorPickerHSVH.glsl");
	m_sliderProgram = gfx::g_ShaderBank.LoadShaderProgram("shader/ColorPicker/ColorPickerSlideHSVH.glsl");

	glm::vec2 size;
	glm::vec2 pos;

	// Picker
	m_picker.SetTexture("asset/ColorPicker/ColorPicker.png");
	size = glm::vec2(256.0f, 256.0f)*m_scale;
	pos = glm::vec2(8.0f, 8.0f)*m_scale;
	m_picker.SetPos(position+pos);
	m_picker.SetSize(size);
	m_pickerIarea.SetPos(position+pos);
	m_pickerIarea.SetSize(size);

	// Slider
	m_slide.SetTexture("asset/ColorPicker/ColorPickerSlide.png");
	size = glm::vec2(32.0f, 256.0f)*m_scale;
	pos = glm::vec2(272.0f, 8.0f)*m_scale;
	m_slide.SetPos(position+pos);
	m_slide.SetSize(size);
	m_sliderIarea.SetPos(position+pos);
	m_sliderIarea.SetSize(size);

	// Markers
	m_pickerMarker.SetTexture("asset/ColorPicker/ColorPickerMarkerInv.png");
	m_pickerMarker.SetTexture("asset/ColorPicker/ColorPickerMarker.png");
	size = glm::vec2(16.0f, 16.0f)*m_scale;
	m_pickerMarker.SetSize(size);
	m_sliderMarker.SetTexture("asset/ColorPicker/ColorPickerSliderMarker.png");
	size = glm::vec2(48.0f, 8.0f)*m_scale;
	m_sliderMarker.SetSize(size);

	// Background
	m_background.SetTexture("asset/ColorPicker/ColorPickerBorder.png");
	size = glm::vec2(312.0f, 272.0f)*m_scale;
	pos = glm::vec2(0, 0)*m_scale;
	m_background.SetPos(position+pos);
	m_background.SetSize(size);

	m_show = show;
	m_hue = 0;
	m_lastClick = glm::vec2(0, 0);

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
		ImGuiIO& io = ImGui::GetIO();
		
		// Update Picker
		if (m_pickerIarea.IsClicked(clickPos))
		{
			m_pickerActive = true;
		}
		if (m_pickerActive)
		{
			if (!io.MouseDown[0])
			{
				m_pickerActive = false;
			}
			clickPos = glm::vec2(io.MousePos.x, io.MousePos.y);
			clickPos = (clickPos - m_pickerIarea.GetPos()) / m_pickerIarea.GetSize();
			clickPos = glm::clamp(clickPos, 0.0f, 1.0f);
			m_lastClick = clickPos;
			CalculateColor(clickPos);
			CalculateMarkerPos(clickPos);
		}

		// Update Slider
		m_sliderIarea.Update();
		if (m_sliderIarea.IsClicked(clickPos))
		{
			m_sliderActive = true;
		}
		if (m_sliderActive)
		{
			if (!io.MouseDown[0])
			{
				m_sliderActive = false;
			}
			clickPos = glm::vec2(io.MousePos.x, io.MousePos.y);
			clickPos = (clickPos - m_pickerIarea.GetPos()) / m_pickerIarea.GetSize();
			clickPos = glm::clamp(clickPos, 0.0f, 1.0f);
			CalculateSliderMarkerPos(clickPos);
			m_hue = (1.0f - clickPos.y) * 360;
			CalculateColor(m_lastClick);
			GeneratePicker();
		}
	}
}

void ColorPicker::Draw(gfx::RenderQueue* rq)
{
	if (m_show)
	{
		rq->Enqueue(m_picker);
		rq->Enqueue(m_slide);
		rq->Enqueue(m_pickerMarker);
		rq->Enqueue(m_background);
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
	float v = 1.0f-uv.y;
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
	else if (h <= 360)
	{
		m_color.x = c + m;
		m_color.z = x + m;
	}
}

void ColorPicker::CalculateMarkerPos(glm::vec2 uv)
{
	glm::vec2 pickerPos = m_pickerIarea.GetPos();
	glm::vec2 pickerSize = m_pickerIarea.GetSize();
	glm::vec2 markerSize = glm::vec2(16,16)*m_scale;

	glm::vec2 position = pickerPos + pickerSize*uv - markerSize*0.5f;
	m_pickerMarker.SetPos(position);

	if (uv.y > 0.75f)
	{
		m_pickerMarker.SetTexture("asset/ColorPicker/ColorPickerMarkerInv.png");
	}
	else
	{
		m_pickerMarker.SetTexture("asset/ColorPicker/ColorPickerMarker.png");
	}
}

void ColorPicker::CalculateSliderMarkerPos(glm::vec2 uv)
{
	glm::vec2 sliderPos = m_sliderIarea.GetPos();
	glm::vec2 sliderSize = m_sliderIarea.GetSize();
	glm::vec2 markerSize = glm::vec2(8, 8)*m_scale;

	glm::vec2 position;
	position.x = sliderPos.x-8;
	position.y = sliderPos.y + sliderSize.y*uv.y-markerSize.y*0.5f;
	m_sliderMarker.SetPos(position);
}

void ColorPicker::TogglePicker()
{
	m_show = !m_show;
}

//glm::vec4 ColorPicker::GetColor()
//{
//	return m_color;
//}


