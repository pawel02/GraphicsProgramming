#pragma once

class Layer
{
public:
	virtual ~Layer() {};

	virtual void on_attach() {};
	virtual void on_detach() = 0;
	virtual void on_update(float deltaTime) = 0;
};