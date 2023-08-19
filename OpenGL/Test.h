#pragma once

namespace Tests {

	class Test {

	public:
		Test() {};
		virtual ~Test() {}

		virtual void OnUpdate(const float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnGuiRender() {}


	};
}

