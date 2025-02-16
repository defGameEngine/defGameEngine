#pragma once

#ifndef DGE_CONSOLE_HPP
#define DGE_CONSOLE_HPP

#include <vector>

#include "Pixel.hpp"
#include "Layer.hpp"
#include "defGameEngine.hpp"

namespace def
{
    class Console
    {
    public:
        struct Entry
        {
            std::string command;
            std::string output;

            Pixel outputColour;
        };

        explicit Console(Layer* layer);

        void Clear();
        void HandleCommand(const std::string& command);
        void HandleHistoryBrowsing();

        void Show(bool show);
        void SetBackgroundColour(const Pixel& colour);

    private:
        Pixel m_BackgroundColour;

        std::vector<Entry> m_History;
		size_t m_PickedHistoryCommand;

        Layer* m_Layer;

    };
}

#endif
