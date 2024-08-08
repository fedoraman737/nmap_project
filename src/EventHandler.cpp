#include "EventHandler.h"
#include <stdio.h>
#include <iostream>

#define PAN_BUTTON sf::Mouse::Middle

#define ZOOM_MIN -0.1f
#define ZOOM_MAX 0.1f

EventHandler::EventHandler(sf::RenderWindow &win, NetworkMap &networkmap) : window(win), networkMap(networkmap)
{
	dragging = false;
}

void EventHandler::handle(sf::Event &event)
{
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		mouseMoved(event);
		break;
	case sf::Event::MouseButtonPressed:
		mouseButtonPressed(event);
		break;
	case sf::Event::MouseButtonReleased:
		mouseButtonReleased(event);
		break;
	case sf::Event::MouseWheelScrolled:
		mouseWheelScrolled(event);
		break;
	}
}

void EventHandler::mouseMoved(sf::Event &event)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition();

	if (dragging)
	{
		sf::Vector2i delta = lastPos - mousePos;
		networkMap.view.move((sf::Vector2f)delta);
		lastPos = mousePos;
		return;
	}

	lastPos = mousePos;
	networkMap.isNodeHovered = false;


	sf::Vector2f relativeMousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
	for (const Host host : networkMap.hosts)
	{
		sf::Vector2f nodePos = networkMap.hostPositions[host.ip];
		sf::CircleShape node(10.0f);
		node.setOrigin(10.0f, 10.0f);
		node.setPosition(nodePos);

		if (node.getGlobalBounds().contains(relativeMousePos))
		{
			networkMap.hoveredNode = nodePos;
			networkMap.isNodeHovered = true;
			break;
		}
	}
}

void EventHandler::mouseWheelScrolled(sf::Event &event)
{
	float zoomDelta = std::clamp(event.mouseWheelScroll.delta, ZOOM_MIN, ZOOM_MAX);
	networkMap.zoomFactor += zoomDelta;
}

void EventHandler::mouseButtonPressed(sf::Event &event)
{

	if (event.mouseButton.button == PAN_BUTTON)
	{
		std::cout << "MouseButtonPressed" << std::endl;
		dragging = true;
	}

	if (event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
		networkMap.isNodeHighlighted = false;

		for (const auto &host : networkMap.hosts)
		{
			sf::Vector2f nodePos = networkMap.hostPositions[host.ip];
			sf::CircleShape node(10.0f);
			node.setOrigin(10.0f, 10.0f);
			node.setPosition(nodePos);
			if (node.getGlobalBounds().contains(mousePos))
			{
				networkMap.highlightedNode = nodePos;
				networkMap.isNodeHighlighted = true;
				networkMap.selectedHost = const_cast<Host *>(&host);
				std::cout << "Selected host: " << networkMap.selectedHost->ip << "\n";
				break;
			}
		}
	}
}

void EventHandler::mouseButtonReleased(sf::Event &event)
{
	if (event.mouseButton.button == PAN_BUTTON)
	{
		dragging = false;
		std::cout << "MouseButtonReleased" << std::endl;
	}
}