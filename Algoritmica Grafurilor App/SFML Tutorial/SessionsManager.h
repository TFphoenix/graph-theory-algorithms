#pragma once
#include <cstdint>

class SessionsManager {
public:
	SessionsManager();

private:
	static const uint16_t s_windowWidth = 800;
	static const uint16_t s_windowHeight = 800;

private:
	void MainSession();
	void Tema1Session();
	void Tema2Session();
	void Tema3Session();
	void Tema4Session();
};
