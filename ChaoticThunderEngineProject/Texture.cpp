#include "Texture.hpp"
#include <stb_image.h>
#include "Controller.hpp"

Texture::Texture(std::filesystem::path path, bool has_alpha) :
	_texture_ID(-1),
	_has_alpha(false),
	Width(-1),
	Height(-1),
	Channels(-1),
	Data(nullptr)
{
	this->Path = path;
}
Texture::~Texture() {
	FreeCPU();
	FreeGPU();
}

void Texture::LoadCPU() {
	this->Data = stbi_load(this->Path.generic_string().c_str(), &this->Width, &this->Height, &this->Channels, this->_has_alpha ? 4 : 3);
}
void Texture::TransferToGPU(bool destroy_cpu_copy) {
	if (this->Data == nullptr)
		LoadCPU();

    glGenTextures(1, &this->_texture_ID);
	SetActive();
    glBindTexture(GL_TEXTURE_2D, this->_texture_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int format = Channels == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, this->Width, this->Height, 0, format, GL_UNSIGNED_BYTE, this->Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (destroy_cpu_copy)
		FreeCPU();
}

void Texture::SetActive(int i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, this->_texture_ID);
}

void Texture::FreeCPU() {
	delete[] this->Data;
	this->Width = -1;
	this->Height = -1;
	this->Channels = -1;
	this->Data = nullptr;
}
void Texture::FreeGPU() {
	glDeleteTextures(1, &this->_texture_ID);
	this->_texture_ID = -1;
}