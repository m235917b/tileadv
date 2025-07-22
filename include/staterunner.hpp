#pragma once

class StateRunner
{
    public:
        StateRunner() = default;

        virtual const int run() = 0;
};