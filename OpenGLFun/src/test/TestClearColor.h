#pragma once
#include "Test.h"

namespace test
{
    class TestClearColor :
        public Test
    {
    private:
        float m_Color[4];
    public:
        TestClearColor();
        ~TestClearColor();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}

