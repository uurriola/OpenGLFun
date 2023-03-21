#pragma once

#include <functional>
#include <vector>
#include <utility>
#include <string>

#include "imgui/imgui.h"


namespace test
{
	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};
	};

	class TestMenu : public Test
	{
	public:
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
		TestMenu() {}

		template<typename T>
		void RegisterTest(std::string testName)
		{
			m_Tests.push_back(std::make_pair(testName, []() { return new T(); }));
		}
	};
}
