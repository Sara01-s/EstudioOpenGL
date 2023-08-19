#pragma once
#include "Test.h"

namespace Tests {

	class TestClearColor : public Test {

	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(const float deltaTime) final override;
		void OnRender() override;
		void OnGuiRender() override;
		
	private:
		float m_ClearColor[4];
	};

} // namespace Tests

