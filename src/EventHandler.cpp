#include "EventHandler.h"
#include <stdio.h>
#include <iostream>

#define PAN_BUTTON sf::Mouse::Middle

#define ZOOM_DELTA 0.1f

EventHandler::EventHandler(sf::RenderWindow &win, NetworkMap& networkmap) : window(win), networkMap(networkmap)
{
	dragging = false;
}

void EventHandler::handle(sf::Event &event)
{
	if (event.MouseMoved) {
		mouseMoved(event);
	}

	if (event.MouseButtonPressed) {
		mouseButtonPressed(event);
	}

	if (event.MouseButtonReleased) {
		mouseButtonReleased(event);
	}

	if (event.MouseWheelScrolled) {
		mouseWheelScrolled(event);
	}
}

void EventHandler::mouseMoved(sf::Event &event)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition();

	if (dragging)
	{
		// TODO: Make panning slow down with zoom
		sf::Vector2i delta = lastPos - mousePos;
		networkMap.view.move((sf::Vector2f)delta);
		lastPos = mousePos;
		return;
	}

	lastPos = mousePos;
	networkMap.isNodeHovered = false;

	networkMap.handleNodeHover(window);
}

void EventHandler::mouseWheelScrolled(sf::Event &event)
{
	float zoomDelta = std::clamp(event.mouseWheelScroll.delta, -ZOOM_DELTA, ZOOM_DELTA);
	networkMap.view.zoom(1 - zoomDelta);
}

void EventHandler::mouseButtonPressed(sf::Event &event)
{

	if (event.mouseButton.button == PAN_BUTTON) {
		dragging = true;
	}

	if (event.mouseButton.button == sf::Mouse::Left) {
		networkMap.handleNodeSelection(window);
	}
}

void EventHandler::mouseButtonReleased(sf::Event &event)
{
	if (event.mouseButton.button == PAN_BUTTON)
	{
		dragging = false;
	}
}