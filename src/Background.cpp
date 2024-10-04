#include "Background.h"
#include "TextureManager.h"

Background::Background(const std::string& file_path, std::string name, bool centered)
{
	m_name = name;
	TextureManager::Instance().Load(file_path, m_name);
	const auto size = TextureManager::Instance().GetTextureSize(m_name);
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	setIsCentered(centered);
	SetType(GameObjectType::BACKGROUND);
}

Background::~Background() = default;

void Background::Draw()
{
	TextureManager::Instance().Draw(m_name, GetTransform()->position, 0, 255, isCentered());
}

void Background::Update()
{

}

void Background::Clean()
{

}